#include "personagem.h"
#include "../Mapa/mapa.h"
#include <raylib.h>

void loadPersonagem() {
    personagem.imagem[0] = LoadTexture("Texturas/Personagem/Personagem.png");
    personagem.imagem[1] = LoadTexture("Texturas/Personagem/InvertePersonagem.png");
}

void unloadPersonagem() {
    UnloadTexture(personagem.imagem[0]);
    UnloadTexture(personagem.imagem[1]);
}

void updatePersonagem() {
    personagem.posicao = movimentaPersonagem(personagem.posicao);
}

void desenhaPersonagem() {
    DrawTextureEx(personagem.imagem[0], personagem.posicao, 0, 0.25f, WHITE);
}

// Retorna true se o bloco na posição (px, py) do mundo é sólido
static bool blocoSolido(float px, float py) {
    int col = (int)(px / bloco.largura);
    int lin = (int)(py / bloco.altura);

    // Clamp para não sair da matriz
    if (col < 0) col = 0;
    if (col >= map.colunas) col = map.colunas - 1;
    if (lin < 0) lin = 0;
    if (lin >= map.linhas)  lin = map.linhas  - 1;

    char c = map.matrizMapa[lin][col];
    return c == 'P';
}

Vector2 movimentaPersonagem(Vector2 posicaoAtual) {
    const float velocidade = 5.0f;
    const float gravidade  = 0.5f;
    const float forcaPulo  = -10.0f;
    static float velocidadeY = 0.0f;

    float x = posicaoAtual.x;
    float y = posicaoAtual.y;
    float w = (float)personagem.largura;
    float h = (float)personagem.altura;

    // --- MOVIMENTO HORIZONTAL ---
    float dx = 0;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) dx =  velocidade;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  dx = -velocidade;

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

    // --- GRAVIDADE E MOVIMENTO VERTICAL ---
    velocidadeY += gravidade;
    y += velocidadeY;

    // Colisão com o chão (borda inferior esquerda e direita)
    if (velocidadeY >= 0) {
        bool colideChaoEsq = blocoSolido(x + 2,       y + h);
        bool colideChaoDir = blocoSolido(x + w - 2,   y + h);

        if (colideChaoEsq || colideChaoDir) {
            int lin = (int)((y + h) / bloco.altura);
            y = lin * bloco.altura - h - 0.1f;
            velocidadeY = 0;
        }
    }

    // Colisão com o teto (borda superior esquerda e direita)
    if (velocidadeY < 0) {
        bool colideTeto1 = blocoSolido(x + 2,     y);
        bool colideTeto2 = blocoSolido(x + w - 2, y);

        if (colideTeto1 || colideTeto2) {
            int lin = (int)(y / bloco.altura);
            y = (lin + 1) * bloco.altura + 0.1f;
            velocidadeY = 0;
        }
    }

    // --- PULO (só quando está no chão) ---
    bool noChaoEsq = blocoSolido(x + 2,     y + h + 1);
    bool noChaoDir = blocoSolido(x + w - 2, y + h + 1);
    bool noChao    = noChaoEsq || noChaoDir;

    if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_SPACE)) && noChao) {
        velocidadeY = forcaPulo;
    }

    return { x, y };
}