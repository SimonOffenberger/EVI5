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

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public slots:
    void btnConnectClicked();
    void btnDisconnectClicked();
    void btnSendClicked();

private:
    enum class ConnectionState { Disconnected, Connecting, Connected, Error };
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
