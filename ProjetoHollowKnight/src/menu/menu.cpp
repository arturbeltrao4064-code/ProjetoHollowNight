#include "menu.h"
#include "game.h"
#include "mapa.h"
#include "personagem.h"
#include "save.h"

#include <raylib.h>
#include <stdio.h>
#include <cstdlib>

Estado estadoAtual = ESTADO_MENU;

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
    else if (estadoAtual == ESTADO_INVENTARIO) {
        desenhaInventario();
    }

    // DRAW
    BeginDrawing();
    ClearBackground(BLACK);

    if (estadoAtual == ESTADO_MENU) {
        desenhaFundoMenu();
        desenhaBotao(0, 0, menuPrincipal.opcaoSelecionada, false);
        desenhaBotao(1, 1, menuPrincipal.opcaoSelecionada, false);
        desenhaBotao(2, 2, menuPrincipal.opcaoSelecionada, false);
        desenhaBotao(5, 3, menuPrincipal.opcaoSelecionada, false);
        desenhaBotao(3, 4, menuPrincipal.opcaoSelecionada, false);
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
        DrawRectangle(0, 0, tela.largura, tela.altura, (Color){0, 0, 0, 120});
        const char* titulo = "Pausado";
        DrawText(titulo, tela.largura / 2 - MeasureText(titulo, 32) / 2, 160, 32, WHITE);
        desenhaBotaoPause(0, 0, menuPause.opcaoSelecionada, false);
        desenhaBotaoPause(1, 1, menuPause.opcaoSelecionada, false);
        desenhaBotaoPause(2, 2, menuPause.opcaoSelecionada, false);
        desenhaBotaoPause(3, 3, menuPause.opcaoSelecionada, false);
        desenhaBotaoPause(4, 4, menuPause.opcaoSelecionada, false);
        const char* hint = "W/S = Navegar | ENTER = Selecionar | ESC = Continuar";
        DrawText(hint, tela.largura / 2 - MeasureText(hint, 14) / 2, tela.altura - 30, 14, DARKGRAY);
    }
    else if (estadoAtual == ESTADO_CONFIGURACOES) {
        desenhaFundoMenu();
        const char* titulo = "Configuracoes";
        const char* opcoes[3] = {"VOLUME", "AJUSTES", "VOLTAR"};
        DrawText(titulo, tela.largura / 2 - MeasureText(titulo, 40) / 2, 200, 40, WHITE);
        for (int i = 0; i < menuConfiguracoes.totalOpcoes; i++) {
            int fontSize = (i == menuConfiguracoes.opcaoSelecionada) ? 32 : 26;
            Color cor = (i == menuConfiguracoes.opcaoSelecionada) ? YELLOW : WHITE;
            int x = tela.largura / 2 - MeasureText(opcoes[i], fontSize) / 2;
            DrawText(opcoes[i], x, menuConfiguracoes.botoesY[i], fontSize, cor);
        }
        DrawText("ESC = Voltar",
            tela.largura / 2 - MeasureText("ESC = Voltar", 16) / 2,
            tela.altura - 30, 16, DARKGRAY);
    }

    EndDrawing();
}
