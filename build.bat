@echo off
setlocal

:: Configuration
set SRC=src\main.cpp
set BUILD_DIR=build
set ARTIFACT_NAME=maze-win64
set SFML_PATH=.\SFML-2.6.2.zip
set MINGW_URL=https://github.com/brechtsanders/winlibs_mingw/releases/download/13.1.0-16.0.5-11.0.0-msvcrt-r5/winlibs-x86_64-posix-seh-gcc-13.1.0-mingw-w64msvcrt-11.0.0-r5.zip

echo [*] Téléchargement de SFML...
if not exist sfml (
    mkdir sfml
    powershell -Command "Expand-Archive -Path %SFML_PATH% -DestinationPath sfml"
) else (
    echo [*] SFML déjà téléchargé.
)

echo [*] Téléchargement de MinGW...
if not exist mingw64 (
    mkdir mingw64
    if not exist mingw.zip curl -L -o mingw.zip %MINGW_URL%
    powershell -Command "Expand-Archive -Path mingw.zip -DestinationPath ."
) else (
    echo [*] MinGW déjà téléchargé.
)

echo [*] Ajout de MinGW au PATH
set PATH=%cd%.\mingw64\bin;%PATH%

echo [*] Compilation du projet...
if exist %BUILD_DIR% rd /s /q %BUILD_DIR%
mkdir %BUILD_DIR%
g++ -std=c++17 -O2 ^
    -Isfml\SFML-2.6.2\include ^
    -Lsfml\SFML-2.6.2\lib ^
    -o %BUILD_DIR%\%ARTIFACT_NAME%.exe ^
    %SRC% ^
    -lsfml-graphics -lsfml-window -lsfml-system

if %ERRORLEVEL% neq 0 (
    echo [!] La compilation a échoué.
    exit /b 1
)

echo [*] Copie des DLLs SFML...
copy sfml\SFML-2.6.2\bin\*.dll %BUILD_DIR% >nul

echo [*] Copie des DLLs MinGW...
copy mingw64\bin\*.dll %BUILD_DIR% >nul

echo [*] Création de l’archive...
powershell -Command "Compress-Archive -Path %BUILD_DIR%\* -DestinationPath %ARTIFACT_NAME%-portable.zip"

echo [✓] Compilation et empaquetage terminés avec succès.
exit /b 0
