#include "menu.h"
#include "game.h"
#include "personagem.h"
#include <raylib.h>
#include <stdio.h>

void desenhaFundoMenu() {
    ClearBackground(DARKGRAY);
    const char* tituloJogo = "HOLLOW KNIGHT (PROTO)";
    int tamTitulo = MeasureText(tituloJogo, 40);
    DrawText(tituloJogo, tela.largura / 2 - tamTitulo / 2, 100, 40, LIGHTGRAY);
}

void desenhaBotao(int texIndex, int posIndex, int selecionado, bool desabilitado) {
    float x = tela.largura / 2.0f - menuPrincipal.botaoW / 2.0f;
    float y = menuPrincipal.botoesY[posIndex] - menuPrincipal.botaoH / 2.0f;
    Color corBotao = desabilitado ? GRAY : LIGHTGRAY;
    if (posIndex == selecionado && !desabilitado) corBotao = RAYWHITE;
    DrawRectangle((int)x, (int)y, menuPrincipal.botaoW, menuPrincipal.botaoH, corBotao);

    const char* textoBotao = "BOTAO";
    switch (texIndex) {
        case 0: textoBotao = "NOVO JOGO"; break;
        case 1: textoBotao = "CARREGAR"; break;
        case 2: textoBotao = "OPCOES"; break;
        case 3: textoBotao = "SAIR"; break;
        case 5: textoBotao = "AJUDA"; break;
    }

    int tamTexto = MeasureText(textoBotao, 20);
    int textX = (int)(x + (menuPrincipal.botaoW / 2.0f) - (tamTexto / 2.0f));
    int textY = (int)(y + (menuPrincipal.botaoH / 2.0f) - 10);
    DrawText(textoBotao, textX, textY, 20, desabilitado ? DARKGRAY : BLACK);

    if (posIndex == selecionado && !desabilitado)
        DrawRectangleLinesEx({ x - 2, y - 2, (float)menuPrincipal.botaoW + 4, (float)menuPrincipal.botaoH + 4 }, 2, YELLOW);
}

void desenhaBotaoPause(int texIndex, int posIndex, int selecionado, bool desabilitado) {
    float x = tela.largura / 2.0f - menuPause.botaoW / 2.0f;
    float y = menuPause.botoesY[posIndex] - menuPause.botaoH / 2.0f;
    Color corBotao = desabilitado ? GRAY : LIGHTGRAY;
    if (posIndex == selecionado && !desabilitado) corBotao = RAYWHITE;
    DrawRectangle((int)x, (int)y, menuPause.botaoW, menuPause.botaoH, corBotao);

    const char* textoBotao = "BOTAO";
    switch (texIndex) {
        case 0: textoBotao = "CONTINUAR"; break;
        case 1: textoBotao = "INVENTARIO"; break;
        case 2: textoBotao = "MENU PRINCIPAL"; break;
        case 3: textoBotao = "SALVAR"; break;
        case 4: textoBotao = "SAIR"; break;
    }

    int tamTexto = MeasureText(textoBotao, 18);
    int textX = (int)(x + (menuPause.botaoW / 2.0f) - (tamTexto / 2.0f));
    int textY = (int)(y + (menuPause.botaoH / 2.0f) - 9);
    DrawText(textoBotao, textX, textY, 18, desabilitado ? DARKGRAY : BLACK);
    if (posIndex == selecionado && !desabilitado)
        DrawRectangleLinesEx({ x - 2, y - 2, (float)menuPause.botaoW + 4, (float)menuPause.botaoH + 4 }, 2, YELLOW);
}

void desenhaInventario() {
    drawJogo();
    DrawRectangle(0, 0, tela.largura, tela.altura, (Color){0, 0, 0, 150});
    int invLargura = 700;
    int invAltura = 450;
    int invX = tela.largura / 2 - invLargura / 2;
    int invY = tela.altura / 2 - invAltura / 2;
    DrawRectangle(invX, invY, invLargura, invAltura, DARKGRAY);
    DrawRectangleLines(invX, invY, invLargura, invAltura, WHITE);
    DrawText("INVENTARIO DE AMULETOS", invX + 30, invY + 30, 28, YELLOW);
    DrawText("Amuletos Coletados: ", invX + 30, invY + 70, 18, WHITE);

    char contadorText[20];
    sprintf(contadorText, "%d", personagem.dados.amuletosColetados);
    DrawText(contadorText, invX + 280, invY + 70, 18, YELLOW);

    int startX = invX + 60;
    int startY = invY + 130;
    int espacamentoX = 200;

    for (int i = 0; i < TOTAL_AMULETOS; i++) {
        Rectangle slot = { (float)(startX + (i * espacamentoX)), (float)startY, 140, 140 };
        const char* nomeAmuleto = "";
        const char* efeito = "";
        Color corAmuleto = GRAY;

        if (i == AMULETO_ATAQUE) {
            nomeAmuleto = "ATAQUE";
            efeito = "+10 ATK";
            corAmuleto = RED;
        } else if (i == AMULETO_DEFESA) {
            nomeAmuleto = "DEFESA";
            efeito = "+5 DEF";
            corAmuleto = BLUE;
        } else if (i == AMULETO_VIDA) {
            nomeAmuleto = "VIDA";
            efeito = "+20 HP";
            corAmuleto = GREEN;
        }

        if (personagem.dados.amuletos[i].coletado) {
            DrawRectangleRec(slot, Fade(corAmuleto, 0.4f));
            DrawRectangleLinesEx(slot, 3, corAmuleto);
            DrawText(nomeAmuleto, (int)(slot.x + 25), (int)(slot.y + 35), 20, WHITE);
            DrawText(efeito, (int)(slot.x + 20), (int)(slot.y + 65), 14, YELLOW);
            DrawText("[OK]", (int)(slot.x + 35), (int)(slot.y + 105), 14, GREEN);
        } else {
            DrawRectangleRec(slot, Fade(BLACK, 0.7f));
            DrawRectangleLinesEx(slot, 2, GRAY);
            DrawText("?", (int)(slot.x + 60), (int)(slot.y + 50), 40, GRAY);
            DrawText("[---]", (int)(slot.x + 30), (int)(slot.y + 105), 14, DARKGRAY);
        }
    }

    DrawText("Pressione ESC para voltar", invX + 30, invY + invAltura - 40, 16, LIGHTGRAY);
    if (IsKeyPressed(KEY_ESCAPE)) {
        estadoAtual = ESTADO_PAUSADO;
    }
}
