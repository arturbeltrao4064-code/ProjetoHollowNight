#include "hud.h"
#include "personagem.h"
#include <raylib.h>
#include <stdio.h>

extern float flaskCarga;
extern float avisoVidaCheia;

void desenhaHud(int hp, int mp, int ataque, int defesa) {
    (void)ataque;
    (void)defesa;

    int barLargura = 200;
    int barAltura  = 18;
    int x          = 20;
    int yVida      = 20;
    int yEnergia   = 60;
    int yFlask     = 90;

    // --- VIDAS (dinâmico: 5 ou 6 com amuleto) ---
    DrawText("VIDAS", x, yVida - 2, 14, WHITE);
    int vidasMax = personagem.dados.hpMax;
    int vidaW = 28;
    int vidaH = 14;
    int gap = 6;
    int vidaX0 = x + 60;

    for (int i = 0; i < vidasMax; i++) {
        int vx = vidaX0 + i * (vidaW + gap);
        Color corVida;
        if (i < hp) {
            corVida = (i == vidasMax - 1 && vidasMax == 6) ? (Color){100, 255, 100, 255} : RED;
        } else {
            corVida = (Color){60, 20, 20, 200};
        }
        DrawRectangle(vx, yVida, vidaW, vidaH, corVida);
        DrawRectangleLines(vx, yVida, vidaW, vidaH, WHITE);
    }

    // --- ENERGIA MP ---
    DrawRectangle(x, yEnergia, barLargura, barAltura, (Color){ 0, 0, 40, 200 });
    DrawRectangle(x, yEnergia, (int)(barLargura * (mp / 100.0f)), barAltura, BLUE);
    DrawRectangleLines(x, yEnergia, barLargura, barAltura, WHITE);
    DrawText("MP", x + barLargura + 8, yEnergia + 2, 14, WHITE);

    // --- FLASK (cura) ---
    int flask = personagem.dados.flask;
    if (flask < 0) flask = 0;
    if (flask > 100) flask = 100;
    DrawRectangle(x, yFlask, barLargura, barAltura, (Color){ 35, 35, 0, 200 });
    DrawRectangle(x, yFlask, (int)(barLargura * (flask / 100.0f)), barAltura, GOLD);
    DrawRectangleLines(x, yFlask, barLargura, barAltura, WHITE);
    DrawText("FLASK", x + barLargura + 8, yFlask + 2, 14, WHITE);

    // --- HABILIDADES ---
    int yHab = tela.altura - 95;
    DrawText("HABILIDADES:", x, yHab, 14, YELLOW);
    for (int i = 0; i < 3; i++) {
        Rectangle slot = { (float)(x + (i * 30)), (float)(yHab + 20), 25, 25 };
        if (i < personagem.dados.habilidadesColetadas) {
            DrawRectangleRec(slot, ORANGE);
            DrawRectangleLinesEx(slot, 1, YELLOW);
        } else {
            DrawRectangleRec(slot, DARKGRAY);
            DrawRectangleLinesEx(slot, 1, GRAY);
        }
    }
}