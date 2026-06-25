#include "boss.h"
#include "mapa.h"
#include "personagem.h"
#include <raylib.h>

// Definição das variáveis globais do Boss
infoEntidade chefao = { 0 };
bool bossAtivo = false;
bool bossPodeReceberDano = false;
static float bossVelY = 0.0f;
static float bossCountdown = 3.0f;
static bool bossMovendo = false;
static bool bossPreparando = false;
static float bossJumpCooldown = 0.0f;

void loadBoss() {
    bossVelY = 0.0f;
    bossCountdown = 3.0f;
    bossMovendo = false;
    bossPreparando = false;
    bossPodeReceberDano = false;
    bossJumpCooldown = 0.0f;
}

void unloadBoss() {
    // Caso use texturas no futuro, descarregue aqui
}

Vector2 movimentaBoss(Vector2 posicaoAtual) {
    float x = posicaoAtual.x;
    float y = posicaoAtual.y;
    float w = (float)chefao.largura;
    float h = (float)chefao.altura;

    float velocidadeBoss = constantesJogo.velocidade * 0.4f; // boss bem mais lento
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

    // --- 1. PADRÃO DE ATAQUE ---
    if (jogadorEstaLonge) {
        if (diferencaX > 0) chefao.olhandoDireita = true;
        else chefao.olhandoDireita = false;

        // salto em direção ao jogador apenas quando estiver no chão e o cooldown terminar
        if (bossVelY == 0.0f && bossJumpCooldown <= 0.0f) {
            bossVelY = -11.0f;
            bossJumpCooldown = 1.2f;
        }

        // movimento horizontal suave enquanto estiver no ar ou aguardando o salto
        x += (diferencaX > 0 ? velocidadeBoss : -velocidadeBoss);
    }
    else {
        // quando perto: ataque frontal lento (andar para frente)
        float direcaoX = chefao.olhandoDireita ? 1.0f : -1.0f;
        x += direcaoX * velocidadeBoss;

        // ajusta a direção do boss se o jogador estiver do outro lado
        if (diferencaX > 0) chefao.olhandoDireita = true;
        else chefao.olhandoDireita = false;
    }

    // --- 3. GRAVIDADE E DETECÇÃO DE CHÃO ---
    float velocidadeQueda = 4.0f;
    bool pisouNoChao = false;

    // atualiza a velocidade vertical do boss com gravidade
    bossVelY += constantesJogo.gravidade;
    y += bossVelY;

    for (int i = 1; i <= (int)velocidadeQueda; i++) {
        float testeY = y + i;

        bool colideEsq = blocoSolido(x + 4,       testeY + h);
        bool colideMid = blocoSolido(x + (w / 2), testeY + h);
        bool colideDir = blocoSolido(x + w - 4,   testeY + h);

        if (colideEsq || colideMid || colideDir) {
            int lin = (int)((testeY + h) / bloco.altura);
            y = lin * bloco.altura - h - 0.1f;
            pisouNoChao = true;
            bossVelY = 0.0f;
            break;
        }
    }

    if (!pisouNoChao) {
        // trava limitando queda, mas isso já está sendo aplicado via y += bossVelY
    }

    // --- 4. TRAVA ANTILIMBO ---
    float limiteInferiorMapa = (map.linhas - 2) * bloco.altura;
    if (y > limiteInferiorMapa) {
        x = chefao.posicaoInicial.x;
        y = chefao.posicaoInicial.y - 10.0f;
    }

    return (Vector2){ x, y };
}

static void atualizaAtivacaoBoss() {
    if (bossMovendo || bossPreparando) return;

    float distanciaInicio = 260.0f;
    float diferencaX = personagem.posicao.x - chefao.posicaoInicial.x;
    float absDiferencaX = diferencaX < 0 ? -diferencaX : diferencaX;
    float diferencaY = personagem.posicao.y - chefao.posicaoInicial.y;
    float absDiferencaY = diferencaY < 0 ? -diferencaY : diferencaY;

    if (absDiferencaX <= distanciaInicio && absDiferencaY <= 200.0f) {
        bossPreparando = true;
        bossCountdown = 3.0f;
    }
}

void updateBoss() {
    if (!bossAtivo || !chefao.dados.vivo) return;

    if (!bossMovendo) {
        if (!bossPreparando) {
            atualizaAtivacaoBoss();
        }
        else {
            bossCountdown -= GetFrameTime();
            if (bossCountdown <= 0.0f) {
                bossPreparando = false;
                bossMovendo = true;
                bossPodeReceberDano = true;
            }
        }
    }

    if (bossMovendo) {
        chefao.posicao = movimentaBoss(chefao.posicao);
    }
}

void desenhaBoss() {
    if (bossAtivo && chefao.dados.vivo) {
        // Desenha o Boss como um quadrado ROXO ESCURO ou outra cor de destaque de 60x60
        DrawRectangle((int)chefao.posicao.x, (int)chefao.posicao.y, chefao.largura, chefao.altura, ORANGE);
        
        // Desenha uma barra de vida pequena acima da cabeça dele para dar o clima de Boss
        DrawRectangle((int)chefao.posicao.x, (int)chefao.posicao.y - 15, chefao.largura, 6, RED);
        float vidaPercent = (float)chefao.dados.hp / 500.0f;
        DrawRectangle((int)chefao.posicao.x, (int)chefao.posicao.y - 15, (int)(chefao.largura * vidaPercent), 6, GREEN);
    }
}