#ifndef INIMIGO_H
#define INIMIGO_H

#include <raylib.h>
#include "estruturas.h"

void loadInimigo();
void unloadInimigo();
void updateInimigo();
void desenhaInimigo();
Vector2 movimentaInimigo(int indice, Vector2 posicaoAtual);

#endif
