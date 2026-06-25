#include "estruturas.h"

#include <stdio.h>

infoEntidade listaInimigos[MAX_INIMIGOS] = { 0 };
int quantidadeInimigos = 0;
int proximoTunel = 1;
infoTela tela = { 0 };
infoEntidade personagem = { 0 };
infoMapa map = { 0 };
infoBloco bloco = { 0 };
infoMenu menuPrincipal = { 0 };
infoMenu menuPause = { 0 };
constantes constantesJogo = { 0 };
BossState bossState = { 0 };
Texture2D texturaAmuletos[TOTAL_AMULETOS] = { 0 };
Texture2D texturaHabilidade[2] = { 0 };

static void inicializaTela() {
    tela.largura = 1500;
    tela.altura = 800;
    snprintf(tela.titulo, sizeof(tela.titulo), "%s", "Hollow Knight");
    tela.camera = (Camera2D){ {0, 0}, {0, 0}, 0.0f, 1.0f };
}

static void inicializaPersonagemBase() {
    personagem = (infoEntidade){ 0 };
    personagem.largura = 20;
    personagem.altura = 20;
    personagem.olhandoDireita = true;

    personagem.dados.hp = 5;
    personagem.dados.hpMax = 5;
    personagem.dados.mp = 100;
    personagem.dados.flask = 0;
    personagem.dados.valorAtaque = 1;
    personagem.dados.valorDefesa = 5;
    personagem.dados.ataque = false;
    personagem.dados.vivo = true;
    personagem.dados.amuletosColetados = 0;
    personagem.dados.amuletaEquipado = -1;
}

static void inicializaMapaEBlocos() {
    map.linhas = 16;
    map.colunas = 151;
    map.localMapa = "maps/vila.txt";
    map.matrizMapa = nullptr;

    bloco.largura = 30.0f;
    bloco.altura = 30.0f;
}

static void inicializaMenus() {
    menuPrincipal = (infoMenu){ 0 };
    menuPrincipal.totalOpcoes = 4;
    menuPrincipal.opcaoSelecionada = 0;
    menuPrincipal.botoesY[0] = 300;
    menuPrincipal.botoesY[1] = 400;
    menuPrincipal.botoesY[2] = 500;
    menuPrincipal.botoesY[3] = 600;
    menuPrincipal.botaoW = 200;
    menuPrincipal.botaoH = 50;

    menuPause = (infoMenu){ 0 };
    menuPause.totalOpcoes = 5;
    menuPause.opcaoSelecionada = 0;
    menuPause.botoesY[0] = 220;
    menuPause.botoesY[1] = 290;
    menuPause.botoesY[2] = 360;
    menuPause.botoesY[3] = 430;
    menuPause.botoesY[4] = 495;
    menuPause.botaoW = 260;
    menuPause.botaoH = 55;
}

static void inicializaConstantes() {
    constantesJogo.velocidade = 8.0f;
    constantesJogo.gravidade = 0.5f;
    constantesJogo.forcaPulo = -20.0f;
    constantesJogo.velocidadeY = 0.0f;
}

static void inicializaBossState() {
    bossState = (BossState){ 0 };
    bossState.countdown = 3.0f;
    bossState.damageInterval = 1.2f;
    bossState.damageValor = 25;
}

void inicializaEstruturasGlobais() {
    quantidadeInimigos = 0;
    proximoTunel = 1;

    inicializaTela();
    inicializaPersonagemBase();
    inicializaMapaEBlocos();
    inicializaMenus();
    inicializaConstantes();
    inicializaBossState();
}
