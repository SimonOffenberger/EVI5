# RPI-ChatClient in VS Code

Den Ordner mit dieser README und `CMakeLists.txt` direkt in VS Code öffnen.
Die Einrichtung verwendet das installierte Desktop-Qt 6.8.3 unter
`~/Qt/6.8.3/gcc_64`, GCC sowie clangd/clang-tidy 18 aus `/usr/bin`.
Qt Creator kann weiterhin für das Raspberry-Pi-Kit und das Deployment verwendet werden.
Die Editor-Konfiguration prüft gegen Desktop-Qt; zielsystemspezifische Unterschiede
müssen zusätzlich mit dem Raspberry-Pi-Kit gebaut werden.

## Start

1. Die empfohlenen Erweiterungen **clangd** und **CMake Tools** aktivieren
   (auf diesem Rechner bereits installiert).
2. Nach Änderungen an den Einstellungen einmal **Developer: Reload Window** ausführen.
3. Den automatischen Task **Qt: Prepare editor** beim Öffnen zulassen oder über
   **Tasks: Run Task** manuell ausführen. Er erzeugt die Compiler-Datenbank und
   die Qt-MOC-/UI-Header. Die erste Vorbereitung wurde bereits ausgeführt.
4. Bei veralteten Meldungen **clangd: Restart language server** ausführen.

Nach Änderungen an `.ui`-Dateien, neuen Quelldateien oder CMake-Abhängigkeiten
**Qt: Prepare editor** erneut starten. Neue Quelldateien müssen wie üblich
in `CMakeLists.txt` eingetragen werden. Bei der CMake-Preset-Auswahl `vscode` wählen.

Die gleichen Schritte im Terminal:

```sh
cmake --preset vscode
cmake --build --preset vscode-autogen
```

`build/vscode/compile_commands.json` enthält die tatsächlichen Qt-Include-Pfade,
Defines und Compiler-Optionen. `.clangd` verweist auf dieses Verzeichnis.
Der Qt-Creator-Build liegt separat. Für eine andere lokale Qt-Version den
`CMAKE_PREFIX_PATH` in `CMakePresets.json` anpassen.

## Diagnose und Stil

- **clangd** liefert Vervollständigung, Navigation und Live-Diagnosen einschließlich
  der Prüfungen aus `.clang-tidy`.
- **clang-tidy** prüft unter anderem Fehlerquellen, Performance, `nullptr`,
  `override` und Qt-übliche Namen: `CamelCase` für Typen, `camelBack` für
  Funktionen, Parameter und Variablen. Für vollständige Prüfungen einer Datei
  gibt es den Task **Qt: clang-tidy current file** (mit geöffneter `.cpp`-Datei).
  Der Headerfilter umfasst Projekt-Header im Hauptordner sowie in `src/` und
  `include/`. Bei weiteren Header-Verzeichnissen oder Umbenennung des Projektordners
  `HeaderFilterRegex` in `.clang-tidy` anpassen; generierte Header bleiben ausgespart.
- **clang-format** wird über clangd beim Speichern angewendet: vier Leerzeichen,
  Qt-nahe Klammern, Zeilenumbrüche und Zeigerformatierung. Regeln stehen in
  `.clang-format`; Formatierung ist keine clang-tidy-Diagnose.
- Die Microsoft-C++-IntelliSense-Engine und die separate CS128-clang-tidy-Erweiterung
  sind für diesen Workspace als Diagnosequellen deaktiviert, damit sie keine
  doppelten oder falsch konfigurierten Meldungen liefern. clang-tidy läuft in clangd
  und über den expliziten Task.

Die Regeln orientieren sich am [Qt Coding Style](https://wiki.qt.io/Qt_Coding_Style).
Nicht jede Qt-Konvention lässt sich automatisch prüfen oder formatieren.
Für Qt-spezifischen API-Gebrauch (etwa Signals/Slots) bietet Qt Creator zusätzlich
[Clazy](https://doc.qt.io/qtcreator/creator-clang-tools.html); das ist ein eigenes
Werkzeug und nicht Teil von clang-tidy.

Optionaler Desktop-Build: **Ctrl+Shift+B** oder `cmake --build --preset vscode`.
Ein Flashen oder Deployment wird dadurch nicht ausgelöst.
