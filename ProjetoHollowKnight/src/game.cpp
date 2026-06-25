#include "game.h"
#include "mapa.h"
#include "personagem.h"
#include "inimigo.h"
#include <raylib.h>
#include "hud.h"
#include "boss.h"
#include <stdio.h>
#include <string.h>

// Definição da variável global de controle de fase
FaseAtual faseDoJogo = FASE_VILA;

void inicializaPosicoesEntidades() {
    if (map.matrizMapa == NULL) return;

    quantidadeInimigos = 0; // Reseta o contador ao carregar o cenário

    for (int i = 0; i < map.linhas; i++) {
        for (int j = 0; j < map.colunas; j++) {
            char c = map.matrizMapa[i][j];
            float posX = j * bloco.largura;
            float posY = i * bloco.altura;

            if (c == 'J') {
                personagem.posicao = { posX, posY };
                personagem.posicaoInicial = { posX, posY };
            }
            else if (c == 'M') {
                if (quantidadeInimigos < MAX_INIMIGOS) {
                    listaInimigos[quantidadeInimigos].posicao = { posX, posY };
                    listaInimigos[quantidadeInimigos].posicaoInicial = { posX, posY };
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
                chefao.posicao = { posX, posY };
                chefao.posicaoInicial = { posX, posY };
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
    loadMapa(); // Chama a função que agora está no seu arquivo de mapa para medir e carregar o txt
    inicializaPosicoesEntidades(); 
    loadInimigo();
    loadBoss(); 
    tela.fundoJogo = LoadTexture("Texturas/Fundos/Jogo/FundoJogo.png");
}

void unloadJogo() {
    unloadMapa();
    unloadPersonagem();
    unloadInimigo();
    unloadBoss();
    UnloadTexture(tela.fundoJogo); 
}

void gerenciarTransicaoFases() {
    // Como map.colunas agora é medido dinamicamente no mapa.c, esta largura estará sempre correta
    float mapaLargura = map.colunas * bloco.largura;
    bool mudouDeFase = false;

    // --- AVANÇAR DE FASE (Apenas Borda Direita) ---
    if ((personagem.posicao.x + personagem.largura) >= (mapaLargura - 45.0f)) {
        if (faseDoJogo == FASE_VILA) {
            faseDoJogo = FASE_INICIAL;
            map.localMapa = "maps/mapaInicial.txt";
            mudouDeFase = true;
        } 
        else if (faseDoJogo == FASE_INICIAL) {
            faseDoJogo = FASE_FINAL;
            map.localMapa = "maps/mapaFinal.txt";
            mudouDeFase = true;
        }
    }

    // Se houve mudança de cenário, reinicializa o ambiente
    if (mudouDeFase) {
        TraceLog(LOG_INFO, ">>> TROCA DE FASE! Carregando novo mapa: %s", map.localMapa);

        unloadMapa(); // Desaloca a matriz do mapa antigo
        
        // Limpa estados antigos das entidades para evitar bugs visuais
        quantidadeInimigos = 0;
        bossAtivo = false;
        chefao.dados.vivo = false;

        loadMapa(); // Abre o novo .txt e calcula linhas/colunas automaticamente
        
        // Esta função vai ler o novo arquivo, achar onde está o caractere 'J' 
        // e colocar as coordenadas do personagem exatamente em cima dele!
        inicializaPosicoesEntidades(); 
    }
}

void updateJogo() {
    updatePersonagem();
    updateInimigo();
    updateBoss();

    Rectangle rectPlayer = { personagem.posicao.x, personagem.posicao.y, (float)personagem.largura, (float)personagem.altura };

    // --- LÓGICA DE ATAQUE DO PERSONAGEM (CAUSAR DANO) ---
    Rectangle rectAtaque = { 0 };
    if (personagem.dados.ataque) {
        float alcanceAtaque = 40.0f; 
        
        if (personagem.olhandoDireita) {
            rectAtaque = { personagem.posicao.x + personagem.largura, personagem.posicao.y, alcanceAtaque, (float)personagem.altura };
        } else {
            rectAtaque = { personagem.posicao.x - alcanceAtaque, personagem.posicao.y, alcanceAtaque, (float)personagem.altura };
        }
    }

    // --- LOOP DE INTERAÇÃO COM TODOS OS INIMIGOS ---
    for (int i = 0; i < quantidadeInimigos; i++) {
        if (listaInimigos[i].dados.vivo) {
            Rectangle rectInimigo = { listaInimigos[i].posicao.x, listaInimigos[i].posicao.y, (float)listaInimigos[i].largura, (float)listaInimigos[i].altura };
            //se ta atacando e colidiu com o inimigo, causa dano
            if (personagem.dados.ataque && CheckCollisionRecs(rectAtaque, rectInimigo)) {
                listaInimigos[i].dados.hp -= personagem.dados.valorAtaque;
                personagem.dados.ataque = false; 

                if (listaInimigos[i].dados.hp <= 0) {
                    listaInimigos[i].dados.hp = 0;
                    listaInimigos[i].dados.vivo = false; // Marca o inimigo como morto
                }
                
                float direcaoInimigoEmpurrado = (personagem.posicao.x < listaInimigos[i].posicao.x) ? 1.0f : -1.0f;
                listaInimigos[i].posicao.x += direcaoInimigoEmpurrado * 15.0f;
                
                break; 
            }

            if (!personagem.dados.ataque && CheckCollisionRecs(rectPlayer, rectInimigo)) {
                personagem.dados.hp -= (20 - personagem.dados.valorDefesa); // Dano base 20 menos defesa do personagem
                if (personagem.dados.hp <= 0) {
                    personagem.dados.hp = 0;
                    personagem.dados.vivo = false; 
                }

                float direcaoKnockback = (personagem.posicao.x < listaInimigos[i].posicao.x) ? -1.0f : 1.0f;
                personagem.posicao.x += direcaoKnockback * 20.0f; 
                constantesJogo.velocidadeY = -6.0f; 

                break; 
            }
        }
    }
    if (personagem.dados.habilidadeAtiva.ativo) {
        Rectangle rectHab = { personagem.dados.habilidadeAtiva.posicao.x, personagem.dados.habilidadeAtiva.posicao.y, 7, 7 };
    
        for (int i = 0; i < quantidadeInimigos; i++) {
            if (listaInimigos[i].dados.vivo) {
                Rectangle rectInimigo = { listaInimigos[i].posicao.x, listaInimigos[i].posicao.y, (float)listaInimigos[i].largura, (float)listaInimigos[i].altura };
                
                if (CheckCollisionRecs(rectHab, rectInimigo)) {
                    listaInimigos[i].dados.hp -= 50;
                    personagem.dados.habilidadeAtiva.ativo = false;
                    
                    if (listaInimigos[i].dados.hp <= 0) {
                        listaInimigos[i].dados.hp = 0;
                        listaInimigos[i].dados.vivo = false;
                    }
                    break;
                }
            }
        }
    }

    // --- ATUALIZAÇÃO DA CÂMERA ---
    tela.camera.target = {
        personagem.posicao.x + personagem.largura / 2.0f,
        personagem.posicao.y + personagem.altura / 2.0f
    };
    tela.camera.offset = { tela.largura / 2.0f, tela.altura / 2.0f };

    float mapaLargura = map.colunas * bloco.largura;
    float mapaAltura  = map.linhas  * bloco.altura;
    float margemH     = (tela.largura  / 2.0f) / tela.camera.zoom;
    float margemV     = (tela.altura   / 2.0f) / tela.camera.zoom;

    if (tela.camera.target.x < margemH)               tela.camera.target.x = margemH;
    if (tela.camera.target.x > mapaLargura - margemH) tela.camera.target.x = mapaLargura - margemH;
    if (tela.camera.target.y < margemV)               tela.camera.target.y = margemV;
    if (tela.camera.target.y > mapaAltura  - margemV) tela.camera.target.y = mapaAltura  - margemV;

    // --- GATILHO DE FLUXO DE FASES ---
    gerenciarTransicaoFases();
}

void drawFundo() {
    float fw = (float)tela.fundoJogo.width;
    float fh = (float)tela.fundoJogo.height;
    float mapaLargura = map.colunas * bloco.largura;
    float mapaAltura  = map.linhas  * bloco.altura;

    for (float x = 0; x < mapaLargura; x += fw) {
        for (float y = 0; y < mapaAltura; y += fh) {
            DrawTextureV(tela.fundoJogo, { x, y }, WHITE);
        }
    }
}
void desenhaHabilidade() {
    if (personagem.dados.habilidadeAtiva.ativo) {
        int tamHab = personagem.largura / 3;
        DrawRectangle((int)personagem.dados.habilidadeAtiva.posicao.x, 
                     (int)personagem.dados.habilidadeAtiva.posicao.y, 
                     tamHab, tamHab, ORANGE);
    }
}

void drawJogo() {
    ClearBackground(PURPLE);

    BeginMode2D(tela.camera);
        desenhaMapa();
        desenhaPersonagem();
        desenhaHabilidade();
        desenhaInimigo(); 
        desenhaBoss(); 
    EndMode2D();
    
    desenhaHud(personagem.dados.hp, personagem.dados.mp,personagem.dados.valorAtaque, personagem.dados.valorDefesa);
}

