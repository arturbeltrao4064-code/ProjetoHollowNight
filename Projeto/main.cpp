#define _CRT_SECURE_NO_WARNINGS
#include <raylib.h>
#include "Estruturas/estruturas.h"
#include "Menu/menu.h"
#include "Game/game.h"

int main(void) {
    InitWindow(tela.largura, tela.altura, tela.titulo);
    SetTargetFPS(60);

    loadJogo();
    loadMenu();

    while (!WindowShouldClose()) {
        desenhaMenu();
    }

    unloadJogo();
    unloadMenu();
    CloseWindow();
    return 0;
}