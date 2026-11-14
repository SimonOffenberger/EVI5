# Klassendiagramm des RPI-ChatClients

Das Diagramm basiert auf den Klassen in `mainwindow.h`, `chatclient.h` und
`emojireplacer.h`. Einzelne UI-Widgets und Layouts sind zur besseren Übersicht
ausgelassen. Qt-Klassen sind ohne ihre eigenen Methoden dargestellt.

```mermaid
classDiagram
    class QMainWindow {
        <<Qt>>
    }
    class QObject {
        <<Qt>>
    }
    class QWebSocket {
        <<Qt>>
    }
    class QTimer {
        <<Qt>>
    }
    class MainWindow {
        -ConnectionState connectionState
        -Ui_MainWindow* ui
        -Chatclient chatClient
        +MainWindow(QWidget* parent)
        +~MainWindow()
        +btnConnectClicked() void
        +btnDisconnectClicked() void
        +btnSendClicked() void
        -onConnected() void
        -onReconnecting() void
        -onClosed() void
        -onConnectionError(const QString& message) void
        -onMsgRcv(QString msg) void
        -updateConnectionState(ConnectionState state) void
    }
    class ConnectionState {
        <<enumeration>>
        Disconnected
        Connected
        Error
        Reconnecting
    }
    class Ui_MainWindow {
        <<generated>>
    }
    class Chatclient {
        -bool connectionRequested
        -QWebSocket mSocket
        -QTimer reconnectTimer
        -QUrl lastUrl
        -const std_chrono_milliseconds tReconnectTimout
        +Chatclient(QObject* parent)
        +connectToServer(const QUrl& url) void
        +disconnectFromServer() void
        +sendMessage(const QString& message) void
        +connected() void
        +connectionError(const QString& message) void
        +closed() void
        +messageReceived(const QString& message) void
        +reconnecting() void
        +onConnected() void
        +onCancelReconnect() void
        -onDisconnected() void
        -onTextMessageReceived(const QString& message) void
        -onSocketError() void
        -onReconnectTimeout() void
        -scheduleReconnect() void
    }
    class EmojiReplacer {
        -QMap~QString,QString~ replacements$
        +EmojiReplacer()
        +replaceToEmojis(QString& inplaceString) void$
    }

    QMainWindow <|-- MainWindow
    QObject <|-- Chatclient
    MainWindow "1" *-- "1" Chatclient : chatClient
    MainWindow "1" *-- "1" Ui_MainWindow : ui
    MainWindow ..> ConnectionState : interner Zustandstyp
    MainWindow ..> EmojiReplacer : ersetzt Emoticons
    Chatclient "1" *-- "1" QWebSocket : mSocket
    Chatclient "1" *-- "1" QTimer : reconnectTimer
```

`+` kennzeichnet öffentliche und `-` private Elemente. Unterstrichene Elemente
sind statisch. Ein ausgefüllter Diamant steht für Komposition (Besitz),
ein Dreieck für Vererbung und ein gestrichelter Pfeil für eine Abhängigkeit.
`Ui_MainWindow` bezeichnet die generierte Klasse `Ui::MainWindow`;
`std_chrono_milliseconds` steht für `std::chrono::milliseconds`.
`ConnectionState` ist ein privater Enum-Typ von `MainWindow`.
Die konstante, statische Ersetzungstabelle gehört zu `EmojiReplacer`.

## Qt-Signale und Slots

Die folgenden Signale von `Chatclient` sind mit Slots von `MainWindow` verbunden:

| Signal von Chatclient | Slot von MainWindow |
| --- | --- |
| `connected()` | `onConnected()` |
| `closed()` | `onClosed()` |
| `connectionError(const QString&)` | `onConnectionError(const QString&)` |
| `messageReceived(const QString&)` | `onMsgRcv(QString)` |
| `reconnecting()` | `onReconnecting()` |

Die drei öffentlichen `btn…Clicked()`-Methoden von `MainWindow` sind Slots.
Seine privaten `on…`-Methoden sind ebenfalls Slots. Bei `Chatclient` sind
`onConnected()` und `onCancelReconnect()` öffentliche Slots, die übrigen
`on…`-Methoden private Slots. Der WebSocket meldet Verbindung, Trennung,
Empfang und Fehler an diese Slots; der Timer ruft bei Ablauf
`onReconnectTimeout()` auf. Die Reconnect-Verzögerung beträgt 500 ms.
