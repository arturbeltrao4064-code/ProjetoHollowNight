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
    static int slotInventario = 0;
    bool podeAlterarAmuleto = (faseDoJogo == FASE_VILA);
    Vector2 mouse = GetMousePosition();

    drawJogo();
    DrawRectangle(0, 0, tela.largura, tela.altura, (Color){0, 0, 0, 160});

    int invLargura = 720;
    int invAltura = 470;
    int invX = tela.largura / 2 - invLargura / 2;
    int invY = tela.altura / 2 - invAltura / 2;
    DrawRectangle(invX, invY, invLargura, invAltura, DARKGRAY);
    DrawRectangleLines(invX, invY, invLargura, invAltura, WHITE);
    DrawText("INVENTARIO DE AMULETOS", invX + 30, invY + 20, 26, YELLOW);
    DrawText("Use SETAS para navegar, ENTER para equipar", invX + 30, invY + 55, 14, LIGHTGRAY);
    DrawText("(Apenas um amuleto pode ser equipado por vez)", invX + 30, invY + 75, 13, GRAY);
    if (!podeAlterarAmuleto) {
        DrawText("Equipar/desequipar so e permitido na vila", invX + 30, invY + 95, 14, ORANGE);
    }

    // Navegacao
    if (IsKeyPressed(KEY_RIGHT))
        slotInventario = (slotInventario + 1) % TOTAL_AMULETOS;
    if (IsKeyPressed(KEY_LEFT))
        slotInventario = (slotInventario - 1 + TOTAL_AMULETOS) % TOTAL_AMULETOS;
    if (podeAlterarAmuleto && IsKeyPressed(KEY_ENTER) && personagem.dados.amuletos[slotInventario].coletado)
        equipaAmuleto(slotInventario);

    const char* nomes[TOTAL_AMULETOS]   = {"ATAQUE", "VELOCIDADE", "VIDA"};
    const char* efeitos[TOTAL_AMULETOS] = {"Hit duplo (dano x2)", "+50% velocidade", "+1 vida (6 slots)"};
    Color cores[TOTAL_AMULETOS]          = {RED, ORANGE, GREEN};

    int startX = invX + 50;
    int startY = invY + 120;
    int espacamentoX = 210;

    for (int i = 0; i < TOTAL_AMULETOS; i++) {
        Rectangle slot = { (float)(startX + i * espacamentoX), (float)startY, 150, 160 };
        if (CheckCollisionPointRec(mouse, slot)) {
            slotInventario = i;
        }
        bool selecionado = (i == slotInventario);
        bool equipado    = (personagem.dados.amuletaEquipado == i);

        if (personagem.dados.amuletos[i].coletado) {
            float alfa = equipado ? 0.65f : 0.35f;
            DrawRectangleRec(slot, Fade(cores[i], alfa));
            DrawRectangleLinesEx(slot, selecionado ? 4 : 2, selecionado ? WHITE : cores[i]);
            DrawText(nomes[i],   (int)(slot.x + 10), (int)(slot.y + 20), 18, WHITE);
            DrawText(efeitos[i], (int)(slot.x + 10), (int)(slot.y + 55), 12, YELLOW);
            if (equipado) {
                DrawText("EQUIPADO", (int)(slot.x + 25), (int)(slot.y + 120), 16, GOLD);
            } else {
                DrawText("[coletado]", (int)(slot.x + 20), (int)(slot.y + 125), 13, LIGHTGRAY);
            }
        } else {
            DrawRectangleRec(slot, Fade(BLACK, 0.7f));
            DrawRectangleLinesEx(slot, selecionado ? 4 : 2, selecionado ? WHITE : GRAY);
            DrawText("?", (int)(slot.x + 65), (int)(slot.y + 55), 40, GRAY);
            DrawText("[nao coletado]", (int)(slot.x + 10), (int)(slot.y + 125), 12, DARKGRAY);
        }
    }

    DrawText("ESC = voltar", invX + 30, invY + invAltura - 35, 15, LIGHTGRAY);
    if (podeAlterarAmuleto && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && personagem.dados.amuletos[slotInventario].coletado) {
        equipaAmuleto(slotInventario);
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        estadoAtual = ESTADO_PAUSADO;
    }
}
