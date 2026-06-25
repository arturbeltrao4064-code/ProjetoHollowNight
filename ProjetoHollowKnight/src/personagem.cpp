#include "personagem.h"
#include "mapa.h"
#include <raylib.h>

void loadPersonagem() {
    personagem.imagem[0] = LoadTexture("Texturas/Personagem/Personagem.png");
    personagem.imagem[1] = LoadTexture("Texturas/Personagem/InvertePersonagem.png");
    inicializaAmuletos();
}

void unloadPersonagem() {
    UnloadTexture(personagem.imagem[0]);
    UnloadTexture(personagem.imagem[1]);
}

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
 
void verificaColisaoAmuletos() {
    float x = personagem.posicao.x;
    float y = personagem.posicao.y;
    float w = (float)personagem.largura;
    float h = (float)personagem.altura;
    
    // Verifica colisão com amuletos no mapa
    int col = (int)((x + w / 2) / bloco.largura);
    int lin = (int)((y + h / 2) / bloco.altura);
    
    if (lin >= 0 && lin < map.linhas && col >= 0 && col < map.colunas) {
        char c = map.matrizMapa[lin][col];
        
        if (c == 'A') {
            // Determina qual amuleto baseado na posição (você pode melhorar isso depois)
            TipoAmuleto tipo = (TipoAmuleto)(personagem.dados.amuletosColetados % TOTAL_AMULETOS);
            coletaAmuleto(tipo);
            
            // Remove o amuleto do mapa
            map.matrizMapa[lin][col] = ' ';
        }
    }
}
void coletaHabilidade() {
    if (personagem.dados.habilidadesColetadas < 3) {
        personagem.dados.habilidadesColetadas++;
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

void disparaHabilidade() {
    if (IsKeyPressed(KEY_Z) && personagem.dados.habilidadesColetadas > 0 && personagem.dados.mp >= 20) {
        personagem.dados.mp -= 20;
        personagem.dados.habilidadesColetadas--;
        
        personagem.dados.habilidadeAtiva.ativo = true;
        personagem.dados.habilidadeAtiva.posicao = personagem.posicao;
        personagem.dados.habilidadeAtiva.velocidade = 8.0f;
        personagem.dados.habilidadeAtiva.direcao = personagem.olhandoDireita;
    }
}



// Variável local para controlar quanto tempo o boneco fica rosa atacando
static float tempoAtaque = 0.0f;

void updatePersonagem() {
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) personagem.olhandoDireita = true;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  personagem.olhandoDireita = false;

    // --- SISTEMA DE ATIVAÇÃO DO ATAQUE ---
    // Ativa o ataque ao pressionar o botão esquerdo do mouse ou a tecla J
    if (IsKeyPressed(KEY_X) && !personagem.dados.ataque) {
        personagem.dados.ataque = true;
        tempoAtaque = 0.2f; // O ataque dura 0.2 segundos (cerca de 12 frames a 60fps)
    }

    // Gerencia o tempo do ataque
    if (personagem.dados.ataque) {
        tempoAtaque -= GetFrameTime();
        if (tempoAtaque <= 0.0f) {
            personagem.dados.ataque = false; // Fim do ataque
        }
    }

    personagem.posicao = movimentaPersonagem(personagem.posicao);
    verificaColisaoAmuletos();
    verificaColisaoAmuletos();
    verificaColisaoHabilidades();
    disparaHabilidade();
    
    // Atualiza habilidade ativa
    if (personagem.dados.habilidadeAtiva.ativo) {
        float dx = personagem.dados.habilidadeAtiva.direcao ? personagem.dados.habilidadeAtiva.velocidade : -personagem.dados.habilidadeAtiva.velocidade;
        personagem.dados.habilidadeAtiva.posicao.x += dx;
        
        // Remove se sair do mapa
        if (personagem.dados.habilidadeAtiva.posicao.x < 0 || personagem.dados.habilidadeAtiva.posicao.x > map.colunas * bloco.largura) {
            personagem.dados.habilidadeAtiva.ativo = false;
        }
    }
}

void desenhaPersonagem() {
    // SE ESTIVER ATACANDO: Fica ROSA (PINK). SE NÃO: Fica VERDE (GREEN)
    Color corAtual = personagem.dados.ataque ? PINK : GREEN;
    
    DrawRectangle((int)personagem.posicao.x, (int)personagem.posicao.y, personagem.largura, personagem.altura, corAtual);
}

Vector2 movimentaPersonagem(Vector2 posicaoAtual) {
    float x = posicaoAtual.x;
    float y = posicaoAtual.y;
    float w = (float)personagem.largura;
    float h = (float)personagem.altura;

    // --- 1. MOVIMENTO HORIZONTAL ---
    float dx = 0;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) dx =  constantesJogo.velocidade;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  dx = -constantesJogo.velocidade;

    if (dx != 0) {
        x += dx;
        // Verifica colisão lateral: topo e base do personagem
        float checkX = (dx > 0) ? (x + w) : x; // borda que vai colidir
        bool colideEsq = blocoSolido(checkX, y + 2);           // topo
        bool colideDir = blocoSolido(checkX, y + h - 2);       // base

        if (colideEsq || colideDir) {
            if (dx > 0) {
                // Empurra para a esquerda do bloco
                int col = (int)((x + w) / bloco.largura);
                x = col * bloco.largura - w - 0.1f;
            } else {
                // Empurra para a direita do bloco
                int col = (int)(x / bloco.largura);
                x = (col + 1) * bloco.largura + 0.1f;
            }
        }
    }

    // --- 2. GRAVIDADE E MOVIMENTO VERTICAL ---
    // CORRIGIDO: Agora usa 'constantesJogo.velocidadeY' em vez de 'personagem.velocidadeY'
    constantesJogo.velocidadeY += constantesJogo.gravidade;
    y += constantesJogo.velocidadeY;

    // Colisão com o chão (borda inferior esquerda e direita)
    if (constantesJogo.velocidadeY >= 0) {
        bool colideChaoEsq = blocoSolido(x + 2,       y + h);
        bool colideChaoDir = blocoSolido(x + w - 2,   y + h);

        if (colideChaoEsq || colideChaoDir) {
            int lin = (int)((y + h) / bloco.altura);
            y = lin * bloco.altura - h - 0.1f;
            constantesJogo.velocidadeY = 0; // Para de cair
        }
    }

    // Colisão com o teto (borda superior esquerda e direita)
    if (constantesJogo.velocidadeY < 0) {
        bool colideTeto1 = blocoSolido(x + 2,     y);
        bool colideTeto2 = blocoSolido(x + w - 2, y);

        if (colideTeto1 || colideTeto2) {
            int lin = (int)(y / bloco.altura);
            y = (lin + 1) * bloco.altura + 0.1f;
            constantesJogo.velocidadeY = 0; // Zera a velocidade ao bater a cabeça
        }
    }

    // --- 3. PULO (só quando está no chão) ---
    bool noChaoEsq = blocoSolido(x + 2,     y + h + 1);
    bool noChaoDir = blocoSolido(x + w - 2, y + h + 1);
    bool noChao    = noChaoEsq || noChaoDir;

    if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_SPACE)) && noChao) {
        constantesJogo.velocidadeY = constantesJogo.forcaPulo;
    }
    return {x, y};
}
