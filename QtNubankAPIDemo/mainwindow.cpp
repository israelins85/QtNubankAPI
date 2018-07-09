#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include <QSettings>
#include <qnubankapi.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    nubankAPI(new QNubankAPI(this))
{
    QSettings set;
    ui->setupUi(this);
    ui->edtLogin->setText(set.value("user/login").toString());
    ui->edtPassword->setText(set.value("user/password").toString());
    ui->ckbRemember->setChecked(set.value("user/remember").toBool());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnTest_clicked()
{
    QSettings set;

    set.setValue("user/remember", ui->ckbRemember->isChecked());
    if (ui->ckbRemember->isChecked()) {
        set.setValue("user/login", ui->edtLogin->text());
        set.setValue("user/password", ui->edtPassword->text());
    } else {
        set.remove("user/login");
        set.remove("user/password");
    }

    nubankAPI->authenticate(QNubankAPI::AuthRequest(ui->edtLogin->text(), ui->edtPassword->text()), [this](const QNubankAPI::Response& response) {
        if (!response.sucess) return;

//        nubankAPI->cardFeed([](const QNubankAPI::Response& response) {
//            if (!response.sucess) return;
//        });
    });
}
