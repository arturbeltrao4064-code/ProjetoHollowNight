#ifndef BOSS_H
#define BOSS_H

#include "estruturas.h" // Ajuste o caminho se sua pasta de estruturas tiver outro nome

// Declaração do Boss como uma variável global para o jogo inteiro conhecer
extern infoEntidade chefao;
extern bool bossAtivo;
extern bool bossPodeReceberDano;

// Funções principais do Boss
void loadBoss();
void unloadBoss();
void updateBoss();
void desenhaBoss();
Vector2 movimentaBoss(Vector2 posicaoAtual);

#endif