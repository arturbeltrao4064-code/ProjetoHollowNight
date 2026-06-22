#include "menu.h"
#include "../Game/game.h"
#include <raylib.h>
#include <cstdlib>

enum Estado { ESTADO_MENU, ESTADO_JOGANDO, ESTADO_PAUSADO, ESTADO_CONFIGURACOES };
Estado estadoAtual = ESTADO_MENU;

const int totalOpcoes = 4;
int opcaoSelecionada  = 0;
const int botoesY[]   = { 294, 361, 428, 495 };
const int botaoW      = 320;
const int botaoH      = 60;

const int totalPause     = 4;
int opcaoPause           = 0;
const int pauseBotoesY[] = { 220, 290, 360, 430 };
const int pauseBotaoW    = 260;
const int pauseBotaoH    = 55;

const char* opcoesConfig[] = { "Volume", "Voltar" };
const int totalConfig      = 2;
int opcaoConfig            = 0;
const int botoesConfigY[]  = { 294, 361 };

void loadMenu() {
    tela.menuImagem[0] = LoadTexture("Texturas/Fundos/Menu/FundoMenu.png");
    tela.menuBotoes[0] = LoadTexture("Texturas/Fundos/Menu/BotaoJogar.png");
    tela.menuBotoes[1] = LoadTexture("Texturas/Fundos/Menu/BotaoCarregar.png");
    tela.menuBotoes[2] = LoadTexture("Texturas/Fundos/Menu/BotaoOpcoes.png");
    tela.menuBotoes[3] = LoadTexture("Texturas/Fundos/Menu/BotaoSair.png");
    tela.menuBotoes[4] = LoadTexture("Texturas/Fundos/Menu/BotaoSalvar.png");
}

void unloadMenu() {
    UnloadTexture(tela.menuImagem[0]);
    for (int i = 0; i < 5; i++)
        UnloadTexture(tela.menuBotoes[i]);
}

static void desenhaFundoMenu() {
    Rectangle source = { 0, 0, (float)tela.menuImagem[0].width, (float)tela.menuImagem[0].height };
    Rectangle dest   = { 0, 0, (float)tela.largura, (float)tela.altura };
    DrawTexturePro(tela.menuImagem[0], source, dest, { 0 }, 0.0f, WHITE);
}

static void desenhaBotao(int texIndex, int posIndex, int selecionado, bool desabilitado = false) {
    Texture2D tex = tela.menuBotoes[texIndex];
    float x = tela.largura / 2.0f - botaoW / 2.0f;
    float y = botoesY[posIndex] - botaoH / 2.0f;
    Color cor = desabilitado ? DARKGRAY : WHITE;

    Rectangle source = { 0, 0, (float)tex.width, (float)tex.height };
    Rectangle dest   = { x, y, (float)botaoW, (float)botaoH };
    DrawTexturePro(tex, source, dest, { 0 }, 0.0f, cor);

    if (posIndex == selecionado && !desabilitado)
        DrawRectangleLinesEx({ x - 2, y - 2, (float)botaoW + 4, (float)botaoH + 4 }, 2, WHITE);
}

static void desenhaBotaoPause(int texIndex, int posIndex, int selecionado, bool desabilitado = false) {
    Texture2D tex = tela.menuBotoes[texIndex];
    float x = tela.largura / 2.0f - pauseBotaoW / 2.0f; // centralizado
    float y = pauseBotoesY[posIndex] - pauseBotaoH / 2.0f;
    Color cor = desabilitado ? DARKGRAY : WHITE;

    Rectangle source = { 0, 0, (float)tex.width, (float)tex.height };
    Rectangle dest   = { x, y, (float)pauseBotaoW, (float)pauseBotaoH };
    DrawTexturePro(tex, source, dest, { 0 }, 0.0f, cor);

    if (posIndex == selecionado && !desabilitado)
        DrawRectangleLinesEx({ x - 2, y - 2, (float)pauseBotaoW + 4, (float)pauseBotaoH + 4 }, 2, WHITE);
}

static void updateMenuPrincipal() {
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        opcaoSelecionada = (opcaoSelecionada + 1) % totalOpcoes;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        opcaoSelecionada = (opcaoSelecionada - 1 + totalOpcoes) % totalOpcoes;

    if (opcaoSelecionada == 1) {
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) opcaoSelecionada = 2;
        else opcaoSelecionada = 0;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        switch (opcaoSelecionada) {
            case 0: estadoAtual = ESTADO_JOGANDO;       break;
            case 1: /* Carregar — futuro */              break;
            case 2: estadoAtual = ESTADO_CONFIGURACOES; break;
            case 3: CloseWindow(); exit(0);             break;
        }
    }
}

static void updatePause() {
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        opcaoPause = (opcaoPause + 1) % totalPause;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        opcaoPause = (opcaoPause - 1 + totalPause) % totalPause;

    if (opcaoPause == 1) {
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) opcaoPause = 2;
        else opcaoPause = 0;
    }

    if (IsKeyPressed(KEY_ESCAPE))
        estadoAtual = ESTADO_JOGANDO;

    if (IsKeyPressed(KEY_ENTER)) {
        switch (opcaoPause) {
            case 0: estadoAtual = ESTADO_JOGANDO; break;
            case 1: /* Salvar — futuro */          break;
            case 2: estadoAtual = ESTADO_MENU;    break;
            case 3: CloseWindow(); exit(0);        break;
        }
    }
}

static void updateConfiguracoes() {
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        opcaoConfig = (opcaoConfig + 1) % totalConfig;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        opcaoConfig = (opcaoConfig - 1 + totalConfig) % totalConfig;
    if (IsKeyPressed(KEY_ENTER) && opcaoConfig == totalConfig - 1)
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
            opcaoPause  = 0;
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
        for (int i = 0; i < totalOpcoes; i++)
            desenhaBotao(i, i, opcaoSelecionada, i == 1);
        DrawText("W/S = Navegar | ENTER = Selecionar",
            tela.largura / 2 - MeasureText("W/S = Navegar | ENTER = Selecionar", 16) / 2,
            tela.altura - 30, 16, DARKGRAY);
    }
    else if (estadoAtual == ESTADO_JOGANDO) {
        drawJogo();
        const char* hint = "ESC = Pause";
        DrawText(hint, tela.largura - MeasureText(hint, 18) - 10, 10, 18, DARKGRAY);
    }
    else if (estadoAtual == ESTADO_PAUSADO) {
        drawJogo();
        DrawRectangle(0, 0, tela.largura, tela.altura, { 0, 0, 0, 120 });

        const char* titulo = "Pausado";
        DrawText(titulo, tela.largura / 2 - MeasureText(titulo, 32) / 2, 160, 32, WHITE);

        desenhaBotaoPause(0, 0, opcaoPause);
        desenhaBotaoPause(4, 1, opcaoPause, true);
        desenhaBotaoPause(1, 2, opcaoPause);
        desenhaBotaoPause(3, 3, opcaoPause);

        const char* hint = "W/S = Navegar | ENTER = Selecionar | ESC = Continuar";
        DrawText(hint, tela.largura / 2 - MeasureText(hint, 14) / 2, tela.altura - 30, 14, DARKGRAY);
    }
    else if (estadoAtual == ESTADO_CONFIGURACOES) {
        desenhaFundoMenu();
        const char* titulo = "Configuracoes";
        DrawText(titulo, tela.largura / 2 - MeasureText(titulo, 40) / 2, 200, 40, WHITE);
        for (int i = 0; i < totalConfig; i++) {
            int fontSize = (i == opcaoConfig) ? 32 : 26;
            Color cor    = (i == opcaoConfig) ? YELLOW : WHITE;
            int x = tela.largura / 2 - MeasureText(opcoesConfig[i], fontSize) / 2;
            DrawText(opcoesConfig[i], x, botoesConfigY[i], fontSize, cor);
        }
        DrawText("ESC = Voltar",
            tela.largura / 2 - MeasureText("ESC = Voltar", 16) / 2,
            tela.altura - 30, 16, DARKGRAY);
    }

    EndDrawing();
}