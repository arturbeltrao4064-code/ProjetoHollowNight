@echo off
setlocal enabledelayedexpansion
set "ROOT=%~dp0.."

pushd "%ROOT%"
if %errorlevel% neq 0 (
    echo Erro: nao foi possivel acessar a raiz do projeto.
    pause
    exit /b 1
)

echo Compilando...

g++ src\main.cpp src\estruturas.cpp src\hud.cpp src\save.cpp ^
    src\boss\*.cpp src\game\*.cpp src\inimigo\*.cpp src\mapa\*.cpp src\menu\*.cpp src\personagem\*.cpp ^
    -I include -I lib -L lib -lraylib -lopengl32 -lgdi32 -lwinmm -o jogo.exe

if %errorlevel% neq 0 (
    echo Erro na compilacao.
) else (
    echo Compilado com sucesso!
    jogo.exe
)

popd

pause
