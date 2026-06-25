#include "game.h"
#include "mapa.h"
#include "personagem.h"
#include "inimigo.h"
#include "boss.h"
#include "hud.h"
#include <raylib.h>
#include <string.h>
#include <stdio.h>

static void gerenciarTransicaoFases() {
    float mapaLargura = map.colunas * bloco.largura;
    bool mudouDeFase = false;

    if ((personagem.posicao.x + personagem.largura) >= (mapaLargura - 45.0f)) {
        if (faseDoJogo == FASE_VILA) {
            faseDoJogo = FASE_INICIAL;
            map.localMapa = "maps/mapaInicial.txt";
            mudouDeFase = true;
        }
        else if (faseDoJogo == FASE_INICIAL) {
            faseDoJogo = FASE_FINAL;
            map.localMapa = "maps/mapaFinal.txt";
            mudouDeFase = true;
        }
    }

    if (mudouDeFase) {
        TraceLog(LOG_INFO, ">>> TROCA DE FASE! Carregando novo mapa: %s", map.localMapa);
        unloadMapa();
        quantidadeInimigos = 0;
        bossAtivo = false;
        chefao.dados.vivo = false;

        loadMapa();
        inicializaPosicoesEntidades();
    }
}

void updateJogo() {
    updatePersonagem();
    updateInimigo();
    updateBoss();

    Rectangle rectPlayer = { personagem.posicao.x, personagem.posicao.y, (float)personagem.largura, (float)personagem.altura };
    Rectangle rectAtaque = { 0 };

    if (personagem.dados.ataque) {
        float alcanceAtaque = 40.0f;
        if (personagem.olhandoDireita) {
            rectAtaque = { personagem.posicao.x + personagem.largura, personagem.posicao.y, alcanceAtaque, (float)personagem.altura };
        } else {
            rectAtaque = { personagem.posicao.x - alcanceAtaque, personagem.posicao.y, alcanceAtaque, (float)personagem.altura };
        }
    }

    for (int i = 0; i < quantidadeInimigos; i++) {
        if (!listaInimigos[i].dados.vivo) continue;

        Rectangle rectInimigo = { listaInimigos[i].posicao.x, listaInimigos[i].posicao.y, (float)listaInimigos[i].largura, (float)listaInimigos[i].altura };

        if (personagem.dados.ataque && CheckCollisionRecs(rectAtaque, rectInimigo)) {
            listaInimigos[i].dados.hp -= personagem.dados.valorAtaque;
            personagem.dados.ataque = false;
            if (listaInimigos[i].dados.hp <= 0) {
                listaInimigos[i].dados.hp = 0;
                listaInimigos[i].dados.vivo = false;
            }

            float direcaoInimigoEmpurrado = (personagem.posicao.x < listaInimigos[i].posicao.x) ? 1.0f : -1.0f;
            listaInimigos[i].posicao.x += direcaoInimigoEmpurrado * 15.0f;
            break;
        }

        if (!personagem.dados.ataque && CheckCollisionRecs(rectPlayer, rectInimigo)) {
            int dano = 20 - personagem.dados.valorDefesa;
            if (dano < 1) dano = 1;
            personagem.dados.hp -= dano;
            if (personagem.dados.hp <= 0) {
                personagem.dados.hp = 0;
                personagem.dados.vivo = false;
            }

            float direcaoKnockback = (personagem.posicao.x < listaInimigos[i].posicao.x) ? -1.0f : 1.0f;
            personagem.posicao.x += direcaoKnockback * 20.0f;
            constantesJogo.velocidadeY = -6.0f;
            break;
        }
    }

    if (personagem.dados.habilidadeAtiva.ativo) {
        Rectangle rectHab = { personagem.dados.habilidadeAtiva.posicao.x, personagem.dados.habilidadeAtiva.posicao.y, 7, 7 };
        for (int i = 0; i < quantidadeInimigos; i++) {
            if (!listaInimigos[i].dados.vivo) continue;
            Rectangle rectInimigo = { listaInimigos[i].posicao.x, listaInimigos[i].posicao.y, (float)listaInimigos[i].largura, (float)listaInimigos[i].altura };
            if (CheckCollisionRecs(rectHab, rectInimigo)) {
                listaInimigos[i].dados.hp -= 50;
                personagem.dados.habilidadeAtiva.ativo = false;
                if (listaInimigos[i].dados.hp <= 0) {
                    listaInimigos[i].dados.hp = 0;
                    listaInimigos[i].dados.vivo = false;
                }
                break;
            }
        }
    }

    if (bossAtivo && chefao.dados.vivo && bossPodeReceberDano && personagem.dados.ataque) {
        Rectangle rectBoss = { chefao.posicao.x, chefao.posicao.y, (float)chefao.largura, (float)chefao.altura };
        if (CheckCollisionRecs(rectAtaque, rectBoss)) {
            chefao.dados.hp -= personagem.dados.valorAtaque;
            personagem.dados.ataque = false;
            if (chefao.dados.hp <= 0) {
                chefao.dados.hp = 0;
                chefao.dados.vivo = false;
            }
        }
    }

    tela.camera.target = (Vector2){ personagem.posicao.x + personagem.largura / 2.0f, personagem.posicao.y + personagem.altura / 2.0f };
    tela.camera.offset = (Vector2){ tela.largura / 2.0f, tela.altura / 2.0f };

    float mapaLargura = map.colunas * bloco.largura;
    float mapaAltura = map.linhas * bloco.altura;
    float margemH = (tela.largura / 2.0f) / tela.camera.zoom;
    float margemV = (tela.altura / 2.0f) / tela.camera.zoom;

    if (tela.camera.target.x < margemH) tela.camera.target.x = margemH;
    if (tela.camera.target.x > mapaLargura - margemH) tela.camera.target.x = mapaLargura - margemH;
    if (tela.camera.target.y < margemV) tela.camera.target.y = margemV;
    if (tela.camera.target.y > mapaAltura - margemV) tela.camera.target.y = mapaAltura - margemV;

    gerenciarTransicaoFases();
}
