#define _CRT_SECURE_NO_WARNINGS
#include "mapa.h"
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

char** leituraMapa(infoMapa info) {
    FILE* abreMapa = fopen(info.localMapa, "r");
    if (abreMapa == NULL) return NULL;

    char** matriz = (char**)malloc(info.linhas * sizeof(char*));
    for (int i = 0; i < info.linhas; i++) {
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
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

void loadMapa() {
    map.matrizMapa = leituraMapa(map);
    map.mapaImagem[0] = LoadTexture("Texturas/Mapa/SpriteSheetMap.png");
}
void unloadMapa() {
    UnloadTexture(map.mapaImagem[0]);
    liberaMapa(map.matrizMapa, map.linhas);
    map.matrizMapa = NULL;
}

void desenhaMapa() {
    for (int i = 0; i < map.linhas; i++) {
        for (int j = 0; j < map.colunas; j++) {
            char c = map.matrizMapa[i][j];
            float posX = j * bloco.largura;
            float posY = i * bloco.altura;

            // Verifica se é espaço vazio mas tem um 'P' vizinho
            // Se sim, desenha bloco decorativo sem colisão
            if (c == ' ' || c == '\0' || c == '\n' || c == '\r') {
                bool temVizinhoP = false;
                for (int di = -4; di <= 4 && !temVizinhoP; di++) {
                    for (int dj = -4; dj <= 4 && !temVizinhoP; dj++) {
                        int ni = i + di;
                        int nj = j + dj;
                        if (ni >= 0 && ni < map.linhas && nj >= 0 && nj < map.colunas) {
                            if (map.matrizMapa[ni][nj] == 'P') {
                                temVizinhoP = true;
                            }
                        }
                    }
                }
                if (temVizinhoP) {
                    // Desenha mais escuro para diferenciar do bloco real
                    DrawTextureRec(map.mapaImagem[0], { 0, 96, 32, 32 }, { posX, posY }, { 150, 150, 150, 255 });
                }
                continue;
            }

            // Blocos normais
            if (c == 'P') {
                DrawTextureRec(map.mapaImagem[0], { 0, 96, 32, 32 }, { posX, posY }, WHITE);
            }
            else if (c == 'C') DrawRectangle((int)posX, (int)posY, (int)(bloco.largura-1), (int)(bloco.altura-1), PURPLE);
            else if (c == 'A') DrawRectangle((int)posX, (int)posY, (int)(bloco.largura-1), (int)(bloco.altura-1), YELLOW);
            else if (c == 'H') DrawRectangle((int)posX, (int)posY, (int)(bloco.largura-1), (int)(bloco.altura-1), BLUE);
        }
    }
}