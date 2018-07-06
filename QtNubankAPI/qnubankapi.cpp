#include "qnubankapi.h"

class QNubankAPIPrivate {
    Q_DISABLE_COPY(CoordinateDialogPrivate)
    Q_DECLARE_PUBLIC(CoordinateDialog)
    QNubankAPI* const q_ptr;

    QNubankAPIPrivate(QNubankAPI* q) : q_ptr(q) {}

    QString discoveryUrl = "https://prod-s0-webapp-proxy.nubank.com.br/api/discovery";
    QMap<QString, QString> defaultHeader = {
        { "Content-Type", "application/json"},
        { "X-Correlation-Id", "WEB-APP.pewW9"},
        { "User-Agent", "QtNubankAPI - www.github.com/israelins/QtNubankAPI"},
    };
    QMap<QString, QString> proxyUrls;

    bool discoverUrls() {
        static QString discoveryUrl = "https://prod-s0-webapp-proxy.nubank.com.br/api/discovery";
    }
};

QNubankAPI::QNubankAPI(QObject* parent) :
    QObject(parent),
    d_ptr(new QNubankAPIPrivate(this))
{}

void QNubankAPI::authenticate(const QNubankAPI::AuthRequest& request, std::function<void (const QNubankAPI::Response&)> onFinished)
{
    authenticate(this, request, onFinished);
}

void QNubankAPI::authenticate(QObject* context, const QNubankAPI::AuthRequest& request, std::function<void (const QNubankAPI::Response&)> onFinished)
{

}
