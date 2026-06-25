#include "personagem.h"
#include "mapa.h"

void inicializaAmuletos() {
    for (int i = 0; i < TOTAL_AMULETOS; i++) {
        personagem.dados.amuletos[i].coletado = false;
    }
}

void coletaAmuleto(TipoAmuleto tipo) {
    if (!personagem.dados.amuletos[tipo].coletado) {
        personagem.dados.amuletos[tipo].coletado = true;
        personagem.dados.amuletosColetados++;

        switch (tipo) {
            case AMULETO_ATAQUE:
                personagem.dados.valorAtaque += 10;
                break;
            case AMULETO_DEFESA:
                personagem.dados.valorDefesa += 5;
                break;
            case AMULETO_VIDA:
                personagem.dados.hp += 20;
                break;
        }
    }
}

void coletaHabilidade() {
    if (personagem.dados.habilidadesColetadas < 3) {
        personagem.dados.habilidadesColetadas++;
    }
}

void verificaColisaoAmuletos() {
    float x = personagem.posicao.x;
    float y = personagem.posicao.y;
    float w = (float)personagem.largura;
    float h = (float)personagem.altura;

    int col = (int)((x + w / 2) / bloco.largura);
    int lin = (int)((y + h / 2) / bloco.altura);

    if (lin >= 0 && lin < map.linhas && col >= 0 && col < map.colunas) {
        char c = map.matrizMapa[lin][col];
        if (c == 'A') {
            TipoAmuleto tipo = (TipoAmuleto)(personagem.dados.amuletosColetados % TOTAL_AMULETOS);
            coletaAmuleto(tipo);
            map.matrizMapa[lin][col] = ' ';
        }
    }
}

void verificaColisaoHabilidades() {
    float x = personagem.posicao.x;
    float y = personagem.posicao.y;
    float w = (float)personagem.largura;
    float h = (float)personagem.altura;

    int col = (int)((x + w / 2) / bloco.largura);
    int lin = (int)((y + h / 2) / bloco.altura);

    if (lin >= 0 && lin < map.linhas && col >= 0 && col < map.colunas) {
        char c = map.matrizMapa[lin][col];
        if (c == 'H') {
            coletaHabilidade();
            map.matrizMapa[lin][col] = ' ';
        }
    }
}
