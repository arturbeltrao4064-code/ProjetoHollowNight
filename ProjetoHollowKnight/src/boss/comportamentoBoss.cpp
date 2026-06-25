#include "boss.h"
#include "personagem.h"
#include <raylib.h>

extern float bossVelY;
extern float bossCountdown;
extern bool bossMovendo;
extern bool bossPreparando;
extern float bossDamageCooldown;
extern float bossDamageInterval;
extern int bossDamageValor;

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
        if (bossDamageCooldown > 0.0f) {
            bossDamageCooldown -= GetFrameTime();
            if (bossDamageCooldown < 0.0f) bossDamageCooldown = 0.0f;
        }

        if (bossDamageCooldown <= 0.0f) {
            Rectangle areaAtaque = {
                chefao.posicao.x - 40.0f,
                chefao.posicao.y - 40.0f,
                chefao.largura + 80.0f,
                chefao.altura + 80.0f
            };
            Rectangle playerRect = {personagem.posicao.x, personagem.posicao.y, (float)personagem.largura, (float)personagem.altura};

            if (CheckCollisionRecs(areaAtaque, playerRect)) {
                int dano = bossDamageValor - personagem.dados.valorDefesa;
                if (dano < 1) dano = 1;
                personagem.dados.hp -= dano;
                if (personagem.dados.hp < 0) personagem.dados.hp = 0;
                bossDamageCooldown = bossDamageInterval;
            }
        }
    }
}

void desenhaBoss() {
    if (bossAtivo && chefao.dados.vivo) {
        DrawRectangle((int)chefao.posicao.x, (int)chefao.posicao.y, chefao.largura, chefao.altura, ORANGE);
        DrawRectangle((int)chefao.posicao.x, (int)chefao.posicao.y - 15, chefao.largura, 6, RED);
        float vidaPercent = (float)chefao.dados.hp / 500.0f;
        DrawRectangle((int)chefao.posicao.x, (int)chefao.posicao.y - 15, (int)(chefao.largura * vidaPercent), 6, GREEN);
    }
}
