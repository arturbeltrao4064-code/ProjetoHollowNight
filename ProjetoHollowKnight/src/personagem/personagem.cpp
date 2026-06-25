#include "personagem.h"
#include "mapa.h"
#include <raylib.h>
#include <cmath>

static Texture2D texSkin = { 0 };
static Texture2D texHair = { 0 };
static Texture2D texPants = { 0 };
static Texture2D texShirt = { 0 };
static Texture2D texBoots = { 0 };
static Texture2D texSword = { 0 };
static bool spritesPersonagemProntos = false;
static float tempoAtaque = 0.0f;
static float tempoAnimacaoAndando = 0.0f;
static bool personagemAndando = false;
static const float escalaVisualPersonagem = 4.0f;

static bool texturaCarregada(Texture2D textura) {
    return textura.id != 0;
}

static void desenhaCamada(Texture2D textura, Rectangle destino, bool olhandoDireita, Color cor) {
    if (!texturaCarregada(textura)) return;

    Rectangle origem = { 0.0f, 0.0f, (float)textura.width, (float)textura.height };
    if (!olhandoDireita) {
        origem.x = (float)textura.width;
        origem.width = -(float)textura.width;
    }

    DrawTexturePro(textura, origem, destino, (Vector2){ 0.0f, 0.0f }, 0.0f, cor);
}

void loadPersonagem() {
    texSkin = LoadTexture("assets/Personagem/Character skin colors/Male Skin1.png");
    texHair = LoadTexture("assets/Personagem/Male Hair/Male Hair1.png");
    texPants = LoadTexture("assets/Personagem/Male Clothing/Blue Pants.png");
    texShirt = LoadTexture("assets/Personagem/Male Clothing/Blue Shirt v2.png");
    texBoots = LoadTexture("assets/Personagem/Male Clothing/Boots.png");
    texSword = LoadTexture("assets/Personagem/Male Hand/Male Sword.png");

    spritesPersonagemProntos =
        texturaCarregada(texSkin) && texturaCarregada(texHair) && texturaCarregada(texPants) &&
        texturaCarregada(texShirt) && texturaCarregada(texBoots) && texturaCarregada(texSword);

    if (spritesPersonagemProntos) {
        TraceLog(LOG_INFO, "Sprites do personagem carregados: skin1 + hair1 + blue pants/shirt + boots + sword");
    }
    else {
        TraceLog(LOG_WARNING, "Falha ao carregar sprites do personagem. Usando fallback em retangulo.");
    }

    inicializaAmuletos();
}

void unloadPersonagem() {
    if (texturaCarregada(texSkin)) UnloadTexture(texSkin);
    if (texturaCarregada(texHair)) UnloadTexture(texHair);
    if (texturaCarregada(texPants)) UnloadTexture(texPants);
    if (texturaCarregada(texShirt)) UnloadTexture(texShirt);
    if (texturaCarregada(texBoots)) UnloadTexture(texBoots);
    if (texturaCarregada(texSword)) UnloadTexture(texSword);

    spritesPersonagemProntos = false;
}

void updatePersonagem() {
    bool indoDireita = IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT);
    bool indoEsquerda = IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT);

    if (indoDireita) personagem.olhandoDireita = true;
    if (indoEsquerda) personagem.olhandoDireita = false;

    personagemAndando = indoDireita || indoEsquerda;
    if (personagemAndando) tempoAnimacaoAndando += GetFrameTime() * 12.0f;
    else tempoAnimacaoAndando = 0.0f;

    if (IsKeyPressed(KEY_X) && !personagem.dados.ataque) {
        personagem.dados.ataque = true;
        tempoAtaque = 0.20f;
    }

    if (personagem.dados.ataque) {
        tempoAtaque -= GetFrameTime();
        if (tempoAtaque <= 0.0f) {
            personagem.dados.ataque = false;
            tempoAtaque = 0.0f;
        }
    }

    personagem.posicao = movimentaPersonagem(personagem.posicao);
    verificaColisaoAmuletos();
    verificaColisaoHabilidades();
    disparaHabilidade();
    atualizaHabilidade();
}

void desenhaPersonagem() {
    float deslocamentoY = personagemAndando ? sinf(tempoAnimacaoAndando) * 1.2f : 0.0f;
    float larguraVisual = (float)personagem.largura * escalaVisualPersonagem;
    float alturaVisual = (float)personagem.altura * escalaVisualPersonagem;
    float offsetX = (larguraVisual - (float)personagem.largura) * 0.5f;
    float offsetY = alturaVisual - (float)personagem.altura;

    Rectangle destino = {
        personagem.posicao.x - offsetX,
        personagem.posicao.y - offsetY + deslocamentoY,
        larguraVisual,
        alturaVisual
    };

    if (!spritesPersonagemProntos) {
        Color corFallback = personagem.dados.ataque ? PINK : GREEN;
        DrawRectangle((int)destino.x, (int)destino.y, personagem.largura, personagem.altura, corFallback);
        return;
    }

    desenhaCamada(texSkin, destino, personagem.olhandoDireita, WHITE);
    desenhaCamada(texPants, destino, personagem.olhandoDireita, WHITE);
    desenhaCamada(texBoots, destino, personagem.olhandoDireita, WHITE);
    desenhaCamada(texShirt, destino, personagem.olhandoDireita, WHITE);
    desenhaCamada(texHair, destino, personagem.olhandoDireita, WHITE);

    Rectangle destinoEspada = destino;
    destinoEspada.x += personagem.olhandoDireita ? destino.width * 0.45f : -destino.width * 0.45f;
    Color corEspada = personagem.dados.ataque ? (Color){ 255, 230, 230, 255 } : WHITE;
    desenhaCamada(texSword, destinoEspada, personagem.olhandoDireita, corEspada);
}
