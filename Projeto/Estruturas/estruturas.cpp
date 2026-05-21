#include "estruturas.h"
#include <cstddef>

// Aqui você cria e inicializa as variáveis de verdade
infoTela tela = { 1500, 800, "Hollow Knight",{ 0 }};

infoPersonagem personagem = { { 50, 150 }, 30, 32, { 0 }, false}; // {0} limpa a struct da textura

infoMapa map = { 151, 16, "Mapas/MapaTeste.txt", NULL };

infoBloco bloco = { 30.0f, 30.0f };
