#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <raylib.h>
#include "estruturas.h"
#include "menu.h"
#include "game.h"
#include "save.h"

int main(void) {
    inicializaEstruturasGlobais();
    InitWindow(tela.largura, tela.altura, tela.titulo);
    SetRandomSeed((unsigned int)time(NULL));
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);

    loadJogo();
    loadMenu();

    while (!WindowShouldClose()) {
        desenhaMenu();
    }

    salvaJogo();
    unloadJogo();
    unloadMenu();
    CloseWindow();
    return 0;
}