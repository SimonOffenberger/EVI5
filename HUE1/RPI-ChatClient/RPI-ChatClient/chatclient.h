#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QWebSocket>

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

public slots:

    void onConnected();

private slots:
    void onTextMessageReceived(const QString &message);

private:
    QWebSocket mSocket;
};

#endif // CHATCLIENT_H
