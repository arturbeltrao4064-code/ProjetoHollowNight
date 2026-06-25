#include "mapa.h"
#include <raylib.h>

static int proximoAmuletoNaoColetado() {
    for (int i = 0; i < TOTAL_AMULETOS; i++) {
        if (!personagem.dados.amuletos[i].coletado) {
            return i;
        }
    }
    return -1;
}

static void desenhaAmuletoPorTipo(float posX, float posY, int tipo) {
    if (tipo == AMULETO_ATAQUE) {
        DrawRectangle((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, YELLOW);
        DrawRectangleLines((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, ORANGE);
        DrawText("1", (int)posX + (int)bloco.largura/2 - 4, (int)posY + (int)bloco.altura/2 - 7, 14, BLACK);
    }
    else if (tipo == AMULETO_DEFESA) {
        DrawRectangle((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, ORANGE);
        DrawRectangleLines((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, (Color){200, 100, 0, 255});
        DrawText("2", (int)posX + (int)bloco.largura/2 - 4, (int)posY + (int)bloco.altura/2 - 7, 14, BLACK);
    }
    else if (tipo == AMULETO_VIDA) {
        DrawRectangle((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, GREEN);
        DrawRectangleLines((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, DARKGREEN);
        DrawText("3", (int)posX + (int)bloco.largura/2 - 4, (int)posY + (int)bloco.altura/2 - 7, 14, BLACK);
    }
}

void desenhaMapa() {
    if (map.matrizMapa == nullptr) return;

    for (int i = 0; i < map.linhas; i++) {
        for (int j = 0; j < map.colunas; j++) {
            char c = map.matrizMapa[i][j];
            if (c == ' ' || c == '\n' || c == '\0' || c == '\r' || c == 'J' || c == 'M') continue;

            float posX = j * bloco.largura;
            float posY = i * bloco.altura;

            if (c == 'P') {
                bool paredeSaida = (j >= map.colunas - 2);
                DrawRectangle((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura,
                              paredeSaida ? (Color){180, 40, 40, 255} : BLACK);
            }
            else if (c == 'C') DrawRectangle((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, PURPLE);
            else if (c == 'H') DrawRectangle((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, BLUE);
            else if (c == 'A') {
                // Marcador generico: mostra o proximo amuleto ainda nao coletado.
                int tipo = proximoAmuletoNaoColetado();
                if (tipo >= 0) {
                    desenhaAmuletoPorTipo(posX, posY, tipo);
                }
            }
            else if (c == 'E') {
                desenhaAmuletoPorTipo(posX, posY, AMULETO_DEFESA);
            }
            else if (c == 'L') {
                desenhaAmuletoPorTipo(posX, posY, AMULETO_VIDA);
            }
        }
    }
}
