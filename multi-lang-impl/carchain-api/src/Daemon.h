#ifndef DAEMON_H // DAEMON_H
#define DAEMON_H // DAEMON_H

#include <QObject>
#include <QWebSocket>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QWebSocketServer>

#include "Client.h"

class Daemon : public QObject
{
    Q_OBJECT

public:
    explicit Daemon(QObject * = nullptr);

    int port() const;
    void setPort(int);

public slots:
    void start();

private slots:
    void clientConnected();

signals:
    void notification(const QVariantMap &);

private:
    int mPort;
    QWebSocketServer *mWebSocket;
};

#endif // DAEMON_H
