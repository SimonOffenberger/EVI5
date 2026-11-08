#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QWebSocket>
#include <QTimer>

class Chatclient : public QObject
{
    Q_OBJECT

public:
    explicit Chatclient(QObject *parent = nullptr);

    void connectToServer(const QUrl &url);
    void disconnectFromServer();
    void sendMessage(const QString &message);

signals:
    void connected();
    void connectionError(const QString &message);
    void closed();
    void messageReceived(const QString &message);
    void reconnecting();

public slots:
    void onConnected();
    void onCancelReconnect();

private slots:
    void onDisconnected();
    void onTextMessageReceived(const QString &message);
    void onSocketError();
    void onReconnectTimeout();

private:
    void scheduleReconnect();
    bool connectionRequested = false;
    QWebSocket mSocket;
    QTimer reconnectTimer;
    QUrl lastUrl;
    const std::chrono::milliseconds tReconnectTimout = std::chrono::milliseconds(500);
};

#endif // CHATCLIENT_H
