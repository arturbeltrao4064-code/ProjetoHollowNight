#ifndef MENU_H
#define MENU_H

#include "estruturas.h"

void loadMenu();//carrega imagens do menu
void unloadMenu();//descrega imagens do menu

void updateConfiguracoes();//atualiza configurações do jogo
void updatePause();//atualiza estado de pausa
void updateMenuPrincipal();//atualiza estado do menu principal

void desenhaBotaoPause(int texIndex, int posIndex, int selecionado, bool desabilitado);
void desenhaBotao(int texIndex, int posIndex, int selecionado, bool desabilitado);
void desenhaMenu();//desenha o menu principal
void desenhaInventario();//desenha o inventário
void desenhaFundoMenu();//desenha o fundo do menu

#endif
