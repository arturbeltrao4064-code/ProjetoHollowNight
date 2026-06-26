#ifndef MENU_H
#define MENU_H

#include "estruturas.h"

extern Estado estadoAtual;

void loadMenu();
void unloadMenu();

void updatePause();
void updateMenuPrincipal();
bool confirmacaoSaidaPauseAtiva();
int opcaoConfirmacaoSaidaPause();

void desenhaBotaoPause(int texIndex, int posIndex, int selecionado, bool desabilitado);
void desenhaBotao(int texIndex, int posIndex, int selecionado, bool desabilitado);
void desenhaMenu();
void desenhaInventario();
void desenhaFundoMenu();

#endif
