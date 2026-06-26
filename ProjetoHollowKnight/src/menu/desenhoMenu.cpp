#include "menu.h"
#include "game.h"
#include "personagem.h"
#include <raylib.h>
#include <stdio.h>

static void desenhaImagemBotao(Texture2D textura, float x, float y, float w, float h, Color tint) {
    if (textura.width > 0 && textura.height > 0) {
        Rectangle src = { 0.0f, 0.0f, (float)textura.width, (float)textura.height };
        Rectangle dst = { x, y, w, h };
        Vector2 origin = { 0.0f, 0.0f };
        DrawTexturePro(textura, src, dst, origin, 0.0f, tint);
    } else {
        DrawRectangle((int)x, (int)y, (int)w, (int)h, tint);
    }
}

void desenhaFundoMenu() {
    if (menuPrincipal.menuImagem[0].width > 0) {
        DrawTexturePro(menuPrincipal.menuImagem[0], { 0.0f, 0.0f, (float)menuPrincipal.menuImagem[0].width, (float)menuPrincipal.menuImagem[0].height }, { 0.0f, 0.0f, (float)tela.largura, (float)tela.altura }, { 0, 0 }, 0.0f, WHITE);
    } else {
        ClearBackground(DARKGRAY);
    }

}

void desenhaBotao(int texIndex, int posIndex, int selecionado, bool desabilitado) {
    float x = tela.largura / 2.0f - menuPrincipal.botaoW / 2.0f;
    float y = menuPrincipal.botoesY[posIndex] - menuPrincipal.botaoH / 2.0f;
    Color corBotao = desabilitado ? Fade(GRAY, 0.75f) : WHITE;
    if (posIndex == selecionado && !desabilitado) corBotao = WHITE;
    desenhaImagemBotao(menuPrincipal.menuBotoes[texIndex], x, y, (float)menuPrincipal.botaoW, (float)menuPrincipal.botaoH, corBotao);

    if (posIndex == selecionado && !desabilitado)
        DrawRectangleLinesEx({ x - 2, y - 2, (float)menuPrincipal.botaoW + 4, (float)menuPrincipal.botaoH + 4 }, 2, YELLOW);
}

void desenhaBotaoPause(int texIndex, int posIndex, int selecionado, bool desabilitado) {
    float x = tela.largura / 2.0f - menuPause.botaoW / 2.0f;
    float y = menuPause.botoesY[posIndex] - menuPause.botaoH / 2.0f;
    Color corBotao = desabilitado ? Fade(GRAY, 0.75f) : WHITE;
    if (posIndex == selecionado && !desabilitado) corBotao = WHITE;
    desenhaImagemBotao(menuPause.menuBotoes[texIndex], x, y, (float)menuPause.botaoW, (float)menuPause.botaoH, corBotao);

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
    DrawRectangle(invX, invY, invLargura, invAltura, (Color){20, 20, 20, 235});
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
            float alfa = equipado ? 0.75f : 0.4f;
            DrawRectangleRec(slot, Fade(cores[i], alfa));
            DrawRectangleLinesEx(slot, selecionado ? 5 : 3, selecionado ? YELLOW : cores[i]);
            if (texturaAmuletos[i].width > 0 && texturaAmuletos[i].height > 0) {
                Rectangle src = { 0.0f, 0.0f, (float)texturaAmuletos[i].width, (float)texturaAmuletos[i].height };
                Rectangle dst = { slot.x + 35, slot.y + 25, 80, 80 };
                Vector2 origin = { 0.0f, 0.0f };
                DrawTexturePro(texturaAmuletos[i], src, dst, origin, 0.0f, WHITE);
                DrawRectangleLinesEx(dst, 2.0f, WHITE);
            }
            DrawText(nomes[i],   (int)(slot.x + 15), (int)(slot.y + 5), 18, WHITE);
            DrawText(efeitos[i], (int)(slot.x + 10), (int)(slot.y + 110), 12, LIGHTGRAY);
            if (equipado) {
                DrawText("EQUIPADO", (int)(slot.x + 20), (int)(slot.y + 135), 14, GOLD);
            } else {
                DrawText("[coletado]", (int)(slot.x + 15), (int)(slot.y + 135), 12, LIGHTGRAY);
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
