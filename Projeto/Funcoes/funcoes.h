#ifndef FUNCOES_H
#define FUNCOES_H

#include "C:/ProjetosRAYLIB/ProjetoHollowNight/Projeto/Estruturas/estruturas.h"

Vector2 movimentaPersoangem(Vector2);

char** leituraMapa(infoMapa info);

void liberaMapa(char** matriz, int linhas);

void startJogo(void);

void desenhaMenu(void);

void desenhaMapa(void);

void loadArquivos(void);

void unloadArquivos(void);
#endif