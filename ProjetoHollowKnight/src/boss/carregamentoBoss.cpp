#include "boss.h"
#include <raylib.h>

static Texture2D texturaBossDireita = { 0 };
static Texture2D texturaBossEsquerda = { 0 };
static bool bossTemTexturas = false;

void loadBoss() {
    bossState.velY = 0.0f;
    bossState.countdown = 3.0f;
    bossState.movendo = false;
    bossState.preparando = false;
    bossState.podeReceberDano = false;
    bossState.jumpCooldown = 0.0f;
    bossState.damageCooldown = 0.0f;
    bossState.entidade.olhandoDireita = true;

    if (FileExists("assets/Boss/bossDireita.png") && FileExists("assets/Boss/bossEsquerda.png")) {
        texturaBossDireita = LoadTexture("assets/Boss/bossDireita.png");
        texturaBossEsquerda = LoadTexture("assets/Boss/bossEsquerda.png");
        bossState.entidade.imagem[0] = texturaBossDireita;
        bossState.entidade.imagem[1] = texturaBossEsquerda;
        bossTemTexturas = true;
    } else {
        bossTemTexturas = false;
    }
}

void unloadBoss() {
    if (bossTemTexturas) {
        UnloadTexture(texturaBossDireita);
        UnloadTexture(texturaBossEsquerda);
        bossTemTexturas = false;
    }
}
