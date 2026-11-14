#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include "chatclient.h"
#include "emojireplacer.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/** @brief Chat interface for managing connections and displaying messages. */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /** @brief Creates the interface and connects its signals and slots. */
    explicit MainWindow(QWidget *parent = nullptr);
    /** @brief Releases the UI helper object. */
    ~MainWindow() override;

public slots:
    /** @brief Connects to the entered address and clears the chat history. */
    void btnConnectClicked();
    /** @brief Disconnects and updates the interface. */
    void btnDisconnectClicked();
    /** @brief Sends and displays the text with emojis and the username. */
    void btnSendClicked();

private slots:
    /** @brief Updates the interface after a successful connection. */
    void onConnected();
    /** @brief Displays the ongoing reconnection attempt. */
    void onReconnecting();
    /** @brief Sets the interface to the disconnected state. */
    void onClosed();
    /** @brief Displays a connection error as a tooltip. */
    void onConnectionError(const QString &message);
    /** @brief Replaces emoticons and displays the received message. */
    void onMsgRcv(QString msg);

private:
    /** @brief Possible connection states of the interface. */
    enum class ConnectionState { Disconnected, Connected, Error, Reconnecting };
    /** @brief Updates controls and the connection indicator for the given state. */
    void updateConnectionState(ConnectionState state);
    ConnectionState connectionState = ConnectionState::Disconnected;

    Ui::MainWindow *ui;

    QVBoxLayout *mainBoxLayout = nullptr;
    QWidget *mainWidget = nullptr;

    QVBoxLayout *topVBoxLayout = nullptr;
    QHBoxLayout *topConnectionBarLayout = nullptr;
    QHBoxLayout *topUsernameConfLayout = nullptr;
    QLabel *serverLabel = nullptr;
    QLineEdit *serverTextBox = nullptr;
    QPushButton *connectButton = nullptr;
    QPushButton *disconnectButton = nullptr;
    QLabel *userNameLabel = nullptr;
    QLineEdit *userName = nullptr;

    QPlainTextEdit *chatHistory = nullptr;

    QHBoxLayout *bottomSendLayout = nullptr;
    QLineEdit *msgToSend = nullptr;
    QPushButton *sendButton = nullptr;

    Chatclient chatClient;
};
#endif // MAINWINDOW_H
