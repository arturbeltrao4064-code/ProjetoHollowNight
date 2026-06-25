#include "personagem.h"
#include "mapa.h"
#include <raylib.h>

static float tempoAtaque = 0.0f;

void loadPersonagem() {
    personagem.imagem[0] = LoadTexture("Texturas/Personagem/Personagem.png");
    personagem.imagem[1] = LoadTexture("Texturas/Personagem/InvertePersonagem.png");
    inicializaAmuletos();
}

void unloadPersonagem() {
    UnloadTexture(personagem.imagem[0]);
    UnloadTexture(personagem.imagem[1]);
}

void updatePersonagem() {
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) personagem.olhandoDireita = true;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  personagem.olhandoDireita = false;

    if (IsKeyPressed(KEY_X) && !personagem.dados.ataque) {
        personagem.dados.ataque = true;
        tempoAtaque = 0.2f;
    }

    if (personagem.dados.ataque) {
        tempoAtaque -= GetFrameTime();
        if (tempoAtaque <= 0.0f) {
            personagem.dados.ataque = false;
        }
    }

    personagem.posicao = movimentaPersonagem(personagem.posicao);
    verificaColisaoAmuletos();
    verificaColisaoHabilidades();
    disparaHabilidade();
    atualizaHabilidade();
}

void desenhaPersonagem() {
    // SE ESTIVER ATACANDO: Fica ROSA (PINK). SE NÃO: Fica VERDE (GREEN)
    Color corAtual = personagem.dados.ataque ? PINK : GREEN;
    
    DrawRectangle((int)personagem.posicao.x, (int)personagem.posicao.y, personagem.largura, personagem.altura, corAtual);
}

Vector2 movimentaPersonagem(Vector2 posicaoAtual) {
    float x = posicaoAtual.x;
    float y = posicaoAtual.y;
    float w = (float)personagem.largura;
    float h = (float)personagem.altura;

    // --- 1. MOVIMENTO HORIZONTAL ---
    float dx = 0;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) dx =  constantesJogo.velocidade;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  dx = -constantesJogo.velocidade;

    if (dx != 0) {
        x += dx;
        // Verifica colisão lateral: topo e base do personagem
        float checkX = (dx > 0) ? (x + w) : x; // borda que vai colidir
        bool colideEsq = blocoSolido(checkX, y + 2);           // topo
        bool colideDir = blocoSolido(checkX, y + h - 2);       // base

        if (colideEsq || colideDir) {
            if (dx > 0) {
                // Empurra para a esquerda do bloco
                int col = (int)((x + w) / bloco.largura);
                x = col * bloco.largura - w - 0.1f;
            } else {
                // Empurra para a direita do bloco
                int col = (int)(x / bloco.largura);
                x = (col + 1) * bloco.largura + 0.1f;
            }
        }
    }

    // --- 2. GRAVIDADE E MOVIMENTO VERTICAL ---
    // CORRIGIDO: Agora usa 'constantesJogo.velocidadeY' em vez de 'personagem.velocidadeY'
    constantesJogo.velocidadeY += constantesJogo.gravidade;
    y += constantesJogo.velocidadeY;

    // Colisão com o chão (borda inferior esquerda e direita)
    if (constantesJogo.velocidadeY >= 0) {
        bool colideChaoEsq = blocoSolido(x + 2,       y + h);
        bool colideChaoDir = blocoSolido(x + w - 2,   y + h);

        if (colideChaoEsq || colideChaoDir) {
            int lin = (int)((y + h) / bloco.altura);
            y = lin * bloco.altura - h - 0.1f;
            constantesJogo.velocidadeY = 0; // Para de cair
        }
    }

    // Colisão com o teto (borda superior esquerda e direita)
    if (constantesJogo.velocidadeY < 0) {
        bool colideTeto1 = blocoSolido(x + 2,     y);
        bool colideTeto2 = blocoSolido(x + w - 2, y);

        if (colideTeto1 || colideTeto2) {
            int lin = (int)(y / bloco.altura);
            y = (lin + 1) * bloco.altura + 0.1f;
            constantesJogo.velocidadeY = 0; // Zera a velocidade ao bater a cabeça
        }
    }

    // --- 3. PULO (só quando está no chão) ---
    bool noChaoEsq = blocoSolido(x + 2,     y + h + 1);
    bool noChaoDir = blocoSolido(x + w - 2, y + h + 1);
    bool noChao    = noChaoEsq || noChaoDir;

    if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_SPACE)) && noChao) {
        constantesJogo.velocidadeY = constantesJogo.forcaPulo;
    }
    return {x, y};
}
