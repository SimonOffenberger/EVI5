#include "chatclient.h"
#include <QDebug>

Chatclient::Chatclient(QObject *parent) : QObject(parent)
{

    connect(&mSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(&mSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(&mSocket, SIGNAL(textMessageReceived(QString)), this,
            SLOT(onTextMessageReceived(QString)));
    connect(&mSocket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this,
            SLOT(onSocketError()));

    reconnectTimer.setSingleShot(true);
    connect(&reconnectTimer, SIGNAL(timeout()), this, SLOT(onReconnectTimeout()));
}

void Chatclient::connectToServer(const QUrl &url)
{
    reconnectTimer.stop();
    connectionRequested = false;
    mSocket.abort();
    qDebug() << "WebSocket server:" << url;

    if (!url.isValid() || url.host().isEmpty() || (url.scheme() != "ws" && url.scheme() != "wss")) {
        emit connectionError(tr("Ungültige WebSocket-Adresse"));
        return;
    }
    lastUrl = url;
    connectionRequested = true;
    mSocket.open(url);
}

void Chatclient::disconnectFromServer()
{
    connectionRequested = false;
    reconnectTimer.stop();
    mSocket.abort();
    emit closed();
}

void Chatclient::sendMessage(const QString &message)
{
    if (mSocket.state() == QAbstractSocket::ConnectedState)
        mSocket.sendTextMessage(message);
}

void Chatclient::onConnected()
{
    if (!connectionRequested) {
        mSocket.abort();
        return;
    }
    reconnectTimer.stop();
    qDebug() << "WebSocket connected";
    emit connected();
}

void Chatclient::onSocketError()
{
    if (!connectionRequested)
        return;
    scheduleReconnect();
    emit connectionError(mSocket.errorString());
}

void Chatclient::onDisconnected()
{
    if (connectionRequested)
        scheduleReconnect();
}

void Chatclient::scheduleReconnect()
{
    if (!connectionRequested || reconnectTimer.isActive())
        return;
    reconnectTimer.start(tReconnectTimout);
    emit reconnecting();
}

void Chatclient::onTextMessageReceived(const QString &message)
{
    qDebug() << "WebSocket received:" << message;

    // emit calls every connected Slot.
    // like an Obeserver
    emit messageReceived(message);
}

void Chatclient::onReconnectTimeout()
{
    if (!connectionRequested)
        return;
    qDebug() << "TryReconection";
    mSocket.open(lastUrl);
}

void Chatclient::onCancelReconnect()
{
    disconnectFromServer();
}
