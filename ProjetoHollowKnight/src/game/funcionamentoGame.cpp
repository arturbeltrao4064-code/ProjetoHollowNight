#include "game.h"
#include "mapa.h"
#include "personagem.h"
#include "inimigo.h"
#include "boss.h"
#include "hud.h"
#include <raylib.h>
#include <string.h>
#include <stdio.h>

extern float flaskCarga;
static float cooldownDanoJogador = 0.0f;

static void adicionaCargaFlask(int valor) {
    personagem.dados.flask += valor;
    if (personagem.dados.flask > 100) personagem.dados.flask = 100;
}

static void empurraInimigoParaTras(infoEntidade* inimigo, float direcao) {
    float deslocamento = 58.0f;
    float novoX = inimigo->posicao.x + direcao * deslocamento;
    float y = inimigo->posicao.y;
    float w = (float)inimigo->largura;
    float h = (float)inimigo->altura;

    float checkX = (direcao > 0) ? (novoX + w) : novoX;
    bool colideParede = blocoSolido(checkX, y + 2) || blocoSolido(checkX, y + h - 2);

    if (!colideParede) {
        inimigo->posicao.x = novoX;
    }

    // Recuo vertical mais forte para enfatizar o impacto.
    inimigo->posicao.y -= 10.0f;
}

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
        flaskCarga = 100.0f;

        loadMapa();
        inicializaPosicoesEntidades();
    }
}

void updateJogo() {
    if (cooldownDanoJogador > 0.0f) {
        cooldownDanoJogador -= GetFrameTime();
        if (cooldownDanoJogador < 0.0f) cooldownDanoJogador = 0.0f;
    }

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
                adicionaCargaFlask(25);
            }

            float direcaoInimigoEmpurrado = personagem.olhandoDireita ? 1.0f : -1.0f;
            empurraInimigoParaTras(&listaInimigos[i], direcaoInimigoEmpurrado);
            // Inimigo simples morto: recupera 25% do flask
            if (!listaInimigos[i].dados.vivo) {
                flaskCarga += 25.0f;
                if (flaskCarga > 100.0f) flaskCarga = 100.0f;
            }
            break;
        }

        if (!personagem.dados.ataque && cooldownDanoJogador <= 0.0f && CheckCollisionRecs(rectPlayer, rectInimigo)) {
            personagem.dados.hp -= 1;
            if (personagem.dados.hp < 0) personagem.dados.hp = 0;
            if (personagem.dados.hp <= 0) {
                personagem.dados.hp = 0;
                personagem.dados.vivo = false;
            }
            cooldownDanoJogador = 0.8f;

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
                listaInimigos[i].dados.hp -= 1;
                personagem.dados.habilidadeAtiva.ativo = false;
                float direcaoHab = personagem.dados.habilidadeAtiva.direcao ? 1.0f : -1.0f;
                empurraInimigoParaTras(&listaInimigos[i], direcaoHab);
                if (listaInimigos[i].dados.hp <= 0) {
                    listaInimigos[i].dados.hp = 0;
                    listaInimigos[i].dados.vivo = false;
                    adicionaCargaFlask(25);
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
