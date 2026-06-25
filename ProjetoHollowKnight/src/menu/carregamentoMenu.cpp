#include "menu.h"

void loadMenu() {
    // Comentado temporariamente para não depender dos arquivos de imagem no disco
    /*
    menuPrincipal.menuImagem[0] = LoadTexture("Texturas/Fundos/Menu/FundoMenu.png");
    menuPrincipal.menuBotoes[0] = LoadTexture("Texturas/Fundos/Menu/BotaoJogar.png");
    menuPrincipal.menuBotoes[1] = LoadTexture("Texturas/Fundos/Menu/BotaoCarregar.png");
    menuPrincipal.menuBotoes[2] = LoadTexture("Texturas/Fundos/Menu/BotaoOpcoes.png");
    menuPrincipal.menuBotoes[3] = LoadTexture("Texturas/Fundos/Menu/BotaoSair.png");
    menuPrincipal.menuBotoes[4] = LoadTexture("Texturas/Fundos/Menu/BotaoSalvar.png");
    
    menuPause.menuImagem[0] = LoadTexture("Texturas/Fundos/Menu/FundoMenu.png");
    menuPause.menuBotoes[0] = LoadTexture("Texturas/Fundos/Menu/BotaoJogar.png");
    menuPause.menuBotoes[1] = LoadTexture("Texturas/Fundos/Menu/BotaoCarregar.png");
    menuPause.menuBotoes[2] = LoadTexture("Texturas/Fundos/Menu/BotaoOpcoes.png");
    menuPause.menuBotoes[3] = LoadTexture("Texturas/Fundos/Menu/BotaoSair.png");
    menuPause.menuBotoes[4] = LoadTexture("Texturas/Fundos/Menu/BotaoSalvar.png");
    
    menuConfiguracoes.menuImagem[0] = LoadTexture("Texturas/Fundos/Menu/FundoMenu.png");
    menuConfiguracoes.menuBotoes[0] = LoadTexture("Texturas/Fundos/Menu/BotaoJogar.png");
    menuConfiguracoes.menuBotoes[1] = LoadTexture("Texturas/Fundos/Menu/BotaoCarregar.png");
    menuConfiguracoes.menuBotoes[2] = LoadTexture("Texturas/Fundos/Menu/BotaoOpcoes.png");
    menuConfiguracoes.menuBotoes[3] = LoadTexture("Texturas/Fundos/Menu/BotaoSair.png");
    menuConfiguracoes.menuBotoes[4] = LoadTexture("Texturas/Fundos/Menu/BotaoSalvar.png");
    */
}

void unloadMenu() {
    // Comentado temporariamente já que não há texturas na VRAM
    /*
    UnloadTexture(menuPrincipal.menuImagem[0]);
    for (int i = 0; i < 5; i++)
        UnloadTexture(menuPrincipal.menuBotoes[i]);
    */
}
