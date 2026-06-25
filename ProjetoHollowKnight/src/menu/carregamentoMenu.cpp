#include "menu.h"

static const char* MENU_FUNDO = "assets/Menu/menuFundo.png";
static const char* MENU_BOTOES[7] = {
    "assets/Menu/botoes/botaoNovoJogo.png",
    "assets/Menu/botoes/botaoCarregar.png",
    "assets/Menu/botoes/botaoAjuda.png",
    "assets/Menu/botoes/botaoSair.png",
    "assets/Menu/botoes/botaoMenuPrincipal.png",
    "assets/Menu/botoes/botaoInventario.png",
    "assets/Menu/botoes/botaoSalvar.png"
};

void loadMenu() {
    menuPrincipal.menuImagem[0] = LoadTexture(MENU_FUNDO);
    menuPause.menuImagem[0] = menuPrincipal.menuImagem[0];

    for (int i = 0; i < 7; i++) {
        menuPrincipal.menuBotoes[i] = LoadTexture(MENU_BOTOES[i]);
    }

    menuPause.menuBotoes[0] = menuPrincipal.menuBotoes[0];
    menuPause.menuBotoes[1] = menuPrincipal.menuBotoes[5];
    menuPause.menuBotoes[2] = menuPrincipal.menuBotoes[4];
    menuPause.menuBotoes[3] = menuPrincipal.menuBotoes[6];
    menuPause.menuBotoes[4] = menuPrincipal.menuBotoes[3];

    texturaAmuletos[0] = LoadTexture("assets/Objetos/amuleto1.png");
    texturaAmuletos[1] = LoadTexture("assets/Objetos/amuleto2.png");
    texturaAmuletos[2] = LoadTexture("assets/Objetos/amuleto3.png");
}

void unloadMenu() {
    UnloadTexture(menuPrincipal.menuImagem[0]);
    for (int i = 0; i < 7; i++) {
        UnloadTexture(menuPrincipal.menuBotoes[i]);
    }

    for (int i = 0; i < TOTAL_AMULETOS; i++) {
        UnloadTexture(texturaAmuletos[i]);
    }
}
