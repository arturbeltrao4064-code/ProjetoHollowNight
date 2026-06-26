#include "boss.h"
#include "mapa.h"
#include "personagem.h"
#include <raylib.h>

Vector2 movimentaBoss(Vector2 posicaoAtual) {
    float x = posicaoAtual.x;
    float y = posicaoAtual.y;
    float w = (float)bossState.entidade.largura;
    float h = (float)bossState.entidade.altura;

    float velocidadeBoss = constantesJogo.velocidade * 0.4f;
    float distanciaChase = 300.0f;
    float distanciaPulo = 200.0f;
    float distanciaAtaque = 120.0f;
    float diferencaX = personagem.posicao.x - x;
    float absDiferencaX = diferencaX < 0 ? -diferencaX : diferencaX;

    bool jogadorEstaLonge = absDiferencaX > distanciaPulo;
    bool jogadorEstaPerto = absDiferencaX <= distanciaAtaque;

    // Area X dedicada do boss: ele nao sai da sala onde nasceu.
    float areaXMin = bossState.entidade.posicaoInicial.x - 210.0f;
    float areaXMax = bossState.entidade.posicaoInicial.x + 210.0f;

    if (bossState.jumpCooldown > 0.0f) {
        bossState.jumpCooldown -= GetFrameTime();
        if (bossState.jumpCooldown < 0.0f) bossState.jumpCooldown = 0.0f;
    }

    float moveX = 0.0f;

    if (jogadorEstaLonge) {
        if (diferencaX > 0) bossState.entidade.olhandoDireita = true;
        else bossState.entidade.olhandoDireita = false;

        if (bossState.velY == 0.0f && bossState.jumpCooldown <= 0.0f) {
            bossState.velY = -11.0f;
            bossState.jumpCooldown = 1.2f;
        }

        moveX = (diferencaX > 0 ? velocidadeBoss : -velocidadeBoss);
    }
    else {
        float direcaoX = bossState.entidade.olhandoDireita ? 1.0f : -1.0f;
        moveX = direcaoX * velocidadeBoss;

        if (diferencaX > 0) bossState.entidade.olhandoDireita = true;
        else bossState.entidade.olhandoDireita = false;
    }

    // Colisao horizontal com blocos solidos para evitar atravessar paredes.
    if (moveX != 0.0f) {
        float proximoX = x + moveX;
        float checkX = (moveX > 0.0f) ? (proximoX + w - 2.0f) : (proximoX + 2.0f);
        bool colideX = blocoSolido(checkX, y + h * 0.35f)
                    || blocoSolido(checkX, y + h * 0.7f)
                    || blocoSolido(checkX, y + h - 2.0f);
        if (!colideX) {
            x = proximoX;
        } else {
            // Evita ficar preso em parede/quina apos o pre-tempo.
            bossState.entidade.olhandoDireita = !bossState.entidade.olhandoDireita;
            if (bossState.velY == 0.0f) {
                bossState.velY = -6.0f;
            }
        }
    }

    bossState.velY += constantesJogo.gravidade;
    float deslocY = bossState.velY;
    float absDeslocY = (deslocY < 0.0f) ? -deslocY : deslocY;
    int passosY = (int)absDeslocY;
    if (passosY < 1) passosY = 1;
    float passoY = deslocY / (float)passosY;

    // Move em passos pequenos para evitar atravessar bloco durante pulo/queda.
    for (int i = 0; i < passosY; i++) {
        float proximoY = y + passoY;

        if (passoY < 0.0f) {
            bool colideTopoEsq = blocoSolido(x + 4, proximoY + 1);
            bool colideTopoMid = blocoSolido(x + (w / 2), proximoY + 1);
            bool colideTopoDir = blocoSolido(x + w - 4, proximoY + 1);
            if (colideTopoEsq || colideTopoMid || colideTopoDir) {
                int linTopo = (int)(proximoY / bloco.altura);
                y = (linTopo + 1) * bloco.altura + 0.1f;
                bossState.velY = 0.0f;
                break;
            }
        }

        if (passoY > 0.0f) {
            bool colidePeEsq = blocoSolido(x + 4, proximoY + h);
            bool colidePeMid = blocoSolido(x + (w / 2), proximoY + h);
            bool colidePeDir = blocoSolido(x + w - 4, proximoY + h);
            if (colidePeEsq || colidePeMid || colidePeDir) {
                int lin = (int)((proximoY + h) / bloco.altura);
                y = lin * bloco.altura - h - 0.1f;
                bossState.velY = 0.0f;
                break;
            }
        }

        y = proximoY;
    }

    if (y < 0.0f) {
        y = 0.0f;
        if (bossState.velY < 0.0f) bossState.velY = 0.0f;
    }

    float limiteInferiorMapa = (map.linhas - 2) * bloco.altura;
    if (y > limiteInferiorMapa) {
        x = bossState.entidade.posicaoInicial.x;
        y = bossState.entidade.posicaoInicial.y - 10.0f;
    }

    // Limites horizontais do mapa para o boss nao sair da area jogavel.
    if (x < 0.0f) x = 0.0f;
    float limiteDireito = map.colunas * bloco.largura - w;
    if (x > limiteDireito) x = limiteDireito;

    if (x < areaXMin) {
        x = areaXMin;
        bossState.entidade.olhandoDireita = true;
    }
    if (x > areaXMax) {
        x = areaXMax;
        bossState.entidade.olhandoDireita = false;
    }

    return (Vector2){x, y};
}
