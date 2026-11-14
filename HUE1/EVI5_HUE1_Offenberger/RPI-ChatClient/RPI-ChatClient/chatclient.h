#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QWebSocket>
#include <QTimer>

/** @brief WebSocket chat client with automatic reconnection. */
class Chatclient : public QObject
{
    Q_OBJECT

public:
    /** @brief Initializes the WebSocket and reconnect timer. */
    explicit Chatclient();

    /** @brief Validates the server address and starts connecting. */
    void connectToServer(const QUrl &url);
    /** @brief Disconnects and stops further connection attempts. */
    void disconnectFromServer();
    /** @brief Sends a text message when connected. */
    void sendMessage(const QString &message);

signals:
    /** @brief Signals a successful connection. */
    void connected();
    /** @brief Provides a description of a connection error. */
    void connectionError(const QString &message);
    /** @brief Signals an explicitly requested disconnection. */
    void closed();
    /** @brief Provides a received text message. */
    void messageReceived(const QString &message);
    /** @brief Signals a scheduled reconnection attempt. */
    void reconnecting();

public slots:
    /** @brief Accepts a requested connection and stops the timer. */
    void onConnected();
    /** @brief Cancels reconnection and disconnects. */
    void onCancelReconnect();

private slots:
    /** @brief Schedules reconnection after a connection loss. */
    void onDisconnected();
    /** @brief Forwards a received message as a signal. */
    void onTextMessageReceived(const QString &message);
    /** @brief Reports socket errors and schedules reconnection. */
    void onSocketError();
    /** @brief Reconnects to the last address when the timer expires. */
    void onReconnectTimeout();

private:
    /** @brief Starts the reconnect timer if needed. */
    void scheduleReconnect();
    bool connectionRequested = false;
    QWebSocket mSocket;
    QTimer reconnectTimer;
    QUrl lastUrl;
    const std::chrono::milliseconds tReconnectTimout = std::chrono::milliseconds(500);
};

#endif // CHATCLIENT_H
