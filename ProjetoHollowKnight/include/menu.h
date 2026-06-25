#ifndef MENU_H
#define MENU_H

#include "estruturas.h"

extern Estado estadoAtual;

void loadMenu();
void unloadMenu();

void updateConfiguracoes();
void updatePause();
void updateMenuPrincipal();

void desenhaBotaoPause(int texIndex, int posIndex, int selecionado, bool desabilitado);
void desenhaBotao(int texIndex, int posIndex, int selecionado, bool desabilitado);
void desenhaMenu();
void desenhaInventario();
void desenhaFundoMenu();

#endif
