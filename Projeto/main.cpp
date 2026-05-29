#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "Estruturas/estruturas.h"
#include "Funcoes/funcoes.h"

// Criamos uma variavel para controlar onde o jogador est�

int main(void) {
    InitWindow(tela.largura, tela.altura, tela.titulo);

    SetTargetFPS(60);

    loadArquivos();

    while (!WindowShouldClose()) {
        desenhaMenu(); // que internamente chama update e draw separados
    }
    unloadArquivos();
    return 0;
}