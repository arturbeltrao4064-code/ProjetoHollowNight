#define _CRT_SECURE_NO_WARNINGS
#include "mapa.h"
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

char** leituraMapa(infoMapa info) {
    FILE* abreMapa = fopen((char*)info.localMapa, "r");
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
    FILE* arquivo = fopen((char*)map.localMapa, "r");
    if (arquivo == NULL) {
        TraceLog(LOG_ERROR, "Nao foi possivel abrir o mapa para medicao: %s", map.localMapa);
        return;
    }

    int contLinhas = 0;
    int maxColunas = 0;
    char buffer[512];

    while (fgets(buffer, sizeof(buffer), arquivo) != NULL) {
        contLinhas++;
        int tam = 0;
        while (buffer[tam] != '\0' && buffer[tam] != '\n' && buffer[tam] != '\r') {
            tam++;
        }
        if (tam > maxColunas) {
            maxColunas = tam;
        }
    }
    fclose(arquivo);

    map.linhas = contLinhas;
    map.colunas = maxColunas;
    map.matrizMapa = leituraMapa(map);

    TraceLog(LOG_INFO, "Mapa carregado dinamicamente [%s] -> Linhas: %d, Colunas: %d", map.localMapa, map.linhas, map.colunas);
}

void unloadMapa() {
    liberaMapa(map.matrizMapa, map.linhas);
    map.matrizMapa = NULL;
}
