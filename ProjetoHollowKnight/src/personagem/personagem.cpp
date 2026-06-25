#include "personagem.h"
#include "mapa.h"
#include <raylib.h>

static float tempoAtaque = 0.0f;
static float acumuladorRegenMP = 0.0f;
static float flaskTempoSegurando = 0.0f;
static bool flask1HitDado = false;

float flaskCarga = 100.0f;
float avisoVidaCheia = 0.0f;
static float tempoCanalizandoCura = 0.0f;

void loadPersonagem() {
    inicializaAmuletos();
    flaskCarga = 100.0f;
    avisoVidaCheia = 0.0f;
    flaskTempoSegurando = 0.0f;
    flask1HitDado = false;
}

void unloadPersonagem() {
}

void updatePersonagem() {
    bool indoDireita = IsKeyDown(KEY_RIGHT);
    bool indoEsquerda = IsKeyDown(KEY_LEFT);

    if (indoDireita) personagem.olhandoDireita = true;
    if (indoEsquerda) personagem.olhandoDireita = false;

    if (IsKeyPressed(KEY_X) && !personagem.dados.ataque) {
        personagem.dados.ataque = true;
        tempoAtaque = 0.20f;
    }

    if (personagem.dados.ataque) {
        tempoAtaque -= GetFrameTime();
        if (tempoAtaque <= 0.0f) {
            personagem.dados.ataque = false;
            tempoAtaque = 0.0f;
        }
    }

    if (personagem.dados.mp < 100) {
        acumuladorRegenMP += GetFrameTime() * 1.0f;
        while (acumuladorRegenMP >= 1.0f && personagem.dados.mp < 100) {
            personagem.dados.mp += 1;
            acumuladorRegenMP -= 1.0f;
        }
    } else {
        acumuladorRegenMP = 0.0f;
    }

    if (IsKeyDown(KEY_A)) {
        tempoCanalizandoCura += GetFrameTime();
    }

    if (IsKeyReleased(KEY_A)) {
        int hitsPorTempo = (int)(tempoCanalizandoCura / 3.0f);
        if (hitsPorTempo > 2) hitsPorTempo = 2;

        int hitsFaltando = 5 - personagem.dados.hp;
        int hitsPorFlask = personagem.dados.flask / 50;
        int hitsCurados = hitsPorTempo;
        if (hitsCurados > hitsFaltando) hitsCurados = hitsFaltando;
        if (hitsCurados > hitsPorFlask) hitsCurados = hitsPorFlask;

        if (hitsCurados > 0) {
            personagem.dados.hp += hitsCurados;
            personagem.dados.flask -= hitsCurados * 50;
            if (personagem.dados.flask < 0) personagem.dados.flask = 0;
            if (personagem.dados.hp > 5) personagem.dados.hp = 5;
        }

        tempoCanalizandoCura = 0.0f;
    }

    personagem.posicao = movimentaPersonagem(personagem.posicao);
    verificaColisaoAmuletos();
    verificaColisaoHabilidades();
    disparaHabilidade();
    atualizaHabilidade();
}

void desenhaPersonagem() {
    Color corAtual = personagem.dados.ataque ? PINK : GREEN;
    DrawRectangle((int)personagem.posicao.x, (int)personagem.posicao.y, personagem.largura, personagem.altura, corAtual);

    if (personagem.dados.ataque) {
        int espadaLargura = personagem.largura;
        int espadaAltura = personagem.altura / 4;
        if (espadaAltura < 3) espadaAltura = 3;

        int espadaY = (int)personagem.posicao.y + (personagem.altura / 2) - (espadaAltura / 2);
        int espadaX = personagem.olhandoDireita
            ? (int)personagem.posicao.x + personagem.largura
            : (int)personagem.posicao.x - espadaLargura;

        DrawRectangle(espadaX, espadaY, espadaLargura, espadaAltura, YELLOW);
    }
}
