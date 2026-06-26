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
        int tamHab = personagem.largura * 3;
        Texture2D texturaAtual = personagem.dados.habilidadeAtiva.direcao ? texturaHabilidade[0] : texturaHabilidade[1];
        if (texturaAtual.width > 0 && texturaAtual.height > 0) {
            Rectangle src = { 0.0f, 0.0f, (float)texturaAtual.width, (float)texturaAtual.height };
            Rectangle dst = { personagem.dados.habilidadeAtiva.posicao.x - tamHab / 2,
                              personagem.dados.habilidadeAtiva.posicao.y + personagem.altura / 2 - tamHab / 2,
                              (float)tamHab, (float)tamHab };
            DrawTexturePro(texturaAtual, src, dst, { 0, 0 }, 0.0f, WHITE);
        }
    }
}

void drawJogo() {
    ClearBackground(PURPLE);

    BeginMode2D(tela.camera);
        drawFundo();
        desenhaMapa();
        desenhaPersonagem();
        desenhaHabilidade();
        desenhaInimigo();
        desenhaBoss();
    EndMode2D();

    desenhaHud(personagem.dados.hp, personagem.dados.mp, personagem.dados.valorAtaque, personagem.dados.valorDefesa);
}
