#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "Estruturas\estruturas.h"
#include "Funcoes\funcoes.h"

// Criamos uma variável para controlar onde o jogador está
enum Estado { ESTADO_MENU, ESTADO_JOGANDO };

Estado estadoAtual = ESTADO_MENU;

int main(void) {
    InitWindow(tela.largura, tela.altura, tela.titulo);
    map.matrizMapa = leituraMapa(map);
    personagem.imagem = LoadTexture("Texturas/Personagem/Personagem1.png");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        if (estadoAtual == ESTADO_MENU) {
            if (IsKeyPressed(KEY_C)) {
                estadoAtual = ESTADO_JOGANDO;
            }

            BeginDrawing();
            ClearBackground(BLACK);
            DrawText("HOLLOW KNIGHT - MENU", 400, 300, 30, WHITE);
            DrawText("Pressione 'C' para iniciar", 450, 400, 20, WHITE);
            EndDrawing();
        }
        else if (estadoAtual == ESTADO_JOGANDO) {
            // Se estiver jogando, chama a função de lógica/desenho
            startJogo();

            // Se apertar ESC, volta para o menu
            if (IsKeyPressed(KEY_ESCAPE)) {
                estadoAtual = ESTADO_MENU;
            }
        }
    }

    UnloadTexture(personagem.imagem);
    liberaMapa(map.matrizMapa, map.linhas);
    CloseWindow();
    return 0;
}