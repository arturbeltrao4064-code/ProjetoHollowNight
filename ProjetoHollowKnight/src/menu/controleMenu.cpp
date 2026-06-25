#include "menu.h"
#include "game.h"
#include "mapa.h"
#include "save.h"
#include <raylib.h>
#include <cstdlib>

void updateMenuPrincipal() {
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        menuPrincipal.opcaoSelecionada = (menuPrincipal.opcaoSelecionada + 1) % menuPrincipal.totalOpcoes;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        menuPrincipal.opcaoSelecionada = (menuPrincipal.opcaoSelecionada - 1 + menuPrincipal.totalOpcoes) % menuPrincipal.totalOpcoes;

    if (IsKeyPressed(KEY_ENTER)) {
        switch (menuPrincipal.opcaoSelecionada) {
            case 0:
                personagem.dados.hp = 100;
                personagem.dados.mp = 50;
                personagem.dados.habilidadesColetadas = 0;
                personagem.dados.amuletosColetados = 0;
                for (int i = 0; i < TOTAL_AMULETOS; i++) {
                    personagem.dados.amuletos[i].coletado = false;
                }
                personagem.dados.ataque = false;
                personagem.dados.vivo = true;
                personagem.dados.habilidadeAtiva.ativo = false;
                faseDoJogo = FASE_VILA;
                unloadMapa();
                map.localMapa = "maps/mapaVila.txt";
                loadMapa();
                inicializaPosicoesEntidades();
                estadoAtual = ESTADO_JOGANDO;
                break;
            case 1:
                carregaJogo();
                unloadMapa();
                switch (faseDoJogo) {
                    case FASE_VILA: map.localMapa = "maps/mapaVila.txt"; break;
                    case FASE_INICIAL: map.localMapa = "maps/mapaInicial.txt"; break;
                    case FASE_FINAL: map.localMapa = "maps/mapaFinal.txt"; break;
                }
                loadMapa();
                inicializaPosicoesEntidades();
                estadoAtual = ESTADO_JOGANDO;
                break;
            case 2:
                estadoAtual = ESTADO_CONFIGURACOES;
                break;
            case 3:
                break;
            case 4:
                CloseWindow();
                exit(0);
                break;
        }
    }
}

void updatePause() {
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        menuPause.opcaoSelecionada = (menuPause.opcaoSelecionada + 1) % menuPause.totalOpcoes;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        menuPause.opcaoSelecionada = (menuPause.opcaoSelecionada - 1 + menuPause.totalOpcoes) % menuPause.totalOpcoes;

    if (IsKeyPressed(KEY_ESCAPE))
        estadoAtual = ESTADO_JOGANDO;

    if (IsKeyPressed(KEY_ENTER)) {
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
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        menuConfiguracoes.opcaoSelecionada = (menuConfiguracoes.opcaoSelecionada + 1) % menuConfiguracoes.totalOpcoes;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        menuConfiguracoes.opcaoSelecionada = (menuConfiguracoes.opcaoSelecionada - 1 + menuConfiguracoes.totalOpcoes) % menuConfiguracoes.totalOpcoes;
    if (IsKeyPressed(KEY_ENTER) && menuConfiguracoes.opcaoSelecionada == menuConfiguracoes.totalOpcoes - 1)
        estadoAtual = ESTADO_MENU;
    if (IsKeyPressed(KEY_ESCAPE))
        estadoAtual = ESTADO_MENU;
}
