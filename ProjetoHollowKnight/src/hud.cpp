#include "hud.h"
#include "personagem.h"
#include <raylib.h>
#include <stdio.h>

extern float flaskCarga;
extern float avisoVidaCheia;

void desenhaHud(int hp, int mp, int ataque, int defesa) {
    (void)ataque;
    (void)defesa;

    const float FLASK_MAX = 100.0f;
    const float FLASK_SEGMENTO = FLASK_MAX / 3.0f;

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

    // --- FLASK (cura em 3 segmentos) ---
    float flaskVisual = flaskCarga;
    if (flaskVisual < 0.0f) flaskVisual = 0.0f;
    if (flaskVisual > FLASK_MAX) flaskVisual = FLASK_MAX;

    DrawText("FLASK", x, yFlask - 2, 14, WHITE);
    int flaskW = 28;
    int flaskH = 14;
    int flaskGap = 6;
    int flaskX0 = x + 60;

    // Segmentos drenam da esquerda para a direita.
    for (int i = 0; i < 3; i++) {
        float segmentoMax = FLASK_MAX - (i * FLASK_SEGMENTO);
        float segmentoMin = segmentoMax - FLASK_SEGMENTO;
        float preenchimento = (flaskVisual - segmentoMin) / FLASK_SEGMENTO;
        if (preenchimento < 0.0f) preenchimento = 0.0f;
        if (preenchimento > 1.0f) preenchimento = 1.0f;

        int fx = flaskX0 + i * (flaskW + flaskGap);
        DrawRectangle(fx, yFlask, flaskW, flaskH, (Color){ 35, 35, 0, 200 });
        DrawRectangle(fx, yFlask, (int)(flaskW * preenchimento), flaskH, GOLD);
        DrawRectangleLines(fx, yFlask, flaskW, flaskH, WHITE);
    }

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