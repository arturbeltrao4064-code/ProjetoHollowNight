#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H
#include <raylib.h>
#define QT_BOTOES_MAX 5 // Tamanho do inventário (5x5)
#define MAX_INIMIGOS 10 // Limite máximo de inimigos que o mapa pode ter

enum Estado { ESTADO_MENU, ESTADO_JOGANDO, ESTADO_PAUSADO, ESTADO_CONFIGURACOES, ESTADO_INVENTARIO };

typedef enum {
    AMULETO_ATAQUE,
    AMULETO_DEFESA,
    AMULETO_VIDA,
    TOTAL_AMULETOS
} TipoAmuleto;
 
typedef struct {
    TipoAmuleto tipo;
    bool coletado;
} Amuleto;

typedef enum {
    FASE_VILA = 0,
    FASE_INICIAL,
    FASE_FINAL
} FaseAtual;

// Certifique-se de que na sua struct infoMapa, o localMapa seja assim:
// const char* localMapa;

typedef struct {
    int largura;
    int altura;
    char titulo[20];
    Camera2D camera;
    Texture2D fundoJogo;
} infoTela;

typedef struct {
    float velocidade;
    float gravidade;
    float forcaPulo;
    float velocidadeY;
} constantes;

typedef struct {
    int totalOpcoes;
    int opcaoSelecionada;
    int botoesY[QT_BOTOES_MAX];
    int botaoW;
    int botaoH;
    Texture2D menuImagem[10];
    Texture2D menuBotoes[10];   
} infoMenu;

typedef struct {
    bool ativo;
    Vector2 posicao;
    float velocidade;
    bool direcao; // true = direita, false = esquerda
} Habilidade;

typedef struct {
    int hp;
    int mp;
    int valorAtaque;
    int valorDefesa;
    bool ataque;
    bool vivo;
    int amuletosColetados;
    Amuleto amuletos[TOTAL_AMULETOS];
    int habilidadesColetadas;  // ← ADICIONA
    Habilidade habilidadeAtiva; // ← ADICIONA
} dadosEntidade;

typedef struct {
    Vector2 posicao;
    Vector2 posicaoInicial;
    Texture2D imagem[3];
    bool colidindo;
    int largura;
    int altura;
    bool olhandoDireita;
    dadosEntidade dados;
} infoEntidade;

typedef struct {
    int linhas;
    int colunas;
    const char* localMapa; // <-- Deixe como const char* em vez de char[]
    char** matrizMapa;
} infoMapa;

typedef struct {
    float largura;
    float altura;
} infoBloco;

extern infoTela tela;
extern infoEntidade personagem;

extern infoEntidade listaInimigos[MAX_INIMIGOS];
extern int quantidadeInimigos;
extern FaseAtual faseDoJogo;
extern dadosEntidade dadosPersonagem;
extern infoMapa map;
extern infoBloco bloco;
extern infoMenu menuPrincipal;
extern infoMenu menuPause;
extern infoMenu menuConfiguracoes;
extern constantes constantesJogo;

#endif
