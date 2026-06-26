#include "menu.h"
#include "game.h"
#include "mapa.h"
#include "save.h"
#include "personagem.h"
#include <raylib.h>
#include <cstdlib>

static bool confirmacaoSaidaAtiva = false;
static int opcaoConfirmacaoSaida = 0; // 0 = NAO, 1 = SIM

bool confirmacaoSaidaPauseAtiva() {
    return confirmacaoSaidaAtiva;
}

int opcaoConfirmacaoSaidaPause() {
    return opcaoConfirmacaoSaida;
}

static void executaOpcaoMenuPrincipal(int opcao) {
    switch (opcao) {
        case 0:
            carregaJogo();
            unloadMapa();
            map.localMapa = caminhoMapaPorFase(faseDoJogo);
            carregarMapaAtualComEntidades();
            estadoAtual = ESTADO_JOGANDO;
            break;
        case 1:
            resetarEstadoNovoJogo();
            unloadMapa();
            map.localMapa = caminhoMapaPorFase(faseDoJogo);
            carregarMapaAtualComEntidades();
            estadoAtual = ESTADO_JOGANDO;
            break;
        case 2:
            estadoAtual = ESTADO_AJUDA;
            break;
        case 3:
            CloseWindow();
            exit(0);
            break;
    }
}

static void executaOpcaoPause(int opcao) {
    switch (opcao) {
        case 0:
            estadoAtual = ESTADO_JOGANDO;
            break;
        case 1:
            estadoAtual = ESTADO_INVENTARIO;
            break;
        case 2:
            salvaJogo();
            break;
        case 3:
            confirmacaoSaidaAtiva = true;
            opcaoConfirmacaoSaida = 0;
            break;
    }
}

void updateMenuPrincipal() {
    Vector2 mouse = GetMousePosition();
    bool cliqueMouse = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    float x = tela.largura / 2.0f - menuPrincipal.botaoW / 2.0f;
    for (int i = 0; i < menuPrincipal.totalOpcoes; i++) {
        float y = menuPrincipal.botoesY[i] - menuPrincipal.botaoH / 2.0f;
        Rectangle botao = { x, y, (float)menuPrincipal.botaoW, (float)menuPrincipal.botaoH };
        if (CheckCollisionPointRec(mouse, botao)) {
            menuPrincipal.opcaoSelecionada = i;
        }
    }

    if (IsKeyPressed(KEY_DOWN))
        menuPrincipal.opcaoSelecionada = (menuPrincipal.opcaoSelecionada + 1) % menuPrincipal.totalOpcoes;
    if (IsKeyPressed(KEY_UP))
        menuPrincipal.opcaoSelecionada = (menuPrincipal.opcaoSelecionada - 1 + menuPrincipal.totalOpcoes) % menuPrincipal.totalOpcoes;

    if (IsKeyPressed(KEY_J)) {
        menuPrincipal.opcaoSelecionada = 0;
        executaOpcaoMenuPrincipal(0);
        return;
    }
    if (IsKeyPressed(KEY_N)) {
        menuPrincipal.opcaoSelecionada = 1;
        executaOpcaoMenuPrincipal(1);
        return;
    }
    if (IsKeyPressed(KEY_A)) {
        menuPrincipal.opcaoSelecionada = 2;
        executaOpcaoMenuPrincipal(2);
        return;
    }
    if (IsKeyPressed(KEY_S)) {
        menuPrincipal.opcaoSelecionada = 3;
        executaOpcaoMenuPrincipal(3);
        return;
    }

    if (IsKeyPressed(KEY_ENTER) || cliqueMouse) {
        executaOpcaoMenuPrincipal(menuPrincipal.opcaoSelecionada);
    }
}

void updatePause() {
    Vector2 mouse = GetMousePosition();
    bool cliqueMouse = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    if (confirmacaoSaidaAtiva) {
        Rectangle botaoNao = { (float)tela.largura / 2 - 140.0f, (float)tela.altura / 2 + 30.0f, 120.0f, 44.0f };
        Rectangle botaoSim = { (float)tela.largura / 2 + 20.0f, (float)tela.altura / 2 + 30.0f, 120.0f, 44.0f };

        if (CheckCollisionPointRec(mouse, botaoNao)) opcaoConfirmacaoSaida = 0;
        if (CheckCollisionPointRec(mouse, botaoSim)) opcaoConfirmacaoSaida = 1;

        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) {
            opcaoConfirmacaoSaida = (opcaoConfirmacaoSaida == 0) ? 1 : 0;
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            confirmacaoSaidaAtiva = false;
            opcaoConfirmacaoSaida = 0;
            return;
        }

        if (IsKeyPressed(KEY_ENTER) || cliqueMouse) {
            bool confirmouSim = (opcaoConfirmacaoSaida == 1);
            if (cliqueMouse) {
                if (CheckCollisionPointRec(mouse, botaoNao)) confirmouSim = false;
                if (CheckCollisionPointRec(mouse, botaoSim)) confirmouSim = true;
            }

            if (confirmouSim) {
                CloseWindow();
                exit(0);
            } else {
                confirmacaoSaidaAtiva = false;
                opcaoConfirmacaoSaida = 0;
            }
        }
        return;
    }

    float x = tela.largura / 2.0f - menuPause.botaoW / 2.0f;
    for (int i = 0; i < menuPause.totalOpcoes; i++) {
        float y = menuPause.botoesY[i] - menuPause.botaoH / 2.0f;
        Rectangle botao = { x, y, (float)menuPause.botaoW, (float)menuPause.botaoH };
        if (CheckCollisionPointRec(mouse, botao)) {
            menuPause.opcaoSelecionada = i;
        }
    }

    if (IsKeyPressed(KEY_DOWN))
        menuPause.opcaoSelecionada = (menuPause.opcaoSelecionada + 1) % menuPause.totalOpcoes;
    if (IsKeyPressed(KEY_UP))
        menuPause.opcaoSelecionada = (menuPause.opcaoSelecionada - 1 + menuPause.totalOpcoes) % menuPause.totalOpcoes;

    if (IsKeyPressed(KEY_ESCAPE))
        estadoAtual = ESTADO_JOGANDO;

    if (IsKeyPressed(KEY_TAB)) {
        estadoAtual = ESTADO_INVENTARIO;
        return;
    }

    if (IsKeyPressed(KEY_C)) {
        menuPause.opcaoSelecionada = 0;
        executaOpcaoPause(0);
        return;
    }
    if (IsKeyPressed(KEY_I)) {
        menuPause.opcaoSelecionada = 1;
        executaOpcaoPause(1);
        return;
    }
    if (IsKeyPressed(KEY_J)) {
        menuPause.opcaoSelecionada = 2;
        executaOpcaoPause(2);
        return;
    }
    if (IsKeyPressed(KEY_S)) {
        menuPause.opcaoSelecionada = 3;
        executaOpcaoPause(3);
        return;
    }

    if (IsKeyPressed(KEY_ENTER) || cliqueMouse) {
        executaOpcaoPause(menuPause.opcaoSelecionada);
    }
}

