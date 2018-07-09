#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qnubankapi.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnTest_clicked();

private:
    Ui::MainWindow *ui;
    QNubankAPI* nubankAPI = nullptr;
};

#endif // MAINWINDOW_H
