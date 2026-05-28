#include "estruturas.h"

// Aqui você cria e inicializa as variáveis de verdade
infoTela tela = { 1500, 800, "Hollow Knight",{ 0 }};

infoPersonagem personagem = { { 50, 130 }, { 0, 0 }, { 0 }, false, {5.0f} }; // {0} limpa a struct da textura

infoMapa map = { 151, 16, "Mapas/MapaTeste.txt", nullptr };

infoBloco bloco = { 30.0f, 30.0f };
