@echo off
echo Compilando...

g++ src\*.cpp src\boss\*.cpp src\game\*.cpp src\inimigo\*.cpp src\mapa\*.cpp src\menu\*.cpp src\personagem\*.cpp -I include -I lib -L lib -lraylib -lopengl32 -lgdi32 -lwinmm -o jogo.exe

if %errorlevel% neq 0 (
    echo Erro na compilacao.
) else (
    echo Compilado com sucesso!
    jogo.exe
)

pause
