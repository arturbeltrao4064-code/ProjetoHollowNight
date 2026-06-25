#include "boss.h"
#include "personagem.h"
#include "mapa.h"
#include <raylib.h>

static void aplicaKnockbackJogador(float forcaX, float forcaY) {
    float centroPlayer = personagem.posicao.x + personagem.largura * 0.5f;
    float centroBoss = bossState.entidade.posicao.x + bossState.entidade.largura * 0.5f;
    float direcao = (centroPlayer < centroBoss) ? -1.0f : 1.0f;
    float novoX = personagem.posicao.x + direcao * forcaX;

    float checkX = (direcao > 0.0f) ? (novoX + personagem.largura) : novoX;
    bool colideParede = blocoSolido(checkX, personagem.posicao.y + 2.0f)
                    || blocoSolido(checkX, personagem.posicao.y + personagem.altura - 2.0f);
    if (!colideParede) {
        personagem.posicao.x = novoX;
    }

    constantesJogo.velocidadeY = -forcaY;
}

static void resolveColisaoCorpoBossJogador() {
    Rectangle playerRect = { personagem.posicao.x, personagem.posicao.y, (float)personagem.largura, (float)personagem.altura };
    Rectangle bossRect = { bossState.entidade.posicao.x, bossState.entidade.posicao.y, (float)bossState.entidade.largura, (float)bossState.entidade.altura };

    if (!CheckCollisionRecs(bossRect, playerRect)) return;

    Rectangle inter = GetCollisionRec(bossRect, playerRect);
    if (inter.width <= 0.0f || inter.height <= 0.0f) return;

    float centroPlayerX = personagem.posicao.x + personagem.largura * 0.5f;
    float centroBossX = bossState.entidade.posicao.x + bossState.entidade.largura * 0.5f;

    // Separa pelo menor eixo de intersecao para evitar "grudar" no boss.
    if (inter.width < inter.height) {
        float direcao = (centroPlayerX < centroBossX) ? -1.0f : 1.0f;
        float novoX = personagem.posicao.x + direcao * (inter.width + 2.0f);
        float checkX = (direcao > 0.0f) ? (novoX + personagem.largura) : novoX;

        bool colideParede = blocoSolido(checkX, personagem.posicao.y + 2.0f)
                        || blocoSolido(checkX, personagem.posicao.y + personagem.altura - 2.0f);
        if (!colideParede) {
            personagem.posicao.x = novoX;
        }
    } else {
        // Fallback vertical: coloca o jogador acima do boss quando sobreposicao vertical predomina.
        personagem.posicao.y = bossState.entidade.posicao.y - personagem.altura - 0.1f;
        if (constantesJogo.velocidadeY > 0.0f) {
            constantesJogo.velocidadeY = 0.0f;
        }
    }
}

static void atualizaAtivacaoBoss() {
    if (bossState.movendo || bossState.preparando) return;

    float distanciaInicio = 260.0f;
    float diferencaX = personagem.posicao.x - bossState.entidade.posicaoInicial.x;
    float absDiferencaX = diferencaX < 0 ? -diferencaX : diferencaX;
    float diferencaY = personagem.posicao.y - bossState.entidade.posicaoInicial.y;
    float absDiferencaY = diferencaY < 0 ? -diferencaY : diferencaY;

    if (absDiferencaX <= distanciaInicio && absDiferencaY <= 200.0f) {
        bossState.preparando = true;
        bossState.countdown = 3.0f;
    }
}

static void atualizarCooldownDanoBoss() {
    if (bossState.damageCooldown > 0.0f) {
        bossState.damageCooldown -= GetFrameTime();
        if (bossState.damageCooldown < 0.0f) bossState.damageCooldown = 0.0f;
    }
}

static void tentarAplicarDanoBossNoJogador() {
    if (bossState.damageCooldown > 0.0f) return;

    Rectangle areaAtaque = {
        bossState.entidade.posicao.x - 40.0f,
        bossState.entidade.posicao.y - 40.0f,
        bossState.entidade.largura + 80.0f,
        bossState.entidade.altura + 80.0f
    };
    Rectangle playerRect = { personagem.posicao.x, personagem.posicao.y, (float)personagem.largura, (float)personagem.altura };

    if (!CheckCollisionRecs(areaAtaque, playerRect)) return;

    personagem.dados.hp -= 1;
    if (personagem.dados.hp < 0) personagem.dados.hp = 0;
    if (personagem.dados.hp == 0) {
        personagem.dados.vivo = false;
    }

    aplicaKnockbackJogador(28.0f, 6.0f);
    bossState.damageCooldown = bossState.damageInterval;
}

void updateBoss() {
    if (!bossState.ativo || !bossState.entidade.dados.vivo) return;

    if (!bossState.movendo) {
        if (!bossState.preparando) {
            atualizaAtivacaoBoss();
        }
        else {
            bossState.countdown -= GetFrameTime();
            if (bossState.countdown <= 0.0f) {
                bossState.preparando = false;
                bossState.movendo = true;
                bossState.podeReceberDano = true;
            }
        }
    }

    if (bossState.movendo) {
        bossState.entidade.posicao = movimentaBoss(bossState.entidade.posicao);
        resolveColisaoCorpoBossJogador();
        atualizarCooldownDanoBoss();
        tentarAplicarDanoBossNoJogador();
    }
}

void desenhaBoss() {
    if (bossState.ativo && bossState.entidade.dados.vivo) {
        Texture2D texturaAtual = bossState.entidade.olhandoDireita ? bossState.entidade.imagem[0] : bossState.entidade.imagem[1];
        if (texturaAtual.width > 0 && texturaAtual.height > 0) {
            Rectangle src = { 0.0f, 0.0f, (float)texturaAtual.width, (float)texturaAtual.height };
            Rectangle dst = { bossState.entidade.posicao.x, bossState.entidade.posicao.y, (float)bossState.entidade.largura, (float)bossState.entidade.altura };
            Vector2 origin = { 0.0f, 0.0f };
            DrawTexturePro(texturaAtual, src, dst, origin, 0.0f, WHITE);
        } else {
            DrawRectangle((int)bossState.entidade.posicao.x, (int)bossState.entidade.posicao.y, bossState.entidade.largura, bossState.entidade.altura, ORANGE);
        }
        DrawRectangle((int)bossState.entidade.posicao.x, (int)bossState.entidade.posicao.y - 15, bossState.entidade.largura, 6, RED);
        float hpMaxBoss = (bossState.entidade.dados.hpMax > 0) ? (float)bossState.entidade.dados.hpMax : 1.0f;
        float vidaPercent = (float)bossState.entidade.dados.hp / hpMaxBoss;
        DrawRectangle((int)bossState.entidade.posicao.x, (int)bossState.entidade.posicao.y - 15, (int)(bossState.entidade.largura * vidaPercent), 6, GREEN);
    }
}
