#include "game.h"
#include "mapa.h"
#include "personagem.h"
#include "inimigo.h"
#include "boss.h"
#include "hud.h"
#include <raylib.h>

void drawFundo() {
    float fw = (float)tela.fundoJogo.width;
    float fh = (float)tela.fundoJogo.height;
    float mapaLargura = map.colunas * bloco.largura;
    float mapaAltura = map.linhas * bloco.altura;

    for (float x = 0; x < mapaLargura; x += fw) {
        for (float y = 0; y < mapaAltura; y += fh) {
            DrawTextureV(tela.fundoJogo, (Vector2){ x, y }, WHITE);
        }
    }
}

void desenhaHabilidade() {
    if (personagem.dados.habilidadeAtiva.ativo) {
        int tamHab = personagem.largura / 3;
        DrawRectangle((int)personagem.dados.habilidadeAtiva.posicao.x,
                      (int)personagem.dados.habilidadeAtiva.posicao.y,
                      tamHab, tamHab, ORANGE);
    }
}

void drawJogo() {
    ClearBackground(PURPLE);

    BeginMode2D(tela.camera);
        desenhaMapa();
        desenhaPersonagem();
        desenhaHabilidade();
        desenhaInimigo();
        desenhaBoss();
    EndMode2D();

    desenhaHud(personagem.dados.hp, personagem.dados.mp, personagem.dados.valorAtaque, personagem.dados.valorDefesa);
}
