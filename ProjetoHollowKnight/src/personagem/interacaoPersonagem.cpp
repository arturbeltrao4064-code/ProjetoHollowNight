#include "personagem.h"
#include "mapa.h"

static int proximoAmuletoNaoColetado() {
    for (int i = 0; i < TOTAL_AMULETOS; i++) {
        if (!personagem.dados.amuletos[i].coletado) {
            return i;
        }
    }
    return -1;
}

void inicializaAmuletos() {
    for (int i = 0; i < TOTAL_AMULETOS; i++) {
        personagem.dados.amuletos[i].coletado = false;
    }
    personagem.dados.hpMax = 5;
    personagem.dados.amuletaEquipado = -1;
    personagem.dados.valorAtaque = 1;
    constantesJogo.velocidade = 8.0f;
}

void coletaAmuleto(TipoAmuleto tipo) {
    // Apenas marca como coletado; efeito e aplicado ao equipar no inventario.
    if (!personagem.dados.amuletos[tipo].coletado) {
        personagem.dados.amuletos[tipo].coletado = true;
        personagem.dados.amuletosColetados++;
    }
}

void equipaAmuleto(int tipo) {
    // Remove efeito do amuleto anterior
    switch (personagem.dados.amuletaEquipado) {
        case AMULETO_ATAQUE:
            personagem.dados.valorAtaque = 1;
            break;
        case AMULETO_DEFESA:
            constantesJogo.velocidade = 8.0f;
            break;
        case AMULETO_VIDA:
            personagem.dados.hpMax = 5;
            if (personagem.dados.hp > 5) personagem.dados.hp = 5;
            break;
    }

    // Toggle: clicar no equipado atual desequipa
    if (personagem.dados.amuletaEquipado == tipo) {
        personagem.dados.amuletaEquipado = -1;
        return;
    }

    // Equipa novo amuleto
    personagem.dados.amuletaEquipado = tipo;
    switch (tipo) {
        case AMULETO_ATAQUE:
            personagem.dados.valorAtaque = 2;
            break;
        case AMULETO_DEFESA:
            constantesJogo.velocidade = 12.0f;
            break;
        case AMULETO_VIDA:
            personagem.dados.hpMax = 6;
            break;
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
            // 'A' funciona como marcador generico: concede sempre o proximo amuleto faltante.
            int tipo = proximoAmuletoNaoColetado();
            if (tipo >= 0) {
                coletaAmuleto((TipoAmuleto)tipo);
                map.matrizMapa[lin][col] = ' ';
            }
        } else if (c == 'E') {
            coletaAmuleto(AMULETO_DEFESA);
            map.matrizMapa[lin][col] = ' ';
        } else if (c == 'L') {
            coletaAmuleto(AMULETO_VIDA);
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
