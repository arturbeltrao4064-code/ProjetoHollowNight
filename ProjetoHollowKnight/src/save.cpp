#define _CRT_SECURE_NO_WARNINGS
#include "save.h"
#include "estruturas.h"
#include <stdio.h>

void salvaJogo() {
    FILE* arquivo = fopen("save/savegame.txt", "w");
    if (arquivo == NULL) {
        TraceLog(LOG_ERROR, "Erro ao criar arquivo de save");
        return;
    }

    // Salva vida e MP
    fprintf(arquivo, "%d\n", personagem.dados.hp);
    fprintf(arquivo, "%d\n", personagem.dados.mp);
    
    // Salva fase atual
    fprintf(arquivo, "%d\n", faseDoJogo);
    
    // Salva habilidades coletadas
    fprintf(arquivo, "%d\n", personagem.dados.habilidadesColetadas);
    
    // Salva amuletos coletados (3 amuletos)
    for (int i = 0; i < TOTAL_AMULETOS; i++) {
        fprintf(arquivo, "%d\n", personagem.dados.amuletos[i].coletado ? 1 : 0);
    }

    fclose(arquivo);
    TraceLog(LOG_INFO, "Jogo salvo com sucesso!");
}

void carregaJogo() {
    FILE* arquivo = fopen("save/savegame.txt", "r");
    if (arquivo == NULL) {
        TraceLog(LOG_WARNING, "Nenhum save encontrado");
        return;
    }

    // Carrega vida e MP
    fscanf(arquivo, "%d", &personagem.dados.hp);
    fscanf(arquivo, "%d", &personagem.dados.mp);
    
    // Carrega fase
    int faseCarregada;
    fscanf(arquivo, "%d", &faseCarregada);
    faseDoJogo = (FaseAtual)faseCarregada;
    
    // Carrega habilidades
    fscanf(arquivo, "%d", &personagem.dados.habilidadesColetadas);
    
    // Carrega amuletos
    for (int i = 0; i < TOTAL_AMULETOS; i++) {
        int coletado;
        fscanf(arquivo, "%d", &coletado);
        personagem.dados.amuletos[i].coletado = (coletado == 1);
    }

    fclose(arquivo);
    TraceLog(LOG_INFO, "Jogo carregado com sucesso!");
}