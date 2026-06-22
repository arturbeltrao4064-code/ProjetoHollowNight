#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H
#include <raylib.h>

typedef struct {
    int largura;
    int altura;
    char titulo[20];
    Texture2D menuImagem[3];
    Texture2D menuBotoes[5];  // ← texturas dos 4 botoes: Jogar, Carregar, Opcoes, Sair
    Texture2D fundoJogo;
    Camera2D camera;
} infoTela;

typedef struct {
    Vector2 posicao;
    Vector2 posicaoInicial;
    Texture2D imagem[3];
    bool colidindo;
    float velocidade;
    int largura;
    int altura;
    bool olhandoDireita;  // ← adiciona aqui
} infoPersonagem;

typedef struct {
    int colunas;
    int linhas;
    char localMapa[30];
    char** matrizMapa;
    Texture2D mapaImagem[3];
} infoMapa;

typedef struct{
char Estado[4];
}infoMenu;

typedef struct {
    float largura;
    float altura;
} infoBloco;

extern infoTela tela;
extern infoPersonagem personagem;
extern infoMapa map;
extern infoBloco bloco;
extern infoMenu menu;
#endif
