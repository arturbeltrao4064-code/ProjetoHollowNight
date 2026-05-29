#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <raylib.h>

// 1. Definição do TIPO (Apenas o molde)
typedef struct {
    int largura;
    int altura;
    char titulo[20];
    Texture2D menuImagem[3];
} infoTela;

typedef struct {
    Vector2 posicao;
    Vector2 posicaoInicial;
    Texture2D imagem[3];
    bool colidindo;
    float velocidade;
    int largura;
    int altura;
    enum DIRECAO {DIREITA,ESQUERDA};
} infoPersonagem;

typedef struct {
    int colunas;
    int linhas;
    char localMapa[30];
    char** matrizMapa;
    Texture2D mapaImagem[3];
} infoMapa;

typedef struct {
    float largura;
    float altura;
} infoBloco;


// 2. Declaração das variáveis como EXTERN
// Isso diz ao C: "Essas variáveis existem, mas serão criadas em outro lugar"
extern infoTela tela;
extern infoPersonagem personagem;
extern infoMapa map;
extern infoBloco bloco;
#endif // !ESTRUTURAS_H
