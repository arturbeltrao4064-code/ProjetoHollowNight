#ifndef GAME_H
#define GAME_H

#include "estruturas.h"

void loadJogo();
void unloadJogo();
void updateJogo(); 
void drawJogo();
void drawFundo();

extern FaseAtual faseDoJogo;

#endif  