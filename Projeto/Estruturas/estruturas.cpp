#include "estruturas.h"

// Aqui cria e inicializa as variáveis de verdade

infoTela tela = { 
    1500, 
    800, 
    "Hollow Knight",
    { {0,0}, {0,0}, 0.0f, 2.0f }  // camera
};

infoEntidade personagem = {
    { 50, 130 },  // posicao
    { 50, 130 },  // posicaoInicial
    { 0 },        // imagem
    false,        // colidindo
    5.0f,         // velocidade
    30,           // largura
    30,       // altura
    true,         // olhandoDireita
    {  // dados
        100, //hp
        50, //mp
        10, //ataque
        5 //defesa
    } 
};

infoMapa map = { 
    151, 
    16, 
    {"Mapa/Mapas/mapaInicial.txt"},
    nullptr
};

infoBloco bloco = { 
    30.0f, 
    30.0f 
};

infoMenu menuPrincipal = {
    4,          // totalOpcoes
    0,          // opcaoSelecionada
    { 300, 400, 500, 600 },  // botoesY
    200,        // botaoW
    50          // botaoH
};

infoMenu menuPause = {
    4,          // totalOpcoes
    0,          // opcaoSelecionada
    { 220, 290, 360, 430 },  // botoesY
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
