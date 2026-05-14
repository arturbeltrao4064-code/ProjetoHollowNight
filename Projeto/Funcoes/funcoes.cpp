#define _CRT_SECURE_NO_WARNINGS
#include "funcoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

Vector2 movimentaPersoangem(Vector2 personagemPosicao) {
    float velocidade = 5.0f;//quanto o perosnagem andará
    if (IsKeyDown(KEY_LEFT) && personagemPosicao.x > 0) {
        personagemPosicao.x -= velocidade;
    }
    if (IsKeyDown(KEY_RIGHT) && personagemPosicao.x < 1100) {
        personagemPosicao.x += velocidade;
    }
    if (IsKeyDown(KEY_UP) && personagemPosicao.y > 0) {
        personagemPosicao.y -= velocidade;
    }
    if (IsKeyDown(KEY_DOWN) && personagemPosicao.y < 700) {
        personagemPosicao.y += velocidade;
    }
    return personagemPosicao;
}


char** leituraMapa(infoMapa info) {
    FILE* abreMapa = fopen(info.localMapa, "r");
    if (abreMapa == NULL) return NULL;

    // Aloca as linhas
    char** matriz = (char**)malloc(info.linhas * sizeof(char*));
    for (int i = 0; i < info.linhas; i++) {
        // Aloca colunas (+2 para garantir espaço do \n e \0)
        matriz[i] = (char*)malloc((info.colunas + 2) * sizeof(char));
        if (fgets(matriz[i], info.colunas + 2, abreMapa) == NULL) {
            matriz[i][0] = '\0';
        }
    }

    fclose(abreMapa);
    return matriz;
}

void liberaMapa(char** matriz, int linhas) {
    if (matriz == NULL) return;
    free(matriz);
    for (int i = 0; i < linhas; i++) free(matriz[i]);
}
void startJogo(void) {
    // Atualização
    personagem.posicao = movimentaPersoangem(personagem.posicao);

    // Desenho
    BeginDrawing();
    ClearBackground(BLACK);

    // 2. Lógica para desenhar o mapa
    if (map.matrizMapa != NULL) {
        for (int i = 0; i < map.linhas; i++) {
            for (int j = 0; j < map.colunas; j++) {
                // Verifica se o caractere não é espaço ou quebra de linha
                char caractere = map.matrizMapa[i][j];
                if (caractere != ' ' && caractere != '\n' && caractere != '\0' && caractere != '\r') {

                    // Calcula a posição do bloco baseado no índice da matriz
                    float posX = j * bloco.largura;
                    float posY = i * bloco.altura;

                    DrawRectangle(posX, posY, bloco.largura - 1, bloco.altura - 1, GREEN);

                }
            }
        }
    }
    DrawTextureV(personagem.imagem, personagem.posicao, WHITE);
    EndDrawing();
}

char menu(void) {
    // Por enquanto, apenas detecta se o jogador apertou 'C'
    if (IsKeyPressed(KEY_C)) return 'c';
    if (IsKeyPressed(KEY_S)) return 's';
    if (IsKeyPressed(KEY_O)) return 'o';
    return ' '; // Retorna vazio se nenhuma tecla do menu foi pressionada
}
void saveGame(void) {
    // Lógica de salvar futuramente
}

void outGame(void) {
    // Lógica de sair
    CloseWindow();
    exit(0);
}