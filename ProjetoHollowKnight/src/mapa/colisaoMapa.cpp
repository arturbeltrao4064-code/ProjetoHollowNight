#include "mapa.h"

bool blocoSolido(float px, float py) {
    if (map.matrizMapa == nullptr) return false;

    int col = (int)(px / bloco.largura);
    int lin = (int)(py / bloco.altura);

    if (col < 0) col = 0;
    if (col >= map.colunas) col = map.colunas - 1;
    if (lin < 0) lin = 0;
    if (lin >= map.linhas) lin = map.linhas - 1;

    char c = map.matrizMapa[lin][col];
    return c == 'P';
}
