#include "inimigo.h"
#include "mapa.h"
#include <raylib.h>

static int tempoMudancaDirecao[MAX_INIMIGOS] = { 0 };

void loadInimigo() {
    // Carrega as texturas normalmente
    listaInimigos[0].imagem[0] = LoadTexture("Texturas/Inimigo/Inimigo.png");
    listaInimigos[0].imagem[1] = LoadTexture("Texturas/Inimigo/InverteInimigo.png");
}

void unloadInimigo() {
    UnloadTexture(listaInimigos[0].imagem[0]);
    UnloadTexture(listaInimigos[0].imagem[1]);
}

Vector2 movimentaInimigoEspecifico(int indice, Vector2 posicaoAtual) {
    float x = posicaoAtual.x;
    float y = posicaoAtual.y;
    float w = (float)listaInimigos[indice].largura;
    float h = (float)listaInimigos[indice].altura;

    if (tempoMudancaDirecao[indice] <= 0) {
        int novaDirecao = GetRandomValue(0, 1);
        listaInimigos[indice].olhandoDireita = (novaDirecao == 1);
        tempoMudancaDirecao[indice] = GetRandomValue(30, 90); // troca direção a cada 0.5 a 1.5 segundos
    }

    float direcaoX = listaInimigos[indice].olhandoDireita ? 1.0f : -1.0f;
    x += direcaoX * constantesJogo.velocidade;
    tempoMudancaDirecao[indice]--;

    // Se bater em um limite de 90 pixels do ponto inicial, muda de direção para não fugir demais
    if (x > listaInimigos[indice].posicaoInicial.x + 90.0f) {
        listaInimigos[indice].olhandoDireita = false;
    }
    if (x < listaInimigos[indice].posicaoInicial.x - 90.0f) {
        listaInimigos[indice].olhandoDireita = true;
    }

    // Se bater em uma parede, inverte a direção imediatamente
    float frenteX = listaInimigos[indice].olhandoDireita ? x + w : x;
    float pontoY = y + h / 2;
    if (blocoSolido(frenteX, pontoY)) {
        listaInimigos[indice].olhandoDireita = !listaInimigos[indice].olhandoDireita;
        x -= direcaoX * constantesJogo.velocidade; // desfaz o último movimento
    }

    // --- 2. GRAVIDADE SIMPLIFICADA E SEGURA ---
    // Aplica uma força constante de queda (simulando a gravidade de forma direta)
    y += 4.0f;

    // Colisão vertical com o chão do teu mapa
    bool colideChaoEsq = blocoSolido(x + 2,     y + h);
    bool colideChaoDir = blocoSolido(x + w - 2, y + h);

    if (colideChaoEsq || colideChaoDir) {
        int lin = (int)((y + h) / bloco.altura);
        y = lin * bloco.altura - h - 0.1f; // Ajusta perfeitamente no topo do bloco
    }

    // Retorna a nova posição corrigida para o elemento do array
    return {x, y};
}

void updateInimigo() {
    // Atualiza todos os inimigos vivos do array
    for (int i = 0; i < quantidadeInimigos; i++) {
        if (listaInimigos[i].dados.vivo) {
            listaInimigos[i].posicao = movimentaInimigoEspecifico(i, listaInimigos[i].posicao);
        }
    }
}

void desenhaInimigo() {
    // Desenha todos os inimigos na tela com o tamanho correto (30x30)
    for (int i = 0; i < quantidadeInimigos; i++) {
        if (listaInimigos[i].dados.vivo) {
            DrawRectangle((int)listaInimigos[i].posicao.x, (int)listaInimigos[i].posicao.y,listaInimigos[i].largura, listaInimigos[i].altura, RED);
        }
    }
}
