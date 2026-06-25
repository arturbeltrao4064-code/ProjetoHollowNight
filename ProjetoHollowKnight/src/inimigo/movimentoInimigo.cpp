#include "inimigo.h"
#include "mapa.h"
#include <raylib.h>

static int tempoMudancaDirecao[MAX_INIMIGOS] = { 0 };

Vector2 movimentaInimigo(int indice, Vector2 posicaoAtual) {
    float x = posicaoAtual.x;
    float y = posicaoAtual.y;
    float w = (float)listaInimigos[indice].largura;
    float h = (float)listaInimigos[indice].altura;

    if (tempoMudancaDirecao[indice] <= 0) {
        int novaDirecao = GetRandomValue(0, 1);
        listaInimigos[indice].olhandoDireita = (novaDirecao == 1);
        tempoMudancaDirecao[indice] = GetRandomValue(30, 90);
    }

    float direcaoX = listaInimigos[indice].olhandoDireita ? 1.0f : -1.0f;
    x += direcaoX * constantesJogo.velocidade;
    tempoMudancaDirecao[indice]--;

    if (x > listaInimigos[indice].posicaoInicial.x + 90.0f) {
        listaInimigos[indice].olhandoDireita = false;
    }
    if (x < listaInimigos[indice].posicaoInicial.x - 90.0f) {
        listaInimigos[indice].olhandoDireita = true;
    }

    float frenteX = listaInimigos[indice].olhandoDireita ? x + w : x;
    float pontoY = y + h / 2;
    if (blocoSolido(frenteX, pontoY)) {
        listaInimigos[indice].olhandoDireita = !listaInimigos[indice].olhandoDireita;
        x -= direcaoX * constantesJogo.velocidade;
    }

    y += 4.0f;

    bool colideChaoEsq = blocoSolido(x + 2, y + h);
    bool colideChaoDir = blocoSolido(x + w - 2, y + h);

    if (colideChaoEsq || colideChaoDir) {
        int lin = (int)((y + h) / bloco.altura);
        y = lin * bloco.altura - h - 0.1f;
    }

    return (Vector2){x, y};
}
