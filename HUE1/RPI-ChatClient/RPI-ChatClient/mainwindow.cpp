#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mainBoxLayout = new QVBoxLayout;
    mainWidget = new QWidget;
    mainWidget->setLayout(mainBoxLayout);
    setCentralWidget(mainWidget);

    /* Top Server and Connect Bar */
    topVBoxLayout = new QVBoxLayout;
    topConnectionBarLayout = new QHBoxLayout;
    topUsernameConfLayout = new QHBoxLayout;

    serverLabel = new QLabel("Server:");
    serverTextBox = new QLineEdit();
    serverTextBox->setText("localhost:1234");
    connectButton = new QPushButton("verbinden");
    disconnectButton = new QPushButton("trennen");

    topConnectionBarLayout->addWidget(serverLabel);
    topConnectionBarLayout->addWidget(serverTextBox);
    topConnectionBarLayout->addWidget(connectButton);
    topConnectionBarLayout->addWidget(disconnectButton);

    userNameLabel = new QLabel("Benutzername");
    userName = new QLineEdit;
    userName->setText("Simon");

    topUsernameConfLayout->addWidget(userNameLabel);
    topUsernameConfLayout->addWidget(userName);

    topVBoxLayout->addLayout(topConnectionBarLayout);
    topVBoxLayout->addLayout(topUsernameConfLayout);

    mainBoxLayout->addLayout(topVBoxLayout);

    /* Chat Box */
    chatHistory = new QPlainTextEdit;
    chatHistory->setReadOnly(true);
    mainBoxLayout->addWidget(chatHistory);

    /* Text Message and User Name */
    bottomSendLayout = new QHBoxLayout;
    msgToSend = new QLineEdit;
    msgToSend->setReadOnly(false);
    sendButton = new QPushButton("senden");

    bottomSendLayout->addWidget(msgToSend);
    bottomSendLayout->addWidget(sendButton);

    mainBoxLayout->addLayout(bottomSendLayout);

    /* Connect signal and slots */
    connect(connectButton, SIGNAL(clicked()), this, SLOT(btnConnectClicked()));
    connect(disconnectButton, SIGNAL(clicked()), this, SLOT(btnDisconnectClicked()));
    connect(sendButton, SIGNAL(clicked()), this, SLOT(btnSendClicked()));
    connect(msgToSend, SIGNAL(returnPressed()), this, SLOT(btnSendClicked()));

    connect(&chatClient, SIGNAL(messageReceived(QString)), this, SLOT(onMsgRcv(QString)));

    connect(&chatClient, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(&chatClient, SIGNAL(closed()), this, SLOT(onClosed()));
    connect(&chatClient, SIGNAL(connectionError(QString)), this, SLOT(onConnectionError(QString)));

    connect(&chatClient, SIGNAL(reconnecting()), this, SLOT(onReconnecting()));

    updateConnectionState(ConnectionState::Disconnected);
}

void MainWindow::onConnected()
{
    updateConnectionState(ConnectionState::Connected);
}

void MainWindow::onClosed()
{
    updateConnectionState(ConnectionState::Disconnected);
}

void MainWindow::onReconnecting()
{
    updateConnectionState(ConnectionState::Reconnecting);
}

void MainWindow::onConnectionError(const QString &message)
{
    if (connectionState != ConnectionState::Reconnecting)
        updateConnectionState(ConnectionState::Error);
    connectButton->setToolTip(message);
}

void MainWindow::btnConnectClicked()
{
    QString address = serverTextBox->text();
    address.prepend("ws://");

    chatClient.connectToServer(QUrl(address));
    chatHistory->clear();
}

void MainWindow::btnDisconnectClicked()
{
    updateConnectionState(ConnectionState::Disconnected);
    chatClient.disconnectFromServer();
}

void MainWindow::updateConnectionState(ConnectionState state)
{
    connectionState = state;
    const bool connected = state == ConnectionState::Connected;
    const bool active = connected || state == ConnectionState::Reconnecting;

    userName->setEnabled(connected);
    userNameLabel->setEnabled(connected);
    msgToSend->setEnabled(connected);
    sendButton->setEnabled(connected);
    chatHistory->setEnabled(connected);
    disconnectButton->setEnabled(active);
    serverTextBox->setEnabled(!active);
    connectButton->setEnabled(!active);
    connectButton->setToolTip(QString());

    if (connected) {
        connectButton->setText(tr("verbunden"));
        connectButton->setStyleSheet("background-color: #238636; color: white;");
    } else if (state == ConnectionState::Error) {
        connectButton->setText(tr("erneut verbinden"));
        connectButton->setStyleSheet("background-color: #c62828; color: white;");
    } else if (state == ConnectionState::Reconnecting) {
        connectButton->setText(tr("reconnect"));
        connectButton->setStyleSheet("background-color: #e4e120; color: black;");
        connectButton->setEnabled(false);
    } else {
        connectButton->setText("verbinden");
        connectButton->setStyleSheet(QString());
    }
}

void MainWindow::btnSendClicked()
{
    QString replacedMsg(msgToSend->text());

    EmojiReplacer::replaceToEmojis(replacedMsg);

    QString msg(userName->text() + ": " + replacedMsg);
    chatClient.sendMessage(msg);
    chatHistory->appendPlainText(msg);
    msgToSend->clear();
}

void MainWindow::onMsgRcv(QString msg)
{
    EmojiReplacer::replaceToEmojis(msg);
    chatHistory->appendPlainText(msg);
}

MainWindow::~MainWindow()
{
    delete ui;
}
