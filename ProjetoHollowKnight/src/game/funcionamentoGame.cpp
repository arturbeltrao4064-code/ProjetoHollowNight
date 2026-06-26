#include "game.h"
#include "mapa.h"
#include "personagem.h"
#include "inimigo.h"
#include "boss.h"
#include "hud.h"
#include "menu.h"
#include <raylib.h>
#include <string.h>
#include <stdio.h>

extern float flaskCarga;
static float cooldownDanoJogador = 0.0f;

static void adicionaCargaFlask(int valor) {
    personagem.dados.flask += valor;
    if (personagem.dados.flask > 100) personagem.dados.flask = 100;
    flaskCarga = (float)personagem.dados.flask;
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
}

static Rectangle getRetanguloAtaqueJogador() {
    Rectangle rectAtaque = { 0 };
    if (!personagem.dados.ataque) return rectAtaque;

    float alcanceAtaque = 40.0f;
    if (personagem.olhandoDireita) {
        rectAtaque = { personagem.posicao.x + personagem.largura, personagem.posicao.y, alcanceAtaque, (float)personagem.altura };
    } else {
        rectAtaque = { personagem.posicao.x - alcanceAtaque, personagem.posicao.y, alcanceAtaque, (float)personagem.altura };
    }
    return rectAtaque;
}

static bool processaCombateContraInimigo(const Rectangle& rectPlayer, const Rectangle& rectAtaque, infoEntidade* inimigo) {
    Rectangle rectInimigo = { inimigo->posicao.x, inimigo->posicao.y, (float)inimigo->largura, (float)inimigo->altura };

    if (personagem.dados.ataque && CheckCollisionRecs(rectAtaque, rectInimigo)) {
        inimigo->dados.hp -= personagem.dados.valorAtaque;
        personagem.dados.ataque = false;
        if (inimigo->dados.hp <= 0) {
            inimigo->dados.hp = 0;
            inimigo->dados.vivo = false;
            adicionaCargaFlask(25);
        }

        float direcaoInimigoEmpurrado = personagem.olhandoDireita ? 1.0f : -1.0f;
        empurraInimigoParaTras(inimigo, direcaoInimigoEmpurrado);
        return true;
    }

    if (!personagem.dados.ataque && cooldownDanoJogador <= 0.0f && CheckCollisionRecs(rectPlayer, rectInimigo)) {
        personagem.dados.hp -= 1;
        if (personagem.dados.hp < 0) personagem.dados.hp = 0;
        if (personagem.dados.hp <= 0) {
            personagem.dados.hp = 0;
            personagem.dados.vivo = false;
        }
        cooldownDanoJogador = 0.8f;

        float direcaoKnockback = (personagem.posicao.x < inimigo->posicao.x) ? -1.0f : 1.0f;
        personagem.posicao.x += direcaoKnockback * 20.0f;
        constantesJogo.velocidadeY = -6.0f;
        return true;
    }

    return false;
}

static void processaCombateInimigos(const Rectangle& rectPlayer, const Rectangle& rectAtaque) {
    for (int i = 0; i < quantidadeInimigos; i++) {
        if (!listaInimigos[i].dados.vivo) continue;
        if (processaCombateContraInimigo(rectPlayer, rectAtaque, &listaInimigos[i])) {
            break;
        }
    }
}

static void processaColisaoHabilidade() {
    if (!personagem.dados.habilidadeAtiva.ativo) return;

    Rectangle rectHab = { personagem.dados.habilidadeAtiva.posicao.x, personagem.dados.habilidadeAtiva.posicao.y, 7, 7 };
    for (int i = 0; i < quantidadeInimigos; i++) {
        if (!listaInimigos[i].dados.vivo) continue;

        Rectangle rectInimigo = { listaInimigos[i].posicao.x, listaInimigos[i].posicao.y, (float)listaInimigos[i].largura, (float)listaInimigos[i].altura };
        if (!CheckCollisionRecs(rectHab, rectInimigo)) continue;

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

static void processaDanoNoBoss(const Rectangle& rectAtaque) {
    if (!(bossState.ativo && bossState.entidade.dados.vivo && bossState.podeReceberDano && personagem.dados.ataque)) return;

    Rectangle rectBoss = { bossState.entidade.posicao.x, bossState.entidade.posicao.y, (float)bossState.entidade.largura, (float)bossState.entidade.altura };
    if (!CheckCollisionRecs(rectAtaque, rectBoss)) return;

    bossState.entidade.dados.hp -= personagem.dados.valorAtaque;
    personagem.dados.ataque = false;
    if (bossState.entidade.dados.hp <= 0) {
        bossState.entidade.dados.hp = 0;
        bossState.entidade.dados.vivo = false;
    }
}

static void atualizaCamera() {
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
}

static void trocarMapa(const char* novoMapa) {
    map.localMapa = novoMapa;
    TraceLog(LOG_INFO, ">>> TROCA DE FASE! Carregando novo mapa: %s", map.localMapa);

    unloadMapa();
    quantidadeInimigos = 0;
    bossState.ativo = false;
    bossState.entidade.dados.vivo = false;
    bossState.entidade.dados.hp = 0;
    flaskCarga = 100.0f;

    loadMapa();
    inicializaPosicoesEntidades();
    atualizarFundoJogo();
}

static bool podeSairDaFaseAtual() {
    if (faseDoJogo == FASE_VILA) return true;
    return bossState.ativo && !bossState.entidade.dados.vivo;
}

static void gerenciarTransicaoFases() {
    float mapaLargura = map.colunas * bloco.largura;
    bool chegouDireita = (personagem.posicao.x + personagem.largura) >= (mapaLargura - 45.0f);

    if (faseDoJogo == FASE_VILA && chegouDireita) {
        if (proximoTunel == 1) {
            faseDoJogo = FASE_INICIAL;
            trocarMapa("maps/tunel1.txt");
        } else if (proximoTunel == 2) {
            faseDoJogo = FASE_FINAL;
            trocarMapa("maps/tunel2.txt");
        } else if (proximoTunel == 3) {
            faseDoJogo = FASE_TUNEL3;
            trocarMapa("maps/tunel3.txt");
        }
        return;
    }

    if (chegouDireita && !podeSairDaFaseAtual()) {
        return;
    }

    if (faseDoJogo == FASE_INICIAL && chegouDireita) {
        if (proximoTunel < 2) proximoTunel = 2;
        faseDoJogo = FASE_VILA;
        trocarMapa("maps/vila.txt");
        return;
    }

    if (faseDoJogo == FASE_FINAL && chegouDireita) {
        if (proximoTunel < 3) proximoTunel = 3;
        faseDoJogo = FASE_VILA;
        trocarMapa("maps/vila.txt");
        return;
    }

    if (faseDoJogo == FASE_TUNEL3 && chegouDireita) {
        proximoTunel = 4;
        faseDoJogo = FASE_VILA;
        trocarMapa("maps/vila.txt");
        estadoAtual = ESTADO_FIM;
        return;
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
    Rectangle rectAtaque = getRetanguloAtaqueJogador();

    processaCombateInimigos(rectPlayer, rectAtaque);
    processaColisaoHabilidade();
    processaDanoNoBoss(rectAtaque);
    atualizaCamera();

    gerenciarTransicaoFases();
}
