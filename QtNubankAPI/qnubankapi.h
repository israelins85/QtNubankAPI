#ifndef QNUBANKAPI_H
#define QNUBANKAPI_H

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <QJsonDocument>

class QNubankAPIPrivate;
class QNubankAPI : public QObject
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(QNubankAPI)
  QNubankAPIPrivate* const d_ptr;

public:
    explicit QNubankAPI(QObject* parent);
    virtual ~QNubankAPI();

    struct Response {
        bool sucess = false;
        QVariantMap data;

        Response()
        {}

        Response(bool sucess, QVariantMap data) :
            sucess(sucess), data(data)
        {}
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

    void cardFeed(std::function<void (const QNubankAPI::Response&)> onFinished);
    void cardFeed(QObject* context, std::function<void (const QNubankAPI::Response&)> onFinished);

    void bills(std::function<void (const QNubankAPI::Response&)> onFinished);
    void bills(QObject* context, std::function<void (const QNubankAPI::Response&)> onFinished);

    void billDetails(QJsonValue bill, std::function<void (const QNubankAPI::Response&)> onFinished);
    void billDetails(QObject* context, QJsonValue bill, std::function<void (const QNubankAPI::Response&)> onFinished);

    void accountFeed(std::function<void (const QNubankAPI::Response&)> onFinished);
    void accountFeed(QObject* context, std::function<void (const QNubankAPI::Response&)> onFinished);

    void accountBalance(std::function<void (const QNubankAPI::Response&)> onFinished);
    void accountBalance(QObject* context, std::function<void (const QNubankAPI::Response&)> onFinished);
};

#endif // QNUBANKAPI_H
