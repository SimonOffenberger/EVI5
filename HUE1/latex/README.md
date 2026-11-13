# LaTeX-Vorlage zur Übung 1

## Bearbeiten

- `main.tex`: Name, Matrikelnummer, Studiengang und Abgabedatum
- `loesung.tex`: Ausarbeitung; rote Ergänzungsmarkierungen bearbeiten
- `bilder/`: Handskizze, Fotos und Screenshots (siehe dortige README)
- `quellcode.tex`: direkte Einbindung aller sieben C++-Header/Quelldateien,
  `CMakeLists.txt`, `CMakePresets.json` und `mainwindow.ui`
- `syntax.tex`: Syntaxhighlighting mit `listings`, Zeilennummern und Zeilenumbruch

Die komplette zweiseitige Übungsangabe wird als Deckblatt vorangestellt.
Die Beschreibungen der Wiederverbindung und Emoji-Ersetzung wurden aus dem
vorhandenen Code vorbereitet. Kategorien laut Ideenkatalog, tatsächliche Tests
und Bildnachweise müssen noch ergänzt werden. Der Hinweis auf das vorhandene
Qt-Designer-Grundgerüst ist vor Abgabe fachlich zu klären.

## Kompilieren

Aus diesem Ordner:

```sh
latexmk -pdf -interaction=nonstopmode -halt-on-error main.tex
```

Alternativ `pdflatex -interaction=nonstopmode -halt-on-error main.tex` zweimal
ausführen, damit Inhaltsverzeichnis und Verweise stimmen.
Benötigt werden eine LaTeX-Installation mit deutscher Babel-Sprachunterstützung
und die in `main.tex` genannten Pakete. Kein Python/Pygments oder Shell-Escape nötig.

Auf diesem Rechner liegt auch TeX Live unter `/usr/bin`; falls die vorrangigen
MiKTeX-Wrapper Probleme verursachen:

```sh
PATH=/usr/bin:/bin /usr/bin/latexmk -pdf -interaction=nonstopmode -halt-on-error main.tex
```

Die lokale TeX-Live-Installation hat derzeit keine deutsche Babel-Sprachdatei.
Für den Prüfbuild wurde die vorhandene MiKTeX-Datei über
`TEXINPUTS=.:/home/simon/.miktex/texmfs/install/tex/latex/babel-german//:`
eingebunden. Für reguläre TeX-Live-Builds das Paket `texlive-lang-german`
installieren; dadurch stehen auch die deutschen Trennmuster bereit.

Die relativen Pfade setzen die bestehende Ordnerstruktur voraus: `latex/`,
`RPI-ChatClient/` und die Angabe-PDF müssen nebeneinander liegen. Für einen
Upload in einen Online-Editor daher diese Struktur erhalten und `latex/main.tex`
als Hauptdokument wählen. Änderungen an den Projektdateien werden beim nächsten
LaTeX-Build automatisch übernommen; neue Dateien zusätzlich in `quellcode.tex`
eintragen. Generierte Build-Dateien werden nicht abgedruckt.

Die vier Emoji-Literale erscheinen im Listing als explizit erläuterte
Unicode-Codepunkte, damit pdfLaTeX ohne zusätzliche Emoji-Schriften funktioniert.
Die Originaldateien werden dabei nicht verändert.

Für die Abgabe die fertige PDF zusammen mit den Projektdateien als ZIP packen.
