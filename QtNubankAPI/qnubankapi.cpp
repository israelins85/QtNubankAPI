#include "qnubankapi.h"

#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>
#include <QDebug>

class QNubankAPIPrivate {
    Q_DISABLE_COPY(QNubankAPIPrivate)
    Q_DECLARE_PUBLIC(QNubankAPI)
    QNubankAPI* const q_ptr;

    QNetworkAccessManager* networkAccessManager;

    const QString discoveryUrl = "https://prod-s0-webapp-proxy.nubank.com.br/api/discovery";
    QVariantMap headers = {
        { "Content-Type", "application/json" },
        { "X-Correlation-Id", "WEB-APP.pewW9" },
        { "User-Agent", "QtNubankAPI - www.github.com/israelins/QtNubankAPI" },
    };

    enum class UrlType {
        Discovery,
        Authenticate,
        CardFeed
    };

    QMap<UrlType, QString> urls {
        { UrlType::Discovery, "https://prod-s0-webapp-proxy.nubank.com.br/api/discovery" }
    };

    QNubankAPIPrivate(QNubankAPI* q) : q_ptr(q) {
        networkAccessManager = new QNetworkAccessManager(q);
    }

    void asyncRequest(UrlType urlType, const QByteArray &verb, QObject* context, std::function<void (const QNubankAPI::Response&)> onFinished, const QVariantMap& body) {
        QString l_url = urls[urlType];

        if (!l_url.isEmpty()) {
            asyncRequest(l_url, verb, context, onFinished, body);
            return;
        }

        if (urlType == UrlType::Authenticate) {
            asyncRequest(urls[UrlType::Discovery], "GET", context, [this, verb, context, onFinished, body](const QNubankAPI::Response& resp) {
                if (!resp.sucess) {
                    onFinished(resp);
                    return;
                }
                urls[UrlType::Authenticate] = resp.data["login"].toString();
                asyncRequest(urls[UrlType::Authenticate], verb, context, onFinished, body);
            }, QVariantMap());

            return;
        }

        QTimer::singleShot(0, context, [onFinished]() {
            QNubankAPI::Response resp(false, { { "message", "Invalid Url" } });
            onFinished(resp);
        });
    }

    void dumpRequest(QNetworkRequest* a_request) {
        qDebug() << "dumpRequest" << a_request->url().toString();
        return;
        for (QByteArray l_name : a_request->rawHeaderList()) {
            qDebug() << l_name << a_request->rawHeader(l_name);
        }
    }

    void dumpReply(QNetworkReply* a_reply) {
        if (a_reply->error() != QNetworkReply::NoError)
            qDebug() << "dumpReply" << a_reply->error() << a_reply->errorString();
        qDebug() << "httpStatusCode" << a_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    //    return;
        for (QByteArray l_name : a_reply->rawHeaderList()) {
            qDebug() << l_name << a_reply->rawHeader(l_name);
        }
    //    qDebug() << PaJson::beautify(PaJson::toString(l_res));
    }

    QNetworkRequest mountRequest(const QString& url) {
        QNetworkRequest request(url);

        QVariantMap::ConstIterator i = headers.begin();
        QVariantMap::ConstIterator e = headers.end();
        for (; i != e; ++i) {
            request.setRawHeader(i.key().toUtf8(), i.value().toString().toUtf8());
        }

        dumpRequest(&request);

        return request;
    }

    QNubankAPI::Response responseFromReply(QNetworkReply* reply) {
        static QMap<QNetworkReply::NetworkError, QString> sl_errStr;
        QNubankAPI::Response response;
        QVariantMap l_httpHeader;
        for (QNetworkReply::RawHeaderPair l_rawHeaderPair: reply->rawHeaderPairs()) {
            l_httpHeader[l_rawHeaderPair.first] = QString::fromUtf8(l_rawHeaderPair.second);
        }
        response.data["httpHeader"] = l_httpHeader;

        QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
        qDebug() << json.toJson();

        response.data = json.toVariant().toMap();

        response.data["httpStatusCode"] = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        response.sucess = (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() / 100 == 2);

        if (!response.sucess || (qint32(response.data["httpStatusCode"].toInt() / 100) != 2) || reply->error()) {
            if (sl_errStr.isEmpty()) {
                sl_errStr.insert(QNetworkReply::ConnectionRefusedError, ("Servidor não está aceitando conexões, "
                                                                         "verifique a conectividade com a internet e "
                                                                         "tente novamente."));
                sl_errStr.insert(QNetworkReply::RemoteHostClosedError, ("Conexão foi abortada pelo servidor"));
                sl_errStr.insert(QNetworkReply::HostNotFoundError, ("Servidor não foi encontrado"));
                sl_errStr.insert(QNetworkReply::TimeoutError, ("Servidor não respondeu em tempo hábil"));
                sl_errStr.insert(QNetworkReply::OperationCanceledError, ("Conexão foi abortada: Servidor não respondeu em tempo hábil"));
                sl_errStr.insert(QNetworkReply::SslHandshakeFailedError, ("Configuração de SSL não aceita"));
                sl_errStr.insert(QNetworkReply::TemporaryNetworkFailureError, ("O servidor está temporáriamente indisponível"));
                sl_errStr.insert(QNetworkReply::ProxyConnectionRefusedError, ("Configuração de proxy inválida"));
                sl_errStr.insert(QNetworkReply::ProxyConnectionClosedError, ("Configuração de proxy inválida"));
                sl_errStr.insert(QNetworkReply::ProxyNotFoundError, ("Configuração de proxy inválida"));
                sl_errStr.insert(QNetworkReply::ProxyTimeoutError, ("Configuração de proxy inválida"));
                sl_errStr.insert(QNetworkReply::ProxyAuthenticationRequiredError, ("Configuração de proxy inválida"));
                sl_errStr.insert(QNetworkReply::ContentAccessDenied, ("Acesso negado"));
                sl_errStr.insert(QNetworkReply::ContentOperationNotPermittedError, ("Operação não permitida"));
                sl_errStr.insert(QNetworkReply::ContentNotFoundError, ("Operação não existe"));
                sl_errStr.insert(QNetworkReply::AuthenticationRequiredError, ("Autenticação requerida"));
                sl_errStr.insert(QNetworkReply::ContentReSendError, ("Dados de envio não puderam ser lidos pela segunda vez"));
                sl_errStr.insert(QNetworkReply::ProtocolUnknownError, ("Protocolo desconhecido"));
                sl_errStr.insert(QNetworkReply::ProtocolInvalidOperationError, ("Operação inválida para esse protocolo"));
                sl_errStr.insert(QNetworkReply::UnknownNetworkError, ("desconhecido"));
                sl_errStr.insert(QNetworkReply::UnknownProxyError, ("proxy"));
                sl_errStr.insert(QNetworkReply::UnknownContentError, ("Recebendo dados do servidor"));
                sl_errStr.insert(QNetworkReply::ProtocolFailure, ("Protocolo inválido"));
            }

            if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() != 200)
                response.data["httpReasonPhase"] = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
            if (reply->error())
                response.data["networkErrorText"] = sl_errStr.value(reply->error());
            response.data["networkError"] = true;
        }

        return response;
    }

    void asyncRequest(const QString& url, const QByteArray &verb, QObject* context, std::function<void (const QNubankAPI::Response&)> onFinished, const QVariantMap& body) {
        QNetworkRequest request = mountRequest(url);
        QByteArray data;
        if (!body.isEmpty()) {
            data = QJsonDocument(QJsonObject::fromVariantMap(body)).toJson(QJsonDocument::Compact);
        }
        QNetworkReply* reply = networkAccessManager->sendCustomRequest(request, verb, data);
        reply->connect(reply, &QNetworkReply::finished, context, [this, reply, onFinished]() {
            dumpReply(reply);
            QNubankAPI::Response response = responseFromReply(reply);
            reply->deleteLater();
            onFinished(response);
        });
    }
};

QNubankAPI::QNubankAPI(QObject* parent) :
    QObject(parent),
    d_ptr(new QNubankAPIPrivate(this))
{}

QNubankAPI::~QNubankAPI()
{
    delete d_ptr;
}

void QNubankAPI::authenticate(const QNubankAPI::AuthRequest& request, std::function<void (const QNubankAPI::Response&)> onFinished)
{
    authenticate(this, request, onFinished);
}

void QNubankAPI::authenticate(QObject* context, const QNubankAPI::AuthRequest& request, std::function<void (const QNubankAPI::Response&)> onFinished)
{
    Q_D(QNubankAPI);

    d->asyncRequest(QNubankAPIPrivate::UrlType::Authenticate, "POST", context, onFinished, {
        { "grant_type", "password" },
        { "login", request.cpf },
        { "password", request.password },
        { "client_id", "other.conta" },
        { "client_secret", "yQPeLzoHuJzlMMSAjC-LgNUJdUecx8XO" }
    });
}

void QNubankAPI::cardFeed(std::function<void (const QNubankAPI::Response&)> onFinished)
{
    cardFeed(this, onFinished);
}

void QNubankAPI::cardFeed(QObject* context, std::function<void (const QNubankAPI::Response&)> onFinished)
{
    Q_D(QNubankAPI);

    d->asyncRequest(QNubankAPIPrivate::UrlType::CardFeed, "GET", context, onFinished, QVariantMap());
}
