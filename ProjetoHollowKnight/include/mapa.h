#ifndef MAPA_H
#define MAPA_H

#include "estruturas.h"

char** leituraMapa(infoMapa info);
void liberaMapa(char** matriz, int linhas);
void loadMapa();
void unloadMapa();
void desenhaMapa();
bool blocoSolido(float px, float py);

#endif
