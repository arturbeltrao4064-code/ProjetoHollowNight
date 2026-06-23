#include "menu.h"
#include "../Game/game.h"
#include <raylib.h>
#include <cstdlib>

Estado estadoAtual = ESTADO_MENU;

void loadMenu() {
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
}

void unloadMenu() {
    UnloadTexture(menuPrincipal.menuImagem[0]);
    for (int i = 0; i < 5; i++)
        UnloadTexture(menuPrincipal.menuBotoes[i]);
}

void desenhaFundoMenu() {
    Rectangle source = { 0, 0, (float)menuPrincipal.menuImagem[0].width, (float)menuPrincipal.menuImagem[0].height };
    Rectangle dest   = { 0, 0, (float)tela.largura, (float)tela.altura };
    DrawTexturePro(menuPrincipal.menuImagem[0], source, dest, { 0 }, 0.0f, WHITE);
}

void desenhaBotao(int texIndex, int posIndex, int selecionado, bool desabilitado = false) {
    Texture2D tex = menuPrincipal.menuBotoes[texIndex];
    float x = tela.largura / 2.0f - menuPrincipal.botaoW / 2.0f;
    float y = menuPrincipal.botoesY[posIndex] - menuPrincipal.botaoH / 2.0f;
    Color cor = desabilitado ? DARKGRAY : WHITE;

    Rectangle source = { 0, 0, (float)tex.width, (float)tex.height };
    Rectangle dest   = { x, y, (float)menuPrincipal.botaoW, (float)menuPrincipal.botaoH };
    DrawTexturePro(tex, source, dest, { 0 }, 0.0f, cor);

    if (posIndex == selecionado && !desabilitado)
        DrawRectangleLinesEx({ x - 2, y - 2, (float)menuPrincipal.botaoW + 4, (float)menuPrincipal.botaoH + 4 }, 2, WHITE);
}

void desenhaBotaoPause(int texIndex, int posIndex, int selecionado, bool desabilitado = false) {
    Texture2D tex = menuPause.menuBotoes[texIndex];
    float x = tela.largura / 2.0f - menuPause.botaoW / 2.0f; // centralizado
    float y = menuPause.botoesY[posIndex] - menuPause.botaoH / 2.0f;
    Color cor = desabilitado ? DARKGRAY : WHITE;

    Rectangle source = { 0, 0, (float)tex.width, (float)tex.height };
    Rectangle dest   = { x, y, (float)menuPause.botaoW, (float)menuPause.botaoH };
    DrawTexturePro(tex, source, dest, { 0 }, 0.0f, cor);

    if (posIndex == selecionado && !desabilitado)
        DrawRectangleLinesEx({ x - 2, y - 2, (float)menuPause.botaoW + 4, (float)menuPause.botaoH + 4 }, 2, WHITE);
}

void updateMenuPrincipal() {
    // Navegação do menu
    // Navegação com as teclas W/S ou UP/DOWN, e seleção com ENTER
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        menuPrincipal.opcaoSelecionada = (menuPrincipal.opcaoSelecionada + 1) % menuPrincipal.totalOpcoes;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        menuPrincipal.opcaoSelecionada = (menuPrincipal.opcaoSelecionada - 1 + menuPrincipal.totalOpcoes) % menuPrincipal.totalOpcoes;

    if (menuPrincipal.opcaoSelecionada == 1) {
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) menuPrincipal.opcaoSelecionada = 2;
        else menuPrincipal.opcaoSelecionada = 0;
    }
    
    if (IsKeyPressed(KEY_ENTER)) {
        switch (menuPrincipal.opcaoSelecionada) {
            case 0: estadoAtual = ESTADO_JOGANDO;       break;
            case 1: /* Carregar — futuro */              break;
            case 2: estadoAtual = ESTADO_CONFIGURACOES; break;
            case 3: CloseWindow(); exit(0);             break;
        }
    }
    // Navegação com as as teclas de atalho (N para novo jogo, C para carregar, O para opções, S para sair)
    if(IsKeyPressed(KEY_N)){
        estadoAtual = ESTADO_JOGANDO;
    }
    if(IsKeyPressed(KEY_C)){
        // Carregar — futuro
    }
    if(IsKeyPressed(KEY_O)){
        estadoAtual = ESTADO_CONFIGURACOES;
    }
    if(IsKeyPressed(KEY_S)){
        CloseWindow(); 
        exit(0);
    }
}

void updatePause() {
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        menuPause.opcaoSelecionada = (menuPause.opcaoSelecionada + 1) % menuPause.totalOpcoes;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        menuPause.opcaoSelecionada = (menuPause.opcaoSelecionada - 1 + menuPause.totalOpcoes) % menuPause.totalOpcoes;

    if (menuPause.opcaoSelecionada == 1) {
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) menuPause.opcaoSelecionada = 2;
        else menuPause.opcaoSelecionada = 0;
    }

    if (IsKeyPressed(KEY_ESCAPE))
        estadoAtual = ESTADO_JOGANDO;

    if (IsKeyPressed(KEY_ENTER)) {
        switch (menuPause.opcaoSelecionada) {
            case 0: estadoAtual = ESTADO_JOGANDO; break;
            case 1: /* Salvar — futuro */          break;
            case 2: estadoAtual = ESTADO_MENU;    break;
            case 3: CloseWindow(); exit(0);        break;
        }
    }
}

void updateConfiguracoes() {
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        menuConfiguracoes.opcaoSelecionada = (menuConfiguracoes.opcaoSelecionada + 1) % menuConfiguracoes.totalOpcoes;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        menuConfiguracoes.opcaoSelecionada = (menuConfiguracoes.opcaoSelecionada - 1 + menuConfiguracoes.totalOpcoes) % menuConfiguracoes.totalOpcoes;
    if (IsKeyPressed(KEY_ENTER) && menuConfiguracoes.opcaoSelecionada == menuConfiguracoes.totalOpcoes - 1)
        estadoAtual = ESTADO_MENU;
    if (IsKeyPressed(KEY_ESCAPE))
        estadoAtual = ESTADO_MENU;
}

void desenhaMenu() {
    // UPDATE
    if (estadoAtual == ESTADO_MENU)
        updateMenuPrincipal();
    else if (estadoAtual == ESTADO_JOGANDO) {
        updateJogo();
        if (IsKeyPressed(KEY_ESCAPE)) {
            estadoAtual = ESTADO_PAUSADO;
            menuPause.opcaoSelecionada = 0;
        }
    }
    else if (estadoAtual == ESTADO_PAUSADO)
        updatePause();
    else if (estadoAtual == ESTADO_CONFIGURACOES)
        updateConfiguracoes();

    // DRAW
    BeginDrawing();
    ClearBackground(BLACK);

    if (estadoAtual == ESTADO_MENU) {
        desenhaFundoMenu();
        for (int i = 0; i < menuPrincipal.totalOpcoes; i++)
            desenhaBotao(i, i, menuPrincipal.opcaoSelecionada, i == 1);
        DrawText("W/S = Navegar | ENTER = Selecionar",
            tela.largura / 2 - MeasureText("W/S = Navegar | ENTER = Selecionar", 16) / 2,
            tela.altura - 30, 16, DARKGRAY);
    }
    else if (estadoAtual == ESTADO_JOGANDO) {
        drawJogo();
        const char* hint = "ESC = Pause";// Dica para pausar o jogo
        DrawText(hint, tela.largura - MeasureText(hint, 18) - 10, 10, 18, DARKGRAY);
    }
    else if (estadoAtual == ESTADO_PAUSADO) {
        drawJogo();
        DrawRectangle(0, 0, tela.largura, tela.altura, { 0, 0, 0, 120 });

        const char* titulo = "Pausado";
        DrawText(titulo, tela.largura / 2 - MeasureText(titulo, 32) / 2, 160, 32, WHITE);

        desenhaBotaoPause(0, 0, menuPause.opcaoSelecionada);
        desenhaBotaoPause(4, 1, menuPause.opcaoSelecionada, true);
        desenhaBotaoPause(1, 2, menuPause.opcaoSelecionada);
        desenhaBotaoPause(3, 3, menuPause.opcaoSelecionada);

        const char* hint = "W/S = Navegar | ENTER = Selecionar | ESC = Continuar";
        DrawText(hint, tela.largura / 2 - MeasureText(hint, 14) / 2, tela.altura - 30, 14, DARKGRAY);
    }
    else if (estadoAtual == ESTADO_CONFIGURACOES) {
        desenhaFundoMenu();
        const char* titulo = "Configuracoes";
        const char* opcoes[3] = { "Volume", "Controles e Informacoes", "Voltar" };
        DrawText(titulo, tela.largura / 2 - MeasureText(titulo, 40) / 2, 200, 40, WHITE);
        for (int i = 0; i < menuConfiguracoes.totalOpcoes; i++) {
            int fontSize = (i == menuConfiguracoes.opcaoSelecionada) ? 32 : 26;
            Color cor    = (i == menuConfiguracoes.opcaoSelecionada) ? YELLOW : WHITE;
            int x = tela.largura / 2 - MeasureText(opcoes[i], fontSize) / 2;
            DrawText(opcoes[i], x, menuConfiguracoes.botoesY[i], fontSize, cor);
        }
        DrawText("ESC = Voltar",
            tela.largura / 2 - MeasureText("ESC = Voltar", 16) / 2,
            tela.altura - 30, 16, DARKGRAY);
    }

    EndDrawing();
}