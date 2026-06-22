#include "estruturas.h"

// Aqui cria e inicializa as variáveis de verdade

infoTela tela = { 
    1500, 
    800, 
    "Hollow Knight",
    { 0 },  // menuImagem
    { 0 },  // menuBotoes  ← adiciona essa linha
    { 0 },  // fundoJogo
    { {0,0}, {0,0}, 0.0f, 2.0f }  // camera
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
    "Mapa/MapaTeste.txt",
    nullptr
};

infoBloco bloco = { 
    30.0f, 
    30.0f 
};
