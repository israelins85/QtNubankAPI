#ifndef QNUBANKAPI_H
#define QNUBANKAPI_H

#include <QObject>
#include <QString>

#include <QJsonDocument>

class QNubankAPIPrivate;
class QNubankAPI : public QObject
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(QNubankAPI)
  QScopedPointer<QNubankAPIPrivate> const d_ptr;

public:
    explicit QNubankAPI(QObject* parent);

    struct Response {
        bool sucess;
        QJsonValue data;
    };

    struct AuthRequest {
        QString cpf;
        QString password;

        AuthRequest()
        {}

        AuthRequest(const QString& cpf = QString(), const QString& password = QString()) :
            cpf(cpf), password(password)
        {}
    };

    void authenticate(const QNubankAPI::AuthRequest& request, std::function<void (const QNubankAPI::Response&)> onFinished);
    void authenticate(QObject* context, const QNubankAPI::AuthRequest& request, std::function<void (const QNubankAPI::Response&)> onFinished);

    void setAccessToken(const QString& accessToken);

    void cardFeed(std::function<void (const QNubankAPI::Response&)> a_onFinished);
    void cardFeed(QObject* context, std::function<void (const QNubankAPI::Response&)> a_onFinished);

    void bills(std::function<void (const QNubankAPI::Response&)> a_onFinished);
    void bills(QObject* context, std::function<void (const QNubankAPI::Response&)> a_onFinished);

    void billDetails(QJsonValue bill, std::function<void (const QNubankAPI::Response&)> a_onFinished);
    void billDetails(QObject* context, QJsonValue bill, std::function<void (const QNubankAPI::Response&)> a_onFinished);

    void accountFeed(std::function<void (const QNubankAPI::Response&)> a_onFinished);
    void accountFeed(QObject* context, std::function<void (const QNubankAPI::Response&)> a_onFinished);

    void accountBalance(std::function<void (const QNubankAPI::Response&)> a_onFinished);
    void accountBalance(QObject* context, std::function<void (const QNubankAPI::Response&)> a_onFinished);
};

#endif // QNUBANKAPI_H
