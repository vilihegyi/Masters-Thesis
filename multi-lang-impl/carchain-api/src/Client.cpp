#include <QDebug>
#include <QTimer>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QLoggingCategory>
#include <QVariantMap>

#include "Client.h"
#include "Daemon.h"
#include "RequestFactory.h"

static QLoggingCategory logClient("Client", QtCriticalMsg);

static const auto requestKey = QStringLiteral("request");

/**
    Create a new client instance for someone who opened the webapp.

    @param socket - Socket we open the connection between API and web page
    @param parent - Daemon responsible for the communication between the two
*/
Client::Client(QWebSocket *socket, Daemon *parent)
    : QObject(parent), mSocket(socket)
{
    mSocket->setParent(this);
    mParent = parent;

    qCDebug(logClient) << "Client created:" << socket->peerAddress()
                       << "Port" << socket->peerPort();

    connect(mSocket, &QWebSocket::disconnected, [=]() {
        qCDebug(logClient) << "Client disconnected:" << socket->peerAddress()
                           << "Port" << socket->peerPort();
        deleteLater();
    });

    connect(mSocket, &QWebSocket::textMessageReceived, this, &Client::request);
}

/**
    Debug that we aren't having a client anymore on this address and port.
*/
Client::~Client()
{
    qCDebug(logClient) << "Client destroyed.";
}

/**
    Handles a new request received from the client.

    @param dataIn - JSON string with the data we received through the socked
*/
void Client::request(const QString &dataIn)
{
    const auto document = QJsonDocument::fromJson(dataIn.toUtf8());
    if (!document.isObject()) {
        qCCritical(logClient) << "Request is not an object:" << dataIn;
        mSocket->close(QWebSocketProtocol::CloseCodeDatatypeNotSupported);
        return;
    }

    qCDebug(logClient).noquote() << "request:"
                                 << QString::fromUtf8(document.toJson());

    const auto object = document.object();
    if (!object.contains(requestKey)) {
        qCCritical(logClient) << "Object is missing request key: " << object;
        mSocket->close(QWebSocketProtocol::CloseCodeDatatypeNotSupported);
        return;
    }

    const auto value = object.value(requestKey);
    if (!value.isString()) {
        qCCritical(logClient) << "Invalid request-type:" << value;
        mSocket->close(QWebSocketProtocol::CloseCodeDatatypeNotSupported);
        return;
    }

    const auto name = value.toString();
    const auto factory = RequestFactory::instance();
    auto request = factory->createRequest(name, this);
    if (!request) {
        qCCritical(logClient) << "Unknown request:" << name;
        mSocket->close(QWebSocketProtocol::CloseCodeDatatypeNotSupported);
        return;
    }

    connect(mSocket, &QWebSocket::binaryMessageReceived, request,
            &Request::data);

    connect(request, &Request::completed, [=](const QVariantMap &result) {
        const auto object = QJsonObject::fromVariantMap(result);
        const auto document = QJsonDocument(object);
        qDebug() << "Emit completed!";
        mSocket->sendTextMessage(
            QString(document.toJson(QJsonDocument::Compact)));
        request->deleteLater();
    });

    connect(request, &Request::progress, [=](const QVariantMap &result) {
        const auto object = QJsonObject::fromVariantMap(result);
        const auto document = QJsonDocument(object);
        mSocket->sendTextMessage(
                    QString(document.toJson(QJsonDocument::Compact)));
    });

    connect(request, &Request::error, [=](const QVariantMap &result) {
        const auto object = QJsonObject::fromVariantMap(result);
        const auto document = QJsonDocument(object);
        mSocket->sendTextMessage(
            QString(document.toJson(QJsonDocument::Compact)));
        request->deleteLater();
    });

    if (!request->exec(object.toVariantMap())) {
        mSocket->close(
            QWebSocketProtocol::CloseCodeAbnormalDisconnection,
            QStringLiteral("Internal error, request-handler failed."));
    }
}

/**
    Notify the web application with the result of the request handling.

    @param result - Map to be converted to JSON to be sent back to the webpage.
*/
void Client::onNotification(const QVariantMap &result)
{
    if (mSocket) {
        const auto object = QJsonObject::fromVariantMap(result);
        const auto document = QJsonDocument(object);
        mSocket->sendTextMessage(
            QString(document.toJson(QJsonDocument::Compact)));
    }
}
