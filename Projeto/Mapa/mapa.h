#ifndef MAPA_H
#define MAPA_H

#include "../Estruturas/estruturas.h"

char** leituraMapa(infoMapa info);
void liberaMapa(char** matriz, int linhas);
void loadMapa();
void unloadMapa();
void desenhaMapa();
void preencheBordas();

#endif
