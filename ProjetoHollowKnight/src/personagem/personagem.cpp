#include "personagem.h"
#include "mapa.h"
#include <raylib.h>

void loadPersonagem() {
    personagem.imagem[0] = LoadTexture("Texturas/Personagem/Personagem.png");
    personagem.imagem[1] = LoadTexture("Texturas/Personagem/InvertePersonagem.png");
    inicializaAmuletos();
}

void unloadPersonagem() {
    UnloadTexture(personagem.imagem[0]);
    UnloadTexture(personagem.imagem[1]);
}

void updatePersonagem() {
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) personagem.olhandoDireita = true;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  personagem.olhandoDireita = false;

    if (IsKeyPressed(KEY_X) && !personagem.dados.ataque) {
        personagem.dados.ataque = true;
        static float tempoAtaque = 0.2f;
    }

    if (personagem.dados.ataque) {
        static float tempoAtaque = 0.0f;
        tempoAtaque -= GetFrameTime();
        if (tempoAtaque <= 0.0f) {
            personagem.dados.ataque = false;
        }
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
}
