#include "menu.h"
#include "game.h"
#include "mapa.h"
#include "save.h"
#include "personagem.h"
#include <raylib.h>
#include <cstdlib>

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

    if (IsKeyPressed(KEY_ENTER) || cliqueMouse) {
        switch (menuPrincipal.opcaoSelecionada) {
            case 0:
                personagem.dados.hp = 5;
                personagem.dados.hpMax = 5;
                personagem.dados.mp = 100;
                flaskCarga = 100.0f;
                personagem.dados.flask = 100;
                personagem.dados.habilidadesColetadas = 0;
                personagem.dados.amuletosColetados = 0;
                for (int i = 0; i < TOTAL_AMULETOS; i++) {
                    personagem.dados.amuletos[i].coletado = false;
                }
                personagem.dados.ataque = false;
                personagem.dados.vivo = true;
                personagem.dados.habilidadeAtiva.ativo = false;
                faseDoJogo = FASE_VILA;
                proximoTunel = 1;
                unloadMapa();
                map.localMapa = "maps/vila.txt";
                loadMapa();
                inicializaPosicoesEntidades();
                estadoAtual = ESTADO_JOGANDO;
                break;
            case 1:
                carregaJogo();
                unloadMapa();
                switch (faseDoJogo) {
                    case FASE_VILA: map.localMapa = "maps/vila.txt"; break;
                    case FASE_INICIAL: map.localMapa = "maps/tunel1.txt"; break;
                    case FASE_FINAL: map.localMapa = "maps/tunel2.txt"; break;
                    case FASE_TUNEL3: map.localMapa = "maps/tunel3.txt"; break;
                }
                loadMapa();
                inicializaPosicoesEntidades();
                estadoAtual = ESTADO_JOGANDO;
                break;
            case 2:
                estadoAtual = ESTADO_CONFIGURACOES;
                break;
            case 3:
                estadoAtual = ESTADO_AJUDA;
                break;
            case 4:
                CloseWindow();
                exit(0);
                break;
        }
    }
}

void updatePause() {
    Vector2 mouse = GetMousePosition();
    bool cliqueMouse = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

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

    if (IsKeyPressed(KEY_ENTER) || cliqueMouse) {
        switch (menuPause.opcaoSelecionada) {
            case 0: estadoAtual = ESTADO_JOGANDO; break;
            case 1: estadoAtual = ESTADO_INVENTARIO; break;
            case 2: estadoAtual = ESTADO_MENU; break;
            case 3: salvaJogo(); break;
            case 4: CloseWindow(); exit(0); break;
        }
    }
}

void updateConfiguracoes() {
    Vector2 mouse = GetMousePosition();
    bool cliqueMouse = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);

    for (int i = 0; i < menuConfiguracoes.totalOpcoes; i++) {
        Rectangle item = {
            (float)(tela.largura / 2 - 140),
            (float)(menuConfiguracoes.botoesY[i] - 20),
            280.0f,
            40.0f
        };
        if (CheckCollisionPointRec(mouse, item)) {
            menuConfiguracoes.opcaoSelecionada = i;
        }
    }

    if (IsKeyPressed(KEY_DOWN))
        menuConfiguracoes.opcaoSelecionada = (menuConfiguracoes.opcaoSelecionada + 1) % menuConfiguracoes.totalOpcoes;
    if (IsKeyPressed(KEY_UP))
        menuConfiguracoes.opcaoSelecionada = (menuConfiguracoes.opcaoSelecionada - 1 + menuConfiguracoes.totalOpcoes) % menuConfiguracoes.totalOpcoes;
    if ((IsKeyPressed(KEY_ENTER) || cliqueMouse) && menuConfiguracoes.opcaoSelecionada == menuConfiguracoes.totalOpcoes - 1)
        estadoAtual = ESTADO_MENU;
    if (IsKeyPressed(KEY_ESCAPE))
        estadoAtual = ESTADO_MENU;
}
