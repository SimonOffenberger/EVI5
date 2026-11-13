# Bilder zur Ausarbeitung

Die folgenden Dateien hier ablegen; die Vorlage bindet sie automatisch ein.
Solange eine Datei fehlt, wird ein beschrifteter Platzhalter angezeigt.

- `handskizze.png`: eigene Handskizze mit Widgets und Layouts
- `raspberry-pi.png`: Screenshot oder Foto der laufenden Raspberry-Pi-Anwendung
- `desktop-klein.png`: Desktop-Anwendung im kleinen Fenster
- `desktop-gross.png`: Desktop-Anwendung im größeren Fenster
- `idee-reconnect.png`: sichtbarer Wiederverbindungszustand
- `idee-emojis.png`: Chat mit Emoji-Ersetzung

Für JPG oder PDF den Dateinamen im jeweiligen `\bild`-Aufruf in
`../loesung.tex` anpassen. Bildunterschriften dort ebenfalls konkretisieren.

## Erstellte Desktop-Aufnahmen

Die vier Desktop-PNGs wurden aus der laufenden Qt-6.8.3-Anwendung unter
Zorin OS 18.1 direkt mit `QWidget::grab()` aufgenommen (Fensterinhalt ohne
Fensterrahmen). `desktop-klein.png`, `idee-emojis.png` und
`idee-reconnect.png` haben 800 × 480 Pixel, `desktop-gross.png` 1200 × 720.

Eine temporäre Qt-Teststeuerung wurde zusätzlich zum unveränderten Projekt
gebaut. Sie stellte den Fenstermodus und die Größen ein, bediente die
Schaltflächen und tauschte Nachrichten mit einem zweiten WebSocket-Client
über die vorhandene Simplechat-Serverimplementierung auf localhost:18473 aus.
Der Server wurde für den Reconnect-Nachweis tatsächlich gestoppt.
Geprüft wurden außerdem die automatische Wiederverbindung nach Serverstart,
der manuelle Abbruch und alle vier Emoji-Ersetzungen beim Senden und Empfangen.
Client und Testserver wurden nach den Aufnahmen beendet.

Noch selbst ergänzen: `handskizze.png` und `raspberry-pi.png`.
