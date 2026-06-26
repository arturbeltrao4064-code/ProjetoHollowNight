#ifndef GAME_H
#define GAME_H

#include "estruturas.h"

void loadJogo();
void unloadJogo();
void updateJogo(); 
void drawJogo();
void drawFundo();
void inicializaPosicoesEntidades();
const char* caminhoMapaPorFase(FaseAtual fase);
void carregarMapaAtualComEntidades();
void resetarEstadoNovoJogo();
void atualizarFundoJogo();

extern FaseAtual faseDoJogo;

#endif  