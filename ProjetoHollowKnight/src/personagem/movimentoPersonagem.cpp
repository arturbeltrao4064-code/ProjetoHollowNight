#include "personagem.h"
#include "mapa.h"
#include <raylib.h>

Vector2 movimentaPersonagem(Vector2 posicaoAtual) {
    float x = posicaoAtual.x;
    float y = posicaoAtual.y;
    float w = (float)personagem.largura;
    float h = (float)personagem.altura;

    float dx = 0;
    if (IsKeyDown(KEY_RIGHT)) dx = constantesJogo.velocidade;
    if (IsKeyDown(KEY_LEFT))  dx = -constantesJogo.velocidade;

    if (dx != 0) {
        x += dx;
        float checkX = (dx > 0) ? (x + w) : x;
        bool colideEsq = blocoSolido(checkX, y + 2);
        bool colideDir = blocoSolido(checkX, y + h - 2);

        if (colideEsq || colideDir) {
            if (dx > 0) {
                int col = (int)((x + w) / bloco.largura);
                x = col * bloco.largura - w - 0.1f;
            } else {
                int col = (int)(x / bloco.largura);
                x = (col + 1) * bloco.largura + 0.1f;
            }
        }
    }

    constantesJogo.velocidadeY += constantesJogo.gravidade;
    y += constantesJogo.velocidadeY;

    if (constantesJogo.velocidadeY >= 0) {
        bool colideChaoEsq = blocoSolido(x + 2, y + h);
        bool colideChaoDir = blocoSolido(x + w - 2, y + h);

        if (colideChaoEsq || colideChaoDir) {
            int lin = (int)((y + h) / bloco.altura);
            y = lin * bloco.altura - h - 0.1f;
            constantesJogo.velocidadeY = 0;
        }
    }

    if (constantesJogo.velocidadeY < 0) {
        bool colideTeto1 = blocoSolido(x + 2, y);
        bool colideTeto2 = blocoSolido(x + w - 2, y);

        if (colideTeto1 || colideTeto2) {
            int lin = (int)(y / bloco.altura);
            y = (lin + 1) * bloco.altura + 0.1f;
            constantesJogo.velocidadeY = 0;
        }
    }

    bool noChaoEsq = blocoSolido(x + 2, y + h + 1);
    bool noChaoDir = blocoSolido(x + w - 2, y + h + 1);
    bool noChao    = noChaoEsq || noChaoDir;

    if (IsKeyPressed(KEY_UP) && noChao) {
        constantesJogo.velocidadeY = constantesJogo.forcaPulo;
    }

    // Trava de limites do mapa para impedir sair da sala pelo teto/laterais.
    if (y < 0.0f) {
        y = 0.0f;
        if (constantesJogo.velocidadeY < 0.0f) constantesJogo.velocidadeY = 0.0f;
    }

    if (x < 0.0f) x = 0.0f;
    float limiteDireito = map.colunas * bloco.largura - w;
    if (x > limiteDireito) x = limiteDireito;

    return (Vector2){ x, y };
}
