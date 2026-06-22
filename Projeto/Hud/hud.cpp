#include "hud.h"
#include <raylib.h>

static float vida    = 100.0f;
static float energia = 100.0f;

void initHud() {
    vida    = 100.0f;
    energia = 100.0f;
}

void desenhaHud() {
    int barLargura = 200;
    int barAltura  = 18;
    int x          = 20;
    int yVida      = 20;
    int yEnergia   = 50;

    // --- VIDA ---
    // Fundo escuro
    DrawRectangle(x, yVida, barLargura, barAltura, { 40, 0, 0, 200 });
    // Preenchimento
    DrawRectangle(x, yVida, (int)(barLargura * (vida / 100.0f)), barAltura, RED);
    // Contorno branco
    DrawRectangleLines(x, yVida, barLargura, barAltura, WHITE);
    // Label
    DrawText("HP", x + barLargura + 8, yVida + 2, 14, WHITE);

    // --- ENERGIA ---
    DrawRectangle(x, yEnergia, barLargura, barAltura, { 0, 0, 40, 200 });
    DrawRectangle(x, yEnergia, (int)(barLargura * (energia / 100.0f)), barAltura, BLUE);
    DrawRectangleLines(x, yEnergia, barLargura, barAltura, WHITE);
    DrawText("MP", x + barLargura + 8, yEnergia + 2, 14, WHITE);
}