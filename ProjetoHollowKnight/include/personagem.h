#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <raylib.h>
#include "estruturas.h"

void loadPersonagem();
void unloadPersonagem();
void updatePersonagem();
void desenhaPersonagem();
void verificaColisaoAmuletos();
void coletaAmuleto(TipoAmuleto tipo);
void inicializaAmuletos();
void coletaHabilidade();
void verificaColisaoHabilidades();
void disparaHabilidade();
void atualizaHabilidade();
void equipaAmuleto(int tipo);
Vector2 movimentaPersonagem(Vector2 posicaoAtual);

extern float flaskCarga;
extern float avisoVidaCheia;

#endif
