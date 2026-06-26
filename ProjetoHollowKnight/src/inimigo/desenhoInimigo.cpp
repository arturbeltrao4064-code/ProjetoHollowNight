#include "inimigo.h"
#include <raylib.h>

void updateInimigo() {
    for (int i = 0; i < quantidadeInimigos; i++) {
        if (listaInimigos[i].dados.vivo) {
            listaInimigos[i].posicao = movimentaInimigo(i, listaInimigos[i].posicao);
        }
    }
}

void desenhaInimigo() {
    const float spriteW = 80.0f;
    const float spriteH = 80.0f;

    for (int i = 0; i < quantidadeInimigos; i++) {
        if (listaInimigos[i].dados.vivo) {
            Texture2D texturaAtual = listaInimigos[i].olhandoDireita ? listaInimigos[i].imagem[0] : listaInimigos[i].imagem[1];
            if (texturaAtual.width > 0 && texturaAtual.height > 0) {
                Rectangle src = { 0.0f, 0.0f, (float)texturaAtual.width, (float)texturaAtual.height };
                float drawX = listaInimigos[i].posicao.x + ((float)listaInimigos[i].largura - spriteW) * 0.5f;
                float drawY = listaInimigos[i].posicao.y + (float)listaInimigos[i].altura - spriteH + 28.0f;
                Rectangle dst = { drawX, drawY, spriteW, spriteH };
                DrawTexturePro(texturaAtual, src, dst, { 0, 0 }, 0.0f, WHITE);
            } else {
                DrawRectangle((int)listaInimigos[i].posicao.x, (int)listaInimigos[i].posicao.y,
                             listaInimigos[i].largura, listaInimigos[i].altura, RED);
            }
        }
    }
}
