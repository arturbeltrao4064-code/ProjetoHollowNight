#include "mapa.h"
#include <raylib.h>

static Texture2D texturaBlocoChao = { 0 };
static Texture2D texturaBlocoTeto = { 0 };
static bool blocosCarregados = false;

static void carregaTexturasBlocos() {
    if (!blocosCarregados) {
        texturaBlocoChao = LoadTexture("assets/Cenario/blocoChao.png");
        texturaBlocoTeto = LoadTexture("assets/Cenario/blocoTeto.png");
        blocosCarregados = true;
    }
}

static int proximoAmuletoNaoColetado() {
    for (int i = 0; i < TOTAL_AMULETOS; i++) {
        if (!personagem.dados.amuletos[i].coletado) {
            return i;
        }
    }
    return -1;
}

static void desenhaAmuletoPorTipo(float posX, float posY, int tipo) {
    Texture2D textura = texturaAmuletos[tipo];
    if (textura.width > 0 && textura.height > 0) {
        Rectangle src = { 0.0f, 0.0f, (float)textura.width, (float)textura.height };
        Rectangle dst = { posX, posY, bloco.largura, bloco.altura };
        Vector2 origin = { 0.0f, 0.0f };
        DrawTexturePro(textura, src, dst, origin, 0.0f, WHITE);
        DrawRectangleLines((int)posX - 1, (int)posY - 1, (int)bloco.largura + 2, (int)bloco.altura + 2, WHITE);
        return;
    }

    if (tipo == AMULETO_ATAQUE) {
        DrawRectangle((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, YELLOW);
        DrawRectangleLines((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, ORANGE);
        DrawRectangleLines((int)posX - 1, (int)posY - 1, (int)bloco.largura + 2, (int)bloco.altura + 2, WHITE);
        DrawText("1", (int)posX + (int)bloco.largura/2 - 4, (int)posY + (int)bloco.altura/2 - 7, 14, BLACK);
    }
    else if (tipo == AMULETO_DEFESA) {
        DrawRectangle((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, ORANGE);
        DrawRectangleLines((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, (Color){200, 100, 0, 255});
        DrawRectangleLines((int)posX - 1, (int)posY - 1, (int)bloco.largura + 2, (int)bloco.altura + 2, WHITE);
        DrawText("2", (int)posX + (int)bloco.largura/2 - 4, (int)posY + (int)bloco.altura/2 - 7, 14, BLACK);
    }
    else if (tipo == AMULETO_VIDA) {
        DrawRectangle((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, GREEN);
        DrawRectangleLines((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, DARKGREEN);
        DrawRectangleLines((int)posX - 1, (int)posY - 1, (int)bloco.largura + 2, (int)bloco.altura + 2, WHITE);
        DrawText("3", (int)posX + (int)bloco.largura/2 - 4, (int)posY + (int)bloco.altura/2 - 7, 14, BLACK);
    }
}

void desenhaMapa() {
    if (map.matrizMapa == nullptr) return;

    carregaTexturasBlocos();

    for (int i = 0; i < map.linhas; i++) {
        for (int j = 0; j < map.colunas; j++) {
            char c = map.matrizMapa[i][j];
            if (c == ' ' || c == '\n' || c == '\0' || c == '\r' || c == 'J' || c == 'M') continue;

            float posX = j * bloco.largura;
            float posY = i * bloco.altura;

            if (c == 'P') {
                bool paredeSaida = (j >= map.colunas - 2);
                if (paredeSaida) {
                    if (texturaBlocoChao.width > 0) {
                        Rectangle src = { 0.0f, 0.0f, (float)texturaBlocoChao.width, (float)texturaBlocoChao.height };
                        Rectangle dst = { posX, posY, bloco.largura, bloco.altura };
                        Vector2 origin = { 0.0f, 0.0f };
                        DrawTexturePro(texturaBlocoChao, src, dst, origin, 0.0f, (Color){200, 200, 200, 255});
                    } else {
                        DrawRectangle((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, (Color){180, 40, 40, 255});
                    }
                } else if (texturaBlocoChao.width > 0) {
                    Rectangle src = { 0.0f, 0.0f, (float)texturaBlocoChao.width, (float)texturaBlocoChao.height };
                    Rectangle dst = { posX, posY, bloco.largura, bloco.altura };
                    Vector2 origin = { 0.0f, 0.0f };
                    DrawTexturePro(texturaBlocoChao, src, dst, origin, 0.0f, WHITE);
                } else {
                    DrawRectangle((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, BLACK);
                }
            }
            else if (c == 'C') continue;
            else if (c == 'H') {
                if (texturaHabilidade[0].width > 0) {
                    Rectangle src = { 0.0f, 0.0f, (float)texturaHabilidade[0].width, (float)texturaHabilidade[0].height };
                    Rectangle dst = { posX - bloco.largura * 0.15f, posY - bloco.altura * 0.8f, bloco.largura * 1.8f, bloco.altura * 2.3f };
                    Vector2 origin = { 0.0f, 0.0f };
                    DrawTexturePro(texturaHabilidade[0], src, dst, origin, 0.0f, WHITE);
                    DrawRectangleLines((int)posX - 1, (int)posY - 1, (int)bloco.largura + 2, (int)bloco.altura + 2, WHITE);
                } else {
                    DrawRectangle((int)posX, (int)posY, (int)bloco.largura, (int)bloco.altura, BLUE);
                    DrawRectangleLines((int)posX - 1, (int)posY - 1, (int)bloco.largura + 2, (int)bloco.altura + 2, WHITE);
                }
            }
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
