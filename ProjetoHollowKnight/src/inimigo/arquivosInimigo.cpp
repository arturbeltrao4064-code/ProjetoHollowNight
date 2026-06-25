#include "inimigo.h"
#include <raylib.h>

void loadInimigo() {
    listaInimigos[0].imagem[0] = LoadTexture("Texturas/Inimigo/Inimigo.png");
    listaInimigos[0].imagem[1] = LoadTexture("Texturas/Inimigo/InverteInimigo.png");
}

void unloadInimigo() {
    UnloadTexture(listaInimigos[0].imagem[0]);
    UnloadTexture(listaInimigos[0].imagem[1]);
}
