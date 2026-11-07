#include "chatclient.h"
#include <QDebug>

Chatclient::Chatclient(QObject *parent) : QObject(parent)
{

    connect(&mSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(&mSocket, SIGNAL(disconnected()), this, SIGNAL(closed()));
    connect(&mSocket, SIGNAL(textMessageReceived(QString)), this,
            SLOT(onTextMessageReceived(QString)));
    connect(&mSocket, &QWebSocket::errorOccurred, this,
            [this](QAbstractSocket::SocketError) {
                emit connectionError(mSocket.errorString());
            });
}

void Chatclient::connectToServer(const QUrl &url)
{
    qDebug() << "WebSocket server:" << url;

    if (!url.isValid() || url.host().isEmpty()
        || (url.scheme() != "ws" && url.scheme() != "wss")) {
        emit connectionError(tr("Ungültige WebSocket-Adresse"));
        return;
    }
    mSocket.open(url);
}

void Chatclient::disconnectFromServer()
{
    mSocket.close();
}

void Chatclient::sendMessage(const QString &message)
{
    if (mSocket.state() == QAbstractSocket::ConnectedState)
        mSocket.sendTextMessage(message);
}

void Chatclient::onConnected()
{
    qDebug() << "WebSocket connected";
    emit connected();
}

void Chatclient::onTextMessageReceived(const QString &message)
{
    qDebug() << "WebSocket received:" << message;

    // emit calls every connected Slot.
    // like an Obeserver
    emit messageReceived(message);
}
