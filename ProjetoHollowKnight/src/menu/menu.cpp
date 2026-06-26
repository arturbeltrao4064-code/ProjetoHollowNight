#include "menu.h"
#include "game.h"
#include "mapa.h"
#include "personagem.h"
#include "save.h"

#include <raylib.h>
#include <stdio.h>
#include <cstdlib>

Estado estadoAtual = ESTADO_MENU;
static int opcaoMorteSelecionada = 0;
static const bool MORTE_DESABILITADA_TESTE = false;

static void updateFim() {
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        estadoAtual = ESTADO_MENU;
    }
}

static void updateAjuda() {
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        estadoAtual = ESTADO_MENU;
    }
}

static void iniciarNovoJogo() {
    resetarEstadoNovoJogo();
    unloadMapa();
    map.localMapa = caminhoMapaPorFase(faseDoJogo);
    carregarMapaAtualComEntidades();
    estadoAtual = ESTADO_JOGANDO;
}

static void updateMorte() {
    Vector2 mouse = GetMousePosition();
    Rectangle botaoReiniciar = { (float)tela.largura / 2 - 160.0f, 340.0f, 320.0f, 52.0f };
    Rectangle botaoMenu = { (float)tela.largura / 2 - 160.0f, 410.0f, 320.0f, 52.0f };

    if (CheckCollisionPointRec(mouse, botaoReiniciar)) {
        opcaoMorteSelecionada = 0;
    }
    if (CheckCollisionPointRec(mouse, botaoMenu)) {
        opcaoMorteSelecionada = 1;
    }

    if (IsKeyPressed(KEY_DOWN)) {
        opcaoMorteSelecionada = (opcaoMorteSelecionada + 1) % 2;
    }
    if (IsKeyPressed(KEY_UP)) {
        opcaoMorteSelecionada = (opcaoMorteSelecionada - 1 + 2) % 2;
    }

    if (IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (opcaoMorteSelecionada == 0) {
            iniciarNovoJogo();
        } else {
            estadoAtual = ESTADO_MENU;
            opcaoMorteSelecionada = 0;
        }
    }
}

static void desenhaTelaMenuPrincipal() {
    desenhaFundoMenu();
    desenhaBotao(1, 0, menuPrincipal.opcaoSelecionada, false);
    desenhaBotao(0, 1, menuPrincipal.opcaoSelecionada, false);
    desenhaBotao(2, 2, menuPrincipal.opcaoSelecionada, false);
    desenhaBotao(3, 3, menuPrincipal.opcaoSelecionada, false);
    DrawText("J=Continuar  N=Novo Jogo  A=Ajuda  S=Sair",
        tela.largura / 2 - MeasureText("J=Continuar  N=Novo Jogo  A=Ajuda  S=Sair", 16) / 2,
        tela.altura - 30, 16, DARKGRAY);
}

static void desenhaTelaPausada() {
    drawJogo();
    DrawRectangle(0, 0, tela.largura, tela.altura, (Color){0, 0, 0, 120});
    const char* titulo = "Pausado";
    DrawText(titulo, tela.largura / 2 - MeasureText(titulo, 32) / 2, 160, 32, WHITE);
    desenhaBotaoPause(0, 0, menuPause.opcaoSelecionada, false);
    desenhaBotaoPause(1, 1, menuPause.opcaoSelecionada, false);
    desenhaBotaoPause(2, 2, menuPause.opcaoSelecionada, false);
    desenhaBotaoPause(3, 3, menuPause.opcaoSelecionada, false);
    const char* hint = "C=Continuar  I=Inventario  J=Salvar  S=Sair  ESC=Continuar";
    DrawText(hint, tela.largura / 2 - MeasureText(hint, 14) / 2, tela.altura - 30, 14, DARKGRAY);

    if (confirmacaoSaidaPauseAtiva()) {
        DrawRectangle(0, 0, tela.largura, tela.altura, (Color){0, 0, 0, 160});

        Rectangle caixa = { (float)tela.largura / 2 - 220.0f, (float)tela.altura / 2 - 90.0f, 440.0f, 200.0f };
        DrawRectangleRec(caixa, (Color){25, 25, 30, 240});
        DrawRectangleLinesEx(caixa, 2, WHITE);

        const char* pergunta = "Tem certeza que deseja sair?";
        DrawText(pergunta,
            (int)(caixa.x + caixa.width / 2 - MeasureText(pergunta, 28) / 2),
            (int)(caixa.y + 28), 28, WHITE);

        Rectangle botaoNao = { caixa.x + 80.0f, caixa.y + 120.0f, 120.0f, 44.0f };
        Rectangle botaoSim = { caixa.x + 240.0f, caixa.y + 120.0f, 120.0f, 44.0f };
        bool simSelecionado = (opcaoConfirmacaoSaidaPause() == 1);

        DrawRectangleRec(botaoNao, simSelecionado ? GRAY : LIGHTGRAY);
        DrawRectangleRec(botaoSim, simSelecionado ? LIGHTGRAY : GRAY);
        DrawRectangleLinesEx(botaoNao, 2, WHITE);
        DrawRectangleLinesEx(botaoSim, 2, WHITE);

        DrawText("NAO", (int)(botaoNao.x + botaoNao.width / 2 - MeasureText("NAO", 22) / 2), (int)(botaoNao.y + 10), 22, BLACK);
        DrawText("SIM", (int)(botaoSim.x + botaoSim.width / 2 - MeasureText("SIM", 22) / 2), (int)(botaoSim.y + 10), 22, BLACK);
    }
}

static void desenhaTelaMorte() {
    drawJogo();
    DrawRectangle(0, 0, tela.largura, tela.altura, (Color){0, 0, 0, 170});

    const char* titulo = "VOCE MORREU";
    DrawText(titulo,
        tela.largura / 2 - MeasureText(titulo, 54) / 2,
        170, 54, RED);

    const char* pergunta = "Deseja comecar novamente?";
    DrawText(pergunta,
        tela.largura / 2 - MeasureText(pergunta, 28) / 2,
        250, 28, RAYWHITE);

    Rectangle botaoReiniciar = { (float)tela.largura / 2 - 160.0f, 340.0f, 320.0f, 52.0f };
    Rectangle botaoMenu = { (float)tela.largura / 2 - 160.0f, 410.0f, 320.0f, 52.0f };

    DrawRectangleRec(botaoReiniciar, opcaoMorteSelecionada == 0 ? LIGHTGRAY : GRAY);
    DrawRectangleRec(botaoMenu, opcaoMorteSelecionada == 1 ? LIGHTGRAY : GRAY);
    DrawRectangleLinesEx(botaoReiniciar, 2, BLACK);
    DrawRectangleLinesEx(botaoMenu, 2, BLACK);

    const char* op0 = "REINICIAR";
    const char* op1 = "VOLTAR AO MENU";
    DrawText(op0,
        (int)(botaoReiniciar.x + botaoReiniciar.width / 2 - MeasureText(op0, 24) / 2),
        (int)(botaoReiniciar.y + 14), 24, BLACK);
    DrawText(op1,
        (int)(botaoMenu.x + botaoMenu.width / 2 - MeasureText(op1, 24) / 2),
        (int)(botaoMenu.y + 14), 24, BLACK);

    const char* hint = "SETAS = Navegar | ENTER = Selecionar";
    DrawText(hint,
        tela.largura / 2 - MeasureText(hint, 16) / 2,
        tela.altura - 40, 16, LIGHTGRAY);
}

static void desenhaTelaFim() {
    DrawRectangle(0, 0, tela.largura, tela.altura, (Color){10, 10, 10, 255});

    const char* titulo = "FIM DE JOGO";
    const char* subtitulo = "Voce concluiu todos os tuneis!";
    const char* info = "Pressione ENTER ou ESC para voltar ao menu";

    DrawText(titulo,
        tela.largura / 2 - MeasureText(titulo, 62) / 2,
        220, 62, GOLD);
    DrawText(subtitulo,
        tela.largura / 2 - MeasureText(subtitulo, 30) / 2,
        320, 30, RAYWHITE);
    DrawText(info,
        tela.largura / 2 - MeasureText(info, 22) / 2,
        410, 22, LIGHTGRAY);
}

static void desenhaTelaAjuda() {
    DrawRectangle(0, 0, tela.largura, tela.altura, (Color){18, 18, 24, 255});

    const char* titulo = "AJUDA";
    DrawText(titulo,
        tela.largura / 2 - MeasureText(titulo, 56) / 2,
        40, 56, YELLOW);

    int x = 90;
    int y = 130;
    int lh = 30;

    DrawText("CONTROLES", x, y, 30, GOLD); y += lh + 12;
    DrawText("SETAS: mover personagem e navegar menus", x, y, 22, RAYWHITE); y += lh;
    DrawText("X: ataque corpo a corpo", x, y, 22, RAYWHITE); y += lh;
    DrawText("D: habilidade (disparo)", x, y, 22, RAYWHITE); y += lh;
    DrawText("A (segurar): canalizar cura com flask", x, y, 22, RAYWHITE); y += lh;
    DrawText("ESC: pause / voltar", x, y, 22, RAYWHITE); y += lh;
    DrawText("TAB: abrir inventario", x, y, 22, RAYWHITE); y += lh + 14;

    DrawText("AMULETOS", x, y, 30, GOLD); y += lh + 12;
    DrawText("1 ATAQUE: aumenta o dano por hit", x, y, 22, RAYWHITE); y += lh;
    DrawText("2 VELOCIDADE: acelera movimento", x, y, 22, RAYWHITE); y += lh;
    DrawText("3 VIDA: adiciona +1 vida maxima", x, y, 22, RAYWHITE); y += lh;
    DrawText("Apenas 1 amuleto equipado por vez", x, y, 22, RAYWHITE); y += lh;
    DrawText("Equipar/desequipar apenas na vila", x, y, 22, RAYWHITE); y += lh + 14;

    DrawText("INIMIGOS E BOSS", x, y, 30, GOLD); y += lh + 12;
    DrawText("Inimigos comuns: aproximam e causam dano por contato", x, y, 22, RAYWHITE); y += lh;
    DrawText("Boss: ativa quando o jogador se aproxima, pula e persegue", x, y, 22, RAYWHITE); y += lh;
    DrawText("Boss causa dano e knockback, mantenha distancia e ataque no timing", x, y, 22, RAYWHITE); y += lh + 18;

    DrawText("ENTER ou ESC para voltar ao menu", x, y, 24, LIGHTGRAY);
}

void desenhaMenu() {
    // UPDATE
    if (estadoAtual == ESTADO_MENU)
        updateMenuPrincipal();
    else if (estadoAtual == ESTADO_JOGANDO) {
        updateJogo();
        if (!MORTE_DESABILITADA_TESTE && (!personagem.dados.vivo || personagem.dados.hp <= 0)) {
            estadoAtual = ESTADO_MORTE;
            opcaoMorteSelecionada = 0;
        }
        if (IsKeyPressed(KEY_TAB)) {
            estadoAtual = ESTADO_INVENTARIO;
        }
        if (IsKeyPressed(KEY_ESCAPE)) {
            estadoAtual = ESTADO_PAUSADO;
            menuPause.opcaoSelecionada = 0;
        }
    }
    else if (estadoAtual == ESTADO_PAUSADO)
        updatePause();
    else if (estadoAtual == ESTADO_INVENTARIO) {
        desenhaInventario();
    }
    else if (estadoAtual == ESTADO_MORTE) {
        updateMorte();
    }
    else if (estadoAtual == ESTADO_FIM) {
        updateFim();
    }
    else if (estadoAtual == ESTADO_AJUDA) {
        updateAjuda();
    }

    // DRAW
    BeginDrawing();
    ClearBackground(BLACK);

    if (estadoAtual == ESTADO_MENU) {
        desenhaTelaMenuPrincipal();
    }
    else if (estadoAtual == ESTADO_JOGANDO) {
        drawJogo();
        const char* hint = "ESC = Pause";
        DrawText(hint, tela.largura - MeasureText(hint, 18) - 10, 10, 18, DARKGRAY);
    }
    else if (estadoAtual == ESTADO_PAUSADO) {
        desenhaTelaPausada();
    }
    else if (estadoAtual == ESTADO_MORTE) {
        desenhaTelaMorte();
    }
    else if (estadoAtual == ESTADO_FIM) {
        desenhaTelaFim();
    }
    else if (estadoAtual == ESTADO_AJUDA) {
        desenhaTelaAjuda();
    }

    EndDrawing();
}
