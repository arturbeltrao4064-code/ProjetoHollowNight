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
            else if (c == 'H') DrawRectangle((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, BLUE);
            else if (c == 'A') {
                // Amuleto de Ataque (1) - amarelo
                DrawRectangle((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, YELLOW);
                DrawRectangleLines((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, ORANGE);
                DrawText("1", (int)posX + (int)bloco.largura/2 - 4, (int)posY + (int)bloco.altura/2 - 7, 14, BLACK);
            }
            else if (c == 'E') {
                // Amuleto de Velocidade (2) - laranja
                DrawRectangle((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, ORANGE);
                DrawRectangleLines((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, (Color){200, 100, 0, 255});
                DrawText("2", (int)posX + (int)bloco.largura/2 - 4, (int)posY + (int)bloco.altura/2 - 7, 14, BLACK);
            }
            else if (c == 'L') {
                // Amuleto de Vida (3) - verde
                DrawRectangle((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, GREEN);
                DrawRectangleLines((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, DARKGREEN);
                DrawText("3", (int)posX + (int)bloco.largura/2 - 4, (int)posY + (int)bloco.altura/2 - 7, 14, BLACK);
            }
        }
    }
}
