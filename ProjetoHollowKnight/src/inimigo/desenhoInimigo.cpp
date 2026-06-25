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
    for (int i = 0; i < quantidadeInimigos; i++) {
        if (listaInimigos[i].dados.vivo) {
            DrawRectangle((int)listaInimigos[i].posicao.x, (int)listaInimigos[i].posicao.y,
                         listaInimigos[i].largura, listaInimigos[i].altura, RED);
        }
    }
}
