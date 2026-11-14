#include "chatclient.h"
#include <QDebug>

Chatclient::Chatclient()
{

    connect(&mSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(&mSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(&mSocket, SIGNAL(textMessageReceived(QString)), this,
            SLOT(onTextMessageReceived(QString)));
    connect(&mSocket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this,
            SLOT(onSocketError()));

    reconnectTimer.setSingleShot(true);
    connect(&reconnectTimer, SIGNAL(timeout()), this, SLOT(onReconnectTimeout()));
    qDebug() << "[Chatclient] Initialized";
}

void Chatclient::connectToServer(const QUrl &url)
{
    // Prevent the previous socket's disconnect signal from scheduling a reconnect.
    reconnectTimer.stop();
    connectionRequested = false;
    mSocket.abort();
    qDebug() << "[Chatclient] Connecting to server:" << url;

    if (!url.isValid() || url.host().isEmpty()) {
        qDebug() << "[Chatclient] Invalid WebSocket address:" << url;
        emit connectionError(tr("Ungültige WebSocket-Adresse"));
        return;
    }
    lastUrl = url;
    connectionRequested = true;
    mSocket.open(url);
}

void Chatclient::disconnectFromServer()
{
    qDebug() << "[Chatclient] Disconnect requested; cancelling automatic reconnect";
    // An explicit disconnect must also cancel automatic reconnection.
    connectionRequested = false;
    reconnectTimer.stop();
    mSocket.abort();
    emit closed();
}

void Chatclient::sendMessage(const QString &message)
{
    if (mSocket.state() == QAbstractSocket::ConnectedState) {
        const qint64 bytesSent = mSocket.sendTextMessage(message);
        qDebug() << "[Chatclient] Send result:" << bytesSent
                 << "bytes for" << message.size() << "characters";
    } else {
        qDebug() << "[Chatclient] Message not sent; socket state:" << mSocket.state();
    }
}

void Chatclient::onConnected()
{
    reconnectTimer.stop();
    qDebug() << "[Chatclient] WebSocket connected; reconnect timer stopped";
    emit connected();
}

void Chatclient::onSocketError()
{
    qDebug() << "[Chatclient] Socket error:" << mSocket.errorString()
             << "state:" << mSocket.state();
    if (!connectionRequested)
        return;
    scheduleReconnect();
    emit connectionError(mSocket.errorString());
}

void Chatclient::onDisconnected()
{
    qDebug() << "[Chatclient] WebSocket disconnected; reconnect requested:"
             << connectionRequested;
    if (connectionRequested)
        scheduleReconnect();
}

void Chatclient::scheduleReconnect()
{
    // Error and disconnect signals can both arrive; schedule only one retry.
    if (!connectionRequested || reconnectTimer.isActive())
        return;
    reconnectTimer.start(tReconnectTimout);
    qDebug() << "[Chatclient] Reconnect scheduled in" << reconnectTimer.interval() << "ms";
    emit reconnecting();
}

void Chatclient::onTextMessageReceived(const QString &message)
{
    qDebug() << "[Chatclient] WebSocket received:" << message.size() << "characters";

    emit messageReceived(message);
}

void Chatclient::onReconnectTimeout()
{
    if (!connectionRequested)
        return;
    qDebug() << "[Chatclient] Reconnecting to:" << lastUrl;
    mSocket.open(lastUrl);
}

void Chatclient::onCancelReconnect()
{
    qDebug() << "[Chatclient] Reconnect cancelled";
    disconnectFromServer();
}
