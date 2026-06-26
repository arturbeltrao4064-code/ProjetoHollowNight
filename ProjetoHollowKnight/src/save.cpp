#define _CRT_SECURE_NO_WARNINGS
#include "save.h"
#include "estruturas.h"
#include <stdio.h>

void salvaJogo() {
    FILE* arquivo = fopen("data/savegame.txt", "w");
    if (arquivo == NULL) {
        TraceLog(LOG_ERROR, "Erro ao salvar");
        return;
    }

    fprintf(arquivo, "%d\n", personagem.dados.hp);
    fprintf(arquivo, "%d\n", personagem.dados.mp);
    fprintf(arquivo, "%d\n", personagem.dados.flask);
    fprintf(arquivo, "%d\n", faseDoJogo);
    fprintf(arquivo, "%d\n", personagem.dados.habilidadesColetadas);
    
    for (int i = 0; i < TOTAL_AMULETOS; i++) {
        fprintf(arquivo, "%d\n", personagem.dados.amuletos[i].coletado ? 1 : 0);
    }

    fprintf(arquivo, "%d\n", proximoTunel);

    fclose(arquivo);
}

void carregaJogo() {
    FILE* arquivo = fopen("data/savegame.txt", "r");
    if (arquivo == NULL) {
        return;
    }

    fscanf(arquivo, "%d", &personagem.dados.hp);
    if (personagem.dados.hp > 5) {
        // Compatibilidade com saves antigos baseados em HP percentual.
        personagem.dados.hp = 5;
    }
    if (personagem.dados.hp < 0) personagem.dados.hp = 0;

    fscanf(arquivo, "%d", &personagem.dados.mp);
    if (personagem.dados.mp < 0) personagem.dados.mp = 0;
    if (personagem.dados.mp > 100) personagem.dados.mp = 100;

    int linha3 = 0;
    int faseCarregada = 0;
    if (fscanf(arquivo, "%d", &linha3) != 1) {
        linha3 = 100;
    }

    int linha4 = 0;
    if (fscanf(arquivo, "%d", &linha4) != 1) {
        linha4 = 0;
    }

    // Save novo: linha3=flask e linha4=fase. Save antigo: linha3=fase e linha4=habilidades.
    if (linha3 >= 0 && linha3 <= 100 && linha4 >= 0 && linha4 <= 3) {
        personagem.dados.flask = linha3;
        faseCarregada = linha4;
        fscanf(arquivo, "%d", &personagem.dados.habilidadesColetadas);
    } else {
        personagem.dados.flask = 100;
        faseCarregada = linha3;
        personagem.dados.habilidadesColetadas = linha4;
    }

    if (personagem.dados.flask < 0) personagem.dados.flask = 0;
    if (personagem.dados.flask > 100) personagem.dados.flask = 100;

    faseDoJogo = (FaseAtual)faseCarregada;
    
    personagem.dados.amuletosColetados = 0;
    for (int i = 0; i < TOTAL_AMULETOS; i++) {
        int coletado;
        fscanf(arquivo, "%d", &coletado);
        personagem.dados.amuletos[i].coletado = (coletado == 1);
        if (personagem.dados.amuletos[i].coletado) {
            personagem.dados.amuletosColetados++;
        }
    }

    int tunelLido = 0;
    if (fscanf(arquivo, "%d", &tunelLido) == 1 && tunelLido >= 1 && tunelLido <= 4) {
        proximoTunel = tunelLido;
    } else {
        // Compatibilidade com saves antigos sem o campo de progresso.
        switch (faseDoJogo) {
            case FASE_INICIAL: proximoTunel = 1; break;
            case FASE_FINAL: proximoTunel = 2; break;
            case FASE_TUNEL3: proximoTunel = 3; break;
            case FASE_VILA:
            default:
                proximoTunel = personagem.dados.amuletosColetados + 1;
                if (proximoTunel < 1) proximoTunel = 1;
                if (proximoTunel > 4) proximoTunel = 4;
                break;
        }
    }

    fclose(arquivo);
}