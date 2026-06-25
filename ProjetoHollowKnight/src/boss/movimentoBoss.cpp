#include "boss.h"
#include "mapa.h"
#include "personagem.h"
#include <raylib.h>

extern float bossVelY;
extern float bossJumpCooldown;

Vector2 movimentaBoss(Vector2 posicaoAtual) {
    float x = posicaoAtual.x;
    float y = posicaoAtual.y;
    float w = (float)chefao.largura;
    float h = (float)chefao.altura;

    float velocidadeBoss = constantesJogo.velocidade * 0.4f;
    float distanciaChase = 300.0f;
    float distanciaPulo = 200.0f;
    float distanciaAtaque = 120.0f;
    float diferencaX = personagem.posicao.x - x;
    float absDiferencaX = diferencaX < 0 ? -diferencaX : diferencaX;

    bool jogadorEstaLonge = absDiferencaX > distanciaPulo;
    bool jogadorEstaPerto = absDiferencaX <= distanciaAtaque;

    if (bossJumpCooldown > 0.0f) {
        bossJumpCooldown -= GetFrameTime();
        if (bossJumpCooldown < 0.0f) bossJumpCooldown = 0.0f;
    }

    if (jogadorEstaLonge) {
        if (diferencaX > 0) chefao.olhandoDireita = true;
        else chefao.olhandoDireita = false;

        if (bossVelY == 0.0f && bossJumpCooldown <= 0.0f) {
            bossVelY = -11.0f;
            bossJumpCooldown = 1.2f;
        }

        x += (diferencaX > 0 ? velocidadeBoss : -velocidadeBoss);
    }
    else {
        float direcaoX = chefao.olhandoDireita ? 1.0f : -1.0f;
        x += direcaoX * velocidadeBoss;

        if (diferencaX > 0) chefao.olhandoDireita = true;
        else chefao.olhandoDireita = false;
    }

    float velocidadeQueda = 4.0f;
    bool pisouNoChao = false;

    bossVelY += constantesJogo.gravidade;
    y += bossVelY;

    for (int i = 1; i <= (int)velocidadeQueda; i++) {
        float testeY = y + i;

        bool colideEsq = blocoSolido(x + 4, testeY + h);
        bool colideMid = blocoSolido(x + (w / 2), testeY + h);
        bool colideDir = blocoSolido(x + w - 4, testeY + h);

        if (colideEsq || colideMid || colideDir) {
            int lin = (int)((testeY + h) / bloco.altura);
            y = lin * bloco.altura - h - 0.1f;
            pisouNoChao = true;
            bossVelY = 0.0f;
            break;
        }
    }

    float limiteInferiorMapa = (map.linhas - 2) * bloco.altura;
    if (y > limiteInferiorMapa) {
        x = chefao.posicaoInicial.x;
        y = chefao.posicaoInicial.y - 10.0f;
    }

    return (Vector2){x, y};
}
