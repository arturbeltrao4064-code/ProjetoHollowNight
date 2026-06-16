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

            if (c == 'P') {
                DrawTextureRec(map.mapaImagem[0], { 0, 96, 32, 32 }, { posX, posY }, WHITE);
            }
            else if (c == 'C') DrawRectangle((int)posX, (int)posY, (int)(bloco.largura - 1), (int)(bloco.altura - 1), PURPLE);
            else if (c == 'A') DrawRectangle((int)posX, (int)posY, (int)(bloco.largura - 1), (int)(bloco.altura - 1), YELLOW);
            else if (c == 'H') DrawRectangle((int)posX, (int)posY, (int)(bloco.largura - 1), (int)(bloco.altura - 1), BLUE);
        }
    }
}
