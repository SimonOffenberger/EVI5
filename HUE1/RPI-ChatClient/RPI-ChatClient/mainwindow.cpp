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
    connect(msgToSend, &QLineEdit::returnPressed, this, &MainWindow::btnSendClicked);

    connect(&chatClient, SIGNAL(messageReceived(QString)), chatHistory,
            SLOT(appendPlainText(QString)));

    connect(&chatClient, &Chatclient::connected, this,
            [this] { updateConnectionState(ConnectionState::Connected); });
    connect(&chatClient, &Chatclient::closed, this, [this] {
        // A socket error may be followed by closed(); keep the error visible.
        if (connectionState != ConnectionState::Error)
            updateConnectionState(ConnectionState::Disconnected);
    });
    connect(&chatClient, &Chatclient::connectionError, this, [this](const QString &message) {
        updateConnectionState(ConnectionState::Error);
        connectButton->setToolTip(message);
    });

    updateConnectionState(ConnectionState::Disconnected);
}

void MainWindow::btnConnectClicked()
{
    QString address = serverTextBox->text().trimmed();
    if (!address.contains("://"))
        address.prepend("ws://");

    updateConnectionState(ConnectionState::Connecting);
    chatClient.connectToServer(QUrl(address));
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
    const bool connecting = state == ConnectionState::Connecting;

    userName->setEnabled(connected);
    userNameLabel->setEnabled(connected);
    msgToSend->setEnabled(connected);
    sendButton->setEnabled(connected);
    chatHistory->setEnabled(connected);
    disconnectButton->setEnabled(connected);
    serverTextBox->setEnabled(!connected && !connecting);
    connectButton->setEnabled(!connected && !connecting);
    connectButton->setToolTip(QString());

    if (connected) {
        connectButton->setText(tr("verbunden"));
        connectButton->setStyleSheet("background-color: #238636; color: white;");
    } else if (state == ConnectionState::Error) {
        connectButton->setText(tr("erneut verbinden"));
        connectButton->setStyleSheet("background-color: #c62828; color: white;");
    } else {
        connectButton->setText(connecting ? tr("verbinde …") : tr("verbinden"));
        connectButton->setStyleSheet(QString());
    }
}

void MainWindow::btnSendClicked()
{
    QString msg(userName->text() + ": " + msgToSend->text());
    chatClient.sendMessage(msg);
    chatHistory->appendPlainText(msg);
    msgToSend->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}
