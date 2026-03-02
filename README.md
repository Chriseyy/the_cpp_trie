# The C++ Trie

Ein generischer Präfixbaum (Trie) in C++, implementiert als performante und saubere **Header-Only Template-Klasse**. 

Dieses Projekt nutzt einen Template-Ansatz (`Trie<T, E>`), wodurch der Trie extrem flexibel ist und beliebige Datentypen (wie `std::string`, `int`, etc.) verwalten kann. Das Projekt beinhaltet zudem eine vollautomatisierte Test-Suite, die mit Google Test (gtest) geschrieben wurde.

## Features
* **Template-basiert:** Unterstützt beliebige Schlüssel- und Wertetypen.
* **Header-Only:** Die gesamte Logik steckt in der `include/trie/Trie.h`. Sie kann einfach per `#include` in jedes andere C++ Projekt übernommen werden, ohne `.cpp`-Dateien kompilieren zu müssen.
* **Umfassend getestet:** Über 15 maßgeschneiderte Unit-Tests prüfen alle Randfälle (Einfügen, Suchen, Löschen, leere Tries, Iterator-Verhalten).
* **Plug & Play Setup:** Dank modernem CMake wird Google Test automatisch heruntergeladen. Es sind keine manuellen Bibliotheks-Installationen nötig!

## Voraussetzungen
* Ein C++ Compiler (g++, clang++, oder MSVC)
* **CMake** (Version 3.14 oder neuer)
* **Make** (oder Ninja)

*(Hinweis: Google Test wird beim Ausführen von CMake automatisch im Hintergrund von GitHub geladen und eingebunden).*

## Kompilieren und Ausführen

Öffne dein Terminal im Hauptverzeichnis des Projekts und führe folgende Befehle aus, um das Projekt zu bauen und die Tests zu starten:

```bash
# 1. Erstelle einen Build-Ordner und wechsle dorthin
mkdir build
cd build

# 2. Lass CMake die Bauanleitung erstellen und Google Test herunterladen
cmake ..

# 3. Kompiliere das Projekt
make

# 4. Führe alle Tests aus!
./run_trie_tests