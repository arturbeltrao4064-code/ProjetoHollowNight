#include "inimigo.h"
#include <raylib.h>

static Texture2D texturaInimigoDireita = { 0 };
static Texture2D texturaInimigoEsquerda = { 0 };

void loadInimigo() {
    texturaInimigoDireita = LoadTexture("assets/Inimigos/inimigoDireita.png");
    texturaInimigoEsquerda = LoadTexture("assets/Inimigos/inimigoEsquerda.png");

    for (int i = 0; i < MAX_INIMIGOS; i++) {
        listaInimigos[i].imagem[0] = texturaInimigoDireita;
        listaInimigos[i].imagem[1] = texturaInimigoEsquerda;
    }
}

void unloadInimigo() {
    UnloadTexture(texturaInimigoDireita);
    UnloadTexture(texturaInimigoEsquerda);
}
