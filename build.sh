#!/bin/bash

# === Configuration ===
SRC="src/main.cpp"
BUILD_DIR="build"
ARTIFACT_NAME="maze-linux"
SFML_URL="https://www.sfml-dev.org/files/SFML-2.6.2-linux-gcc-64-bit.tar.gz"
SFML_ARCHIVE="SFML-2.6.2-linux.tar.gz"
SFML_DIR="sfml"

# === Téléchargement de SFML ===
echo "[*] Téléchargement de SFML..."
if [ ! -d "$SFML_DIR" ]; then
    mkdir -p "$SFML_DIR"
    if [ ! -f "$SFML_ARCHIVE" ]; then
        curl -L -o "$SFML_ARCHIVE" "$SFML_URL"
    fi
    tar -xzf "$SFML_ARCHIVE" -C "$SFML_DIR" --strip-components=1
else
    echo "[*] SFML déjà téléchargé."
fi

# === Compilation ===
echo "[*] Compilation du projet..."
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

g++ -std=c++17 -O2 \
    -I"$SFML_DIR/include" \
    -L"$SFML_DIR/lib" \
    "$SRC" -o "$BUILD_DIR/$ARTIFACT_NAME" \
    -lsfml-graphics -lsfml-window -lsfml-system \
    -g3 

if [ $? -ne 0 ]; then
    echo "[!] La compilation a échoué."
    exit 1
fi

# === Packaging (zip) ===
echo "[*] Création de l’archive..."
ZIP_NAME="$ARTIFACT_NAME-portable.zip"
rm -f "$ZIP_NAME"
cd "$BUILD_DIR" || exit 1
zip -r "../$ZIP_NAME" .
cd ..

echo "[✓] Compilation et empaquetage terminés avec succès."
exit 0
