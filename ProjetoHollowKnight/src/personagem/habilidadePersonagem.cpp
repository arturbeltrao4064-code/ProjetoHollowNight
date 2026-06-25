#include "personagem.h"
#include <raylib.h>

void disparaHabilidade() {
    if (IsKeyPressed(KEY_D) && personagem.dados.habilidadesColetadas > 0 && personagem.dados.mp >= 20) {
        personagem.dados.mp -= 20;
        personagem.dados.habilidadesColetadas--;
        personagem.dados.habilidadeAtiva.ativo = true;
        personagem.dados.habilidadeAtiva.posicao = personagem.posicao;
        personagem.dados.habilidadeAtiva.velocidade = 8.0f;
        personagem.dados.habilidadeAtiva.direcao = personagem.olhandoDireita;
    }
}

void atualizaHabilidade() {
    if (personagem.dados.habilidadeAtiva.ativo) {
        float dx = personagem.dados.habilidadeAtiva.direcao ? personagem.dados.habilidadeAtiva.velocidade : -personagem.dados.habilidadeAtiva.velocidade;
        personagem.dados.habilidadeAtiva.posicao.x += dx;
        if (personagem.dados.habilidadeAtiva.posicao.x < 0 || personagem.dados.habilidadeAtiva.posicao.x > map.colunas * bloco.largura) {
            personagem.dados.habilidadeAtiva.ativo = false;
        }
    }
}
