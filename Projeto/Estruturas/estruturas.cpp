#include "estruturas.h"

// Aqui cria e inicializa as variáveis de verdade

infoTela tela = { 
    1500, 
    800, 
    "Hollow Knight",
    { 0 },
    { {0,0}, {0,0}, 0.0f, 1.0f } 
};

infoPersonagem personagem = {
    { 50, 130 },  // posicao
    { 50, 130 },  // posicaoInicial
    { 0 },        // imagem
    false,        // colidindo
    5.0f,         // velocidade
    20,           // largura
    40            // altura
};
infoMapa map = { 
    151, 
    16, 
    "Mapas/MapaTeste.txt",
    nullptr
};

infoBloco bloco = { 
    30.0f, 
    30.0f 
};
