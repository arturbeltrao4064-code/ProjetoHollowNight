#define _CRT_SECURE_NO_WARNINGS
#include "funcoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

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

char** leituraMapa(infoMapa info) {
    FILE* abreMapa = fopen(info.localMapa, "r");
    if (abreMapa == NULL) return NULL;

    // Aloca as linhas
    char** matriz = (char**)malloc(info.linhas * sizeof(char*));
    for (int i = 0; i < info.linhas; i++) {
        // Aloca colunas (+2 para garantir espaço do \n e \0)
        matriz[i] = (char*)malloc((info.colunas + 5) * sizeof(char));
        if (fgets(matriz[i], info.colunas + 5, abreMapa) == NULL) {
            matriz[i][0] = '\0';
        }
    }

    fclose(abreMapa);
    return matriz;
}

void liberaMapa(char** matriz, int linhas) {
    if (matriz == NULL) return;

    // 1. Libera cada linha (as gavetas) primeiro
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    // 2. Só depois libera o ponteiro principal (a estante)
    free(matriz);
}

Vector2 movimentaPersoangem(Vector2 posicaoAtual) {
    float velocidade = 5.0f;
    if (IsKeyDown(KEY_LEFT) && posicaoAtual.x > 0) posicaoAtual.x -= velocidade;
    if (IsKeyDown(KEY_RIGHT) && posicaoAtual.x < 1100) posicaoAtual.x += velocidade;
    if (IsKeyDown(KEY_UP) && posicaoAtual.y > 0) posicaoAtual.y -= velocidade;
    if (IsKeyDown(KEY_DOWN) && posicaoAtual.y < 700) posicaoAtual.y += velocidade;

    return posicaoAtual;
}



void startJogo(void) {
    // Atualização
    // Desenho
    BeginDrawing();
    ClearBackground(BLACK);

    // 2. Lógica para desenhar o mapa
    //Legenda rapida: 
    // J - jogador - personagem.imagem,laranja,
    // C - Chefe - roxo;
    // P - Parede - branco;
    // A - Amuleto - Amarelo;
    // H - Habilidade - Azul;
    if (map.matrizMapa != NULL) {
        for (int i = 0; i < map.linhas; i++) {
            for (int j = 0; j < map.colunas; j++) {
                // Verifica se o caractere não é espaço ou quebra de linha
                char caractere = map.matrizMapa[i][j];
                
                if (caractere != ' ' && caractere != '\n' && caractere != '\0' && caractere != '\r') {
                    float posX = j * bloco.largura;
                    float posY = i * bloco.altura;
                    if (caractere == 'J') {
                        DrawTextureEx(personagem.imagem, personagem.posicao, 0, 0.3, WHITE);
                        personagem.posicao = movimentaPersoangem(personagem.posicao);
                    }
                    else if (caractere == 'C') {
                        DrawRectangle(posX, posY, bloco.largura - 1, bloco.altura - 1, PURPLE);
                    }
                    else if (caractere == 'P') {
                        DrawRectangle(posX, posY, bloco.largura - 1, bloco.altura - 1, WHITE);
                    }
                    else if (caractere == 'A') {
                        DrawRectangle(posX, posY, bloco.largura - 1, bloco.altura - 1, YELLOW);
                    }
                    else if (caractere == 'H') {
                        DrawRectangle(posX, posY, bloco.largura - 1, bloco.altura - 1, BLUE);
                    }

                }
            }
        }
    }
    EndDrawing();
}

