#include "game.h"
#include "mapa.h"
#include "personagem.h"
#include "inimigo.h"
#include "boss.h"
#include <raylib.h>
#include <stddef.h>
#include <stdio.h>

FaseAtual faseDoJogo = FASE_VILA;
extern float flaskCarga;

static const int HP_INIMIGO_NORMAL = 3;
static const int HP_BOSS = HP_INIMIGO_NORMAL * 2;

const char* caminhoMapaPorFase(FaseAtual fase) {
    switch (fase) {
        case FASE_VILA: return "maps/vila.txt";
        case FASE_INICIAL: return "maps/tunel1.txt";
        case FASE_FINAL: return "maps/tunel2.txt";
        case FASE_TUNEL3: return "maps/tunel3.txt";
        default: return "maps/vila.txt";
    }
}

static const char* caminhoFundoPorFase(FaseAtual fase) {
    if (fase == FASE_VILA) return "assets/Cenario/fundoVila.png";
    return "assets/Cenario/fundoCenario.png";
}

void atualizarFundoJogo() {
    if (tela.fundoJogo.width > 0) {
        UnloadTexture(tela.fundoJogo);
    }
    tela.fundoJogo = LoadTexture(caminhoFundoPorFase(faseDoJogo));
}

void carregarMapaAtualComEntidades() {
    loadMapa();
    inicializaPosicoesEntidades();
}

void resetarEstadoNovoJogo() {
    personagem.dados.hp = 5;
    personagem.dados.hpMax = 5;
    personagem.dados.mp = 100;
    flaskCarga = 100.0f;
    personagem.dados.flask = 100;
    personagem.dados.habilidadesColetadas = 0;
    personagem.dados.amuletosColetados = 0;
    for (int i = 0; i < TOTAL_AMULETOS; i++) {
        personagem.dados.amuletos[i].coletado = false;
    }
    personagem.dados.ataque = false;
    personagem.dados.vivo = true;
    personagem.dados.habilidadeAtiva.ativo = false;
    faseDoJogo = FASE_VILA;
    proximoTunel = 1;
}

void inicializaPosicoesEntidades() {
    if (!map.matrizMapa) return;

    quantidadeInimigos = 0;

    // Reseta estado do boss antes de ler o mapa; ele so deve existir se houver 'C'.
    bossState.ativo = false;
    bossState.movendo = false;
    bossState.preparando = false;
    bossState.countdown = 3.0f;
    bossState.velY = 0.0f;
    bossState.jumpCooldown = 0.0f;
    bossState.damageCooldown = 0.0f;
    bossState.podeReceberDano = false;
    bossState.entidade.dados.vivo = false;
    bossState.entidade.dados.hp = 0;
    bossState.entidade.dados.hpMax = 0;

    for (int i = 0; i < map.linhas; i++) {
        for (int j = 0; j < map.colunas; j++) {
            char c = map.matrizMapa[i][j];
            float posX = j * bloco.largura;
            float posY = i * bloco.altura;

            if (c == 'J') {
                personagem.posicao = (Vector2){ posX, posY };
                personagem.posicaoInicial = (Vector2){ posX, posY };
                // Inicia cada fase com flask cheio.
                personagem.dados.flask = 100;
            }
            else if (c == 'M') {
                if (quantidadeInimigos < MAX_INIMIGOS) {
                    listaInimigos[quantidadeInimigos].posicao = (Vector2){ posX, posY };
                    listaInimigos[quantidadeInimigos].posicaoInicial = (Vector2){ posX, posY };
                    // Hitbox menor para colisao estavel; sprite continua grande no desenho.
                    listaInimigos[quantidadeInimigos].largura = 34;
                    listaInimigos[quantidadeInimigos].altura = 40;
                    listaInimigos[quantidadeInimigos].olhandoDireita = true;
                    // Sistema por hits: cada inimigo aguenta 3 acertos.
                    listaInimigos[quantidadeInimigos].dados.hp = HP_INIMIGO_NORMAL;
                    listaInimigos[quantidadeInimigos].dados.hpMax = HP_INIMIGO_NORMAL;
                    listaInimigos[quantidadeInimigos].dados.mp = 0;
                    listaInimigos[quantidadeInimigos].dados.vivo = true;

                    quantidadeInimigos++;
                }
            }
            else if (c == 'C') {
                float posYBoss = posY + 10.0f;
                bossState.entidade.posicao = (Vector2){ posX, posYBoss };
                bossState.entidade.posicaoInicial = (Vector2){ posX, posYBoss };
                bossState.entidade.largura = 50;
                bossState.entidade.altura = 50;
                bossState.movendo = false;
                bossState.preparando = false;
                bossState.countdown = 3.0f;
                bossState.velY = 0.0f;
                bossState.jumpCooldown = 0.0f;
                bossState.damageCooldown = 0.0f;
                bossState.entidade.dados.hp = HP_BOSS;
                bossState.entidade.dados.hpMax = HP_BOSS;
                bossState.entidade.dados.vivo = true;
                bossState.ativo = true;
                TraceLog(LOG_INFO, "Boss 'C' carregado na posicao: %.2f, %.2f", posX, posY);
            }
        }
    }

    TraceLog(LOG_INFO, "Total de inimigos carregados com sucesso: %d", quantidadeInimigos);
}

void loadJogo() {
    map.localMapa = caminhoMapaPorFase(faseDoJogo);
    carregarMapaAtualComEntidades();
    loadPersonagem();
    loadInimigo();
    loadBoss();
    atualizarFundoJogo();
}

void unloadJogo() {
    unloadMapa();
    unloadPersonagem();
    unloadInimigo();
    unloadBoss();
    UnloadTexture(tela.fundoJogo);
}
