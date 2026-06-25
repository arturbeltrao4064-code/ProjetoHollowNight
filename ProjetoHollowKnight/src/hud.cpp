#include "hud.h"
#include "personagem.h" // Importante para ler os dados do personagem
#include <raylib.h>
#include <stdio.h>

void desenhaHud(int hp, int mp, int ataque, int defesa) {
    int barLargura = 200;
    int barAltura  = 18;
    int x          = 20;
    int yVida      = 20;
    int yEnergia   = 50;

    // --- VIDA ---
    DrawRectangle(x, yVida, barLargura, barAltura, { 40, 0, 0, 200 });
    DrawRectangle(x, yVida, (int)(barLargura * (hp / 100.0f)), barAltura, RED);
    DrawRectangleLines(x, yVida, barLargura, barAltura, WHITE);
    DrawText("HP", x + barLargura + 8, yVida + 2, 14, WHITE);

    // --- ENERGIA ---
    DrawRectangle(x, yEnergia, barLargura, barAltura, { 0, 0, 40, 200 });
    DrawRectangle(x, yEnergia, (int)(barLargura * (mp / 100.0f)), barAltura, BLUE);
    DrawRectangleLines(x, yEnergia, barLargura, barAltura, WHITE);
    DrawText("MP", x + barLargura + 8, yEnergia + 2, 14, WHITE);

    // --- ATAQUE E DEFESA ---
    int yInferior = tela.altura - 60;
    DrawText(TextFormat("ATK: %d", ataque), x, yInferior, 18, WHITE);
    DrawText(TextFormat("DEF: %d", defesa), x, yInferior + 25, 18, WHITE);

    // --- HABILIDADES ---
    int yHab = yInferior + 60;
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