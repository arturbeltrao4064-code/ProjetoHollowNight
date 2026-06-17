#include "game.h"
#include "../Mapa/mapa.h"
#include "../Personagem/personagem.h"
#include <raylib.h>

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
    // Fora do BeginMode2D — desenha direto na tela
    Rectangle source = { 0, 0, (float)tela.fundoJogo.width, (float)tela.fundoJogo.height };
    Rectangle dest   = { 0, 0, (float)tela.largura, (float)tela.altura };
    DrawTexturePro(tela.fundoJogo, source, dest, { 0, 0 }, 0.0f, WHITE);
}

void drawJogo() {
    ClearBackground({ 5, 5, 20, 255 });

    drawFundo();              // ← fora da câmera, cobre a tela toda na escala certa

    BeginMode2D(tela.camera);
        desenhaMapa();
        desenhaPersonagem();
    EndMode2D();
}