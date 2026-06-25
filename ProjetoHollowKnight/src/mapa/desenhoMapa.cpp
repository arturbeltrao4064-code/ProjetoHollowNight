#include "mapa.h"
#include <raylib.h>

void desenhaMapa() {
    if (map.matrizMapa == nullptr) return;

    for (int i = 0; i < map.linhas; i++) {
        for (int j = 0; j < map.colunas; j++) {
            char c = map.matrizMapa[i][j];
            if (c == ' ' || c == '\n' || c == '\0' || c == '\r' || c == 'J' || c == 'M') continue;

            float posX = j * bloco.largura;
            float posY = i * bloco.altura;

            if      (c == 'P') DrawRectangle((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, BLACK);
            else if (c == 'C') DrawRectangle((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, PURPLE);
            else if (c == 'A') DrawRectangle((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, YELLOW);
            else if (c == 'H') DrawRectangle((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, BLUE);
        }
    }
}
