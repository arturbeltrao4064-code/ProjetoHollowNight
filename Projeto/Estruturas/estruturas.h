#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H
#include <raylib.h>


enum Estado { ESTADO_MENU, ESTADO_JOGANDO, ESTADO_PAUSADO, ESTADO_CONFIGURACOES };

typedef struct {
    int largura;
    int altura;
    char titulo[20];
    Camera2D camera;
    Texture2D fundoJogo;
} infoTela;

typedef struct {
    int totalOpcoes;
    int opcaoSelecionada;
    int botoesY[4];
    int botaoW;
    int botaoH;
    Texture2D menuImagem[10];
    Texture2D menuBotoes[10];  // ← texturas dos 4 botoes: Jogar, Carregar, Opcoes, Sair    
}infoMenu;

typedef struct {
    int hp;
    int mp;
    bool ataque;
    bool vivo;
} dadosEntidade;

typedef struct {
    Vector2 posicao;
    Vector2 posicaoInicial;
    Texture2D imagem[3];
    bool colidindo;
    float velocidade;
    int largura;
    int altura;
    bool olhandoDireita;
    dadosEntidade dados;
} infoEntidade;

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

extern infoTela tela;
extern infoEntidade personagem;
extern infoEntidade inimigo;
extern dadosEntidade dadosPersonagem;
extern infoMapa map;
extern infoBloco bloco;
extern infoMenu menuPrincipal;
extern infoMenu menuPause;
extern infoMenu menuConfiguracoes;

#endif
