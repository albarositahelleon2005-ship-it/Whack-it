@echo off
REM ===================================================================
REM  empaquetar.bat - Arma la carpeta que se le pasa a los jugadores.
REM
REM  Antes de correrlo: en Code::Blocks elige el objetivo "Release"
REM  (la lista de arriba, junto al boton de compilar) y compila.
REM ===================================================================

setlocal
cd /d "%~dp0"

if not exist "bin\Release\WhackIt.exe" (
    echo.
    echo  No encuentro bin\Release\WhackIt.exe
    echo.
    echo  En Code::Blocks: cambia el objetivo de "Debug" a "Release"
    echo  y compila ^(Build^). Luego vuelve a correr este archivo.
    echo.
    pause
    exit /b 1
)

set "DESTINO=Distribuir\WhackIt"

if exist "Distribuir" rmdir /s /q "Distribuir"
mkdir "%DESTINO%"

echo  Copiando el juego...
copy  "bin\Release\WhackIt.exe" "%DESTINO%\" >nul
xcopy "recursos" "%DESTINO%\recursos" /e /i /q >nul
copy  "LEEME.txt" "%DESTINO%\" >nul

echo  Comprimiendo...
powershell -NoProfile -Command "Compress-Archive -Path 'Distribuir\WhackIt' -DestinationPath 'Distribuir\WhackIt.zip' -Force"

echo.
echo  ================================================
echo   Listo.
echo.
echo   Carpeta lista para jugar:  Distribuir\WhackIt
echo   Archivo para compartir:    Distribuir\WhackIt.zip
echo  ================================================
echo.
pause
