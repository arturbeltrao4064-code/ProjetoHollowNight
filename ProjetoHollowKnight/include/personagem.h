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
Vector2 movimentaPersonagem(Vector2 posicaoAtual);

#endif
