#include "game.h"
#include "mapa.h"
#include "personagem.h"
#include "inimigo.h"
#include "boss.h"
#include <raylib.h>
#include <stddef.h>
#include <stdio.h>

FaseAtual faseDoJogo = FASE_VILA;

void inicializaPosicoesEntidades() {
    if (!map.matrizMapa) return;

    quantidadeInimigos = 0;

    for (int i = 0; i < map.linhas; i++) {
        for (int j = 0; j < map.colunas; j++) {
            char c = map.matrizMapa[i][j];
            float posX = j * bloco.largura;
            float posY = i * bloco.altura;

            if (c == 'J') {
                personagem.posicao = (Vector2){ posX, posY };
                personagem.posicaoInicial = (Vector2){ posX, posY };
            }
            else if (c == 'M') {
                if (quantidadeInimigos < MAX_INIMIGOS) {
                    listaInimigos[quantidadeInimigos].posicao = (Vector2){ posX, posY };
                    listaInimigos[quantidadeInimigos].posicaoInicial = (Vector2){ posX, posY };
                    listaInimigos[quantidadeInimigos].largura = 30;
                    listaInimigos[quantidadeInimigos].altura = 30;
                    listaInimigos[quantidadeInimigos].olhandoDireita = true;
                    listaInimigos[quantidadeInimigos].dados.hp = 100;
                    listaInimigos[quantidadeInimigos].dados.mp = 0;
                    listaInimigos[quantidadeInimigos].dados.vivo = true;

                    quantidadeInimigos++;
                }
            }
            else if (c == 'C') {
                chefao.posicao = (Vector2){ posX, posY };
                chefao.posicaoInicial = (Vector2){ posX, posY };
                chefao.largura = 60;
                chefao.altura = 60;
                chefao.dados.hp = 500;
                chefao.dados.vivo = true;
                bossAtivo = true;
                TraceLog(LOG_INFO, "Boss 'C' carregado na posicao: %.2f, %.2f", posX, posY);
            }
        }
    }

    TraceLog(LOG_INFO, "Total de inimigos carregados com sucesso: %d", quantidadeInimigos);
}

void loadJogo() {
    switch (faseDoJogo) {
        case FASE_VILA:
            map.localMapa = "maps/mapaVila.txt";
            break;
        case FASE_INICIAL:
            map.localMapa = "maps/mapaInicial.txt";
            break;
        case FASE_FINAL:
            map.localMapa = "maps/mapaFinal.txt";
            break;
    }

    loadMapa();
    inicializaPosicoesEntidades();
    loadInimigo();
    loadBoss();
    tela.fundoJogo = LoadTexture("Fundos/Jogo/FundoJogo.png");
}

void unloadJogo() {
    unloadMapa();
    unloadPersonagem();
    unloadInimigo();
    unloadBoss();
    UnloadTexture(tela.fundoJogo);
}
