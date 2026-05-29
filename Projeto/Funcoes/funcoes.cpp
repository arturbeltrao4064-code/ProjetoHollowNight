#define _CRT_SECURE_NO_WARNINGS
#include "funcoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

void saveGame(void) {
    // Lógica de salvar futuramente
}

void outGame(void) {
    // Lógica de sair
    CloseWindow();
    exit(0);
}

char** leituraMapa(infoMapa info) {
    FILE* abreMapa = fopen(info.localMapa, "r");
    if (abreMapa == NULL) return NULL;

    // Aloca as linhas
    char** matriz = (char**)malloc(info.linhas * sizeof(char*));
    for (int i = 0; i < info.linhas; i++) {
        // Aloca colunas (+2 para garantir espaço do \n e \0)
        matriz[i] = (char*)malloc((info.colunas + 5) * sizeof(char));
        if (fgets(matriz[i], info.colunas + 5, abreMapa) == NULL) {
            matriz[i][0] = '\0';
        }
    }

    fclose(abreMapa);
    return matriz;
}

void liberaMapa(char** matriz, int linhas) {
    if (matriz == NULL) return;

    // 1. Libera cada linha (as gavetas) primeiro
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    // 2. Só depois libera o ponteiro principal (a estante)
    free(matriz);
}
void startJogo(void) {
    // Atualização
    // Desenho
    BeginDrawing();
        
    ClearBackground(BLACK);

    desenhaMapa();

    EndDrawing();
}
void desenhaMapa(void) {

    // 2. Lógica para desenhar o mapa
    //Legenda rapida: 
    // J - jogador - personagem.imagem,laranja,
    // C - Chefe - roxo;
    // P - Parede - branco;
    // A - Amuleto - Amarelo;
    // H - Habilidade - Azul;
    if (map.matrizMapa != NULL) {
        for (int i = 0; i < map.linhas; i++) {
            for (int j = 0; j < map.colunas; j++) {
                // Verifica se o caractere não é espaço ou quebra de linha
                char caractere = map.matrizMapa[i][j];

                if (caractere != ' ' && caractere != '\n' && caractere != '\0' && caractere != '\r') {
                    float posX = j * bloco.largura;
                    float posY = i * bloco.altura;
                    if (caractere == 'J') {
                        DrawRectangle(posX, posY, bloco.largura - 1, bloco.altura - 1, ORANGE);
                        DrawTextureEx(personagem.imagem[0], personagem.posicao, 0, 0.3, WHITE);
                        personagem.posicao = movimentaPersoangem(personagem.posicao);
                    }
                    else if (caractere == 'C') {
                        DrawRectangle(posX, posY, bloco.largura - 1, bloco.altura - 1, PURPLE);
                    }
                    else if (caractere == 'P') {
                        // Carrega local da img, Retangulo para recortar a img(x, y, largura, altura), posicao, cor;
                        DrawTextureRec(map.mapaImagem[0],{ 0.0f, 96.0f, 32.0f, 32.0f },{posX, posY}, WHITE);
                        
                    }
                    else if (caractere == 'A') {
                        DrawRectangle(posX, posY, bloco.largura - 1, bloco.altura - 1, YELLOW);
                    }
                    else if (caractere == 'H') {
                        DrawRectangle(posX, posY, bloco.largura - 1, bloco.altura - 1, BLUE);
                    }

                }
            }
        }
    }

}



void loadArquivos(void) {
    map.matrizMapa = leituraMapa(map);
    map.mapaImagem[0] = LoadTexture("Texturas/Mapa/SpriteSheetMap.png");
    personagem.imagem[0] = LoadTexture("Texturas/Personagem/Personagem1.png");
    tela.menuImagem[0] = LoadTexture("Texturas/Fundos/Menu/FundoMenu.png");
}

void unloadArquivos(void) {
    UnloadTexture(personagem.imagem[0]);
    UnloadTexture(map.mapaImagem[0]);
    UnloadTexture(tela.menuImagem[0]);
    liberaMapa(map.matrizMapa, map.linhas);
    CloseWindow();
}

enum Estado { ESTADO_MENU, ESTADO_JOGANDO };

Estado estadoAtual = ESTADO_MENU;

void desenhaMenu(void) {


    if (estadoAtual == ESTADO_MENU) {
        if (IsKeyPressed(KEY_ENTER)) {
            estadoAtual = ESTADO_JOGANDO;
        }

        BeginDrawing();
        ClearBackground(BLACK);
        Rectangle source = { 0, 0, tela.menuImagem[0].width, tela.menuImagem[0].height };   //CENTRALIZA A IMAGEM DO MENU NOVO Q EU COLOQUEI
        Rectangle dest = { 0, 0, tela.largura, tela.altura };
        DrawTexturePro(tela.menuImagem[0], source, dest, {0}, 0.0f, WHITE);
        EndDrawing();
    }
    else if (estadoAtual == ESTADO_JOGANDO) {
        // Se estiver jogando, chama a função de lógica/desenho
        startJogo();

        // Se apertar ESC, volta para o menu
        if (IsKeyPressed(KEY_M)) {
            estadoAtual = ESTADO_MENU;
        }
    }
}

Vector2 movimentaPersoangem(Vector2 posicaoAtual) {
    float velocidade = 5.0f;
    static float velocidadey = 0; // static para lembrar o valor e nao reniciar toda vez
    velocidadey += 0.5f; // Gravidade
    posicaoAtual.y += velocidadey;

    int coluna = (int)((posicaoAtual.x + personagem.largura / 2) / bloco.largura); // Verifica a parte do meio do personagem
    int linha = (int)((posicaoAtual.y + 50 + personagem.altura) / bloco.altura); // Verifica a parte de baixo do personagem

    if (map.matrizMapa[linha][coluna] == 'P' && velocidadey >= 0) { // Colisão com o chão
        // Lógica de colisão com o chão
        velocidadey = 0; // Para o movimento vertical
        posicaoAtual.y = (linha * bloco.altura) - personagem.altura - 50; // Ajusta a posição para ficar em cima do bloco - 50 para verificar a parte de baixo do personagem
    }
    int linhameio = (int)((posicaoAtual.y + 20 + personagem.altura / 2) / bloco.altura); // Verifica a parte do meio do personagem
    int colunadireita = (int)((posicaoAtual.x + personagem.largura) / bloco.largura);
    if ((IsKeyDown(KEY_D)|| IsKeyDown(KEY_RIGHT)) && posicaoAtual.x > 0) {
        if (map.matrizMapa[linhameio][colunadireita] != 'P') {
            // Lógica de colisão com paredes
            posicaoAtual.x += velocidade; // Move para a direitad
        }
    }

    int colunaesquerda = (int)(posicaoAtual.x / bloco.largura);
    if ((IsKeyDown(KEY_A)|| IsKeyDown(KEY_LEFT)) && posicaoAtual.x < 1500) {
        if (map.matrizMapa[linhameio][colunaesquerda] != 'P') {
            posicaoAtual.x -= velocidade; // Move para a esquerda
        }
    }

    if ((IsKeyPressed(KEY_W)|| IsKeyPressed(KEY_UP)) && map.matrizMapa[linha][coluna] == 'P') {
        velocidadey = -10.0f; // Pulo
    }

    return posicaoAtual;
}
