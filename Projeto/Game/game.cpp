#include "game.h"
#include "../Mapa/mapa.h"
#include "../Personagem/personagem.h"
#include <raylib.h>
#include "../Hud/hud.h"


void loadJogo() {
    loadMapa();
    loadPersonagem();
    tela.fundoJogo = LoadTexture("Texturas/Fundos/Jogo/FundoJogo.png");
}

void unloadJogo() {
    unloadMapa();
    unloadPersonagem();
    UnloadTexture(tela.fundoJogo); 
}
void updateJogo() {
    updatePersonagem();

    tela.camera.target = {
        personagem.posicao.x + personagem.largura / 2.0f,
        personagem.posicao.y + personagem.altura / 2.0f
    };
    tela.camera.offset = {
        tela.largura / 2.0f,
        tela.altura / 2.0f
    };

    // Limita câmera nas bordas do mapa
    float mapaLargura = map.colunas * bloco.largura;
    float mapaAltura  = map.linhas  * bloco.altura;
    float margemH     = (tela.largura  / 2.0f) / tela.camera.zoom;
    float margemV     = (tela.altura   / 2.0f) / tela.camera.zoom;

    if (tela.camera.target.x < margemH)               tela.camera.target.x = margemH;
    if (tela.camera.target.x > mapaLargura - margemH) tela.camera.target.x = mapaLargura - margemH;
    if (tela.camera.target.y < margemV)               tela.camera.target.y = margemV;
    if (tela.camera.target.y > mapaAltura  - margemV) tela.camera.target.y = mapaAltura  - margemV;
}
void drawFundo() {
    float fw = (float)tela.fundoJogo.width;
    float fh = (float)tela.fundoJogo.height;
    float mapaLargura = map.colunas * bloco.largura;
    float mapaAltura  = map.linhas  * bloco.altura;

    // Repete a imagem por todo o mapa
    for (float x = 0; x < mapaLargura; x += fw) {
        for (float y = 0; y < mapaAltura; y += fh) {
            DrawTextureV(tela.fundoJogo, { x, y }, WHITE);
        }
    }
}



void drawJogo() {
    ClearBackground({ 5, 5, 20, 255 });

    BeginMode2D(tela.camera);
        drawFundo();
        desenhaMapa();
        desenhaPersonagem();
    EndMode2D();

    desenhaHud(); // ← fora da câmera, fixo na tela
}
