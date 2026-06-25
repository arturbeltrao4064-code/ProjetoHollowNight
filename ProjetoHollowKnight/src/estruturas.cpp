#include "estruturas.h"

// Aqui cria e inicializa as variáveis de verdade

infoTela tela = { 
    1500, 
    800, 
    "Hollow Knight",
    { {0,0}, {0,0}, 0.0f, 1.0f }  // camera
};

infoEntidade personagem = {
    { 0, 0 },     // Será definido pelo 'J' no mapa
    { 0, 0 },     // posicaoInicial
    { 0 },        // imagem
    false,        // colidindo
    20,           // largura
    20,           // altura
    true,         // olhandoDireita 
    { 5, 5, 100, 0, 1, 5, false, true , 0 } // dados: hp, hpMax, mp, flask, atk, def...
};

// MODIFICADO: Inicializa o array de inimigos zerado. O mapa vai preencher cada um.
infoEntidade listaInimigos[MAX_INIMIGOS] = { 0 };
int quantidadeInimigos = 0;
int proximoTunel = 1;

// Substitua a inicialização antiga por essa:
infoMapa map = { 
    16,           // Linhas (ajustado para bater com a matriz dos txts)
    151,          // Colunas
    "maps/vila.txt",
    nullptr
};

infoBloco bloco = { 
    30.0f, 
    30.0f 
};

infoMenu menuPrincipal = {
    5,          // totalOpcoes
    0,          // opcaoSelecionada
    { 300, 400, 500, 600 , 700},  // botoesY
    200,        // botaoW
    50          // botaoH
};

infoMenu menuPause = {
    5,          // totalOpcoes
    0,          // opcaoSelecionada
    { 220, 290, 360, 430, 495 },  // botoesY
    260,        // botaoW
    55          // botaoH
};

infoMenu menuConfiguracoes = {
    3,          // totalOpcoes
    0,          // opcaoSelecionada
    { 294, 361, 428 },  // botoesY
    200,        // botaoW
    50          // botaoH
};

constantes constantesJogo = {
    8.0f,   // velocidade
    0.5f,   // gravidade
    -20.0f,  // forcaPulo
    0.0f    // velocidadeY padrão
};
