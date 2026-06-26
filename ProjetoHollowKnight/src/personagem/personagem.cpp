#include "personagem.h"
#include "mapa.h"
#include <raylib.h>

static const int FLASK_CUSTO_POR_CURA = 33;
static const float TEMPO_CURA_SEGUNDOS = 3.0f;

static float tempoAtaque = 0.0f;
static float acumuladorRegenMP = 0.0f;

float flaskCarga = 100.0f;
float avisoVidaCheia = 0.0f;
static float tempoCanalizandoCura = 0.0f;

static Texture2D texturaPersonagemDireita = {0};
static Texture2D texturaPersonagemEsquerda = {0};
static Texture2D texturaPersonagemAtacando = {0};
static Texture2D texturaPersonagemAtacandoEsquerda = {0};

void loadPersonagem()
{
    inicializaAmuletos();
    flaskCarga = 100.0f;
    avisoVidaCheia = 0.0f;
    tempoCanalizandoCura = 0.0f;

    texturaPersonagemDireita = LoadTexture("assets/Personagem/personagemDireita.png");
    texturaPersonagemEsquerda = LoadTexture("assets/Personagem/personagemEsquerda.png");
    texturaPersonagemAtacando = LoadTexture("assets/Personagem/personagemAtacando.png");
    texturaPersonagemAtacandoEsquerda = LoadTexture("assets/Personagem/personagemAtacandoEsquerda.png");
    texturaHabilidade[0] = LoadTexture("assets/Objetos/habilidadeDireita.png");
    texturaHabilidade[1] = LoadTexture("assets/Objetos/habilidadeEsquerda.png");

    personagem.imagem[0] = texturaPersonagemDireita;
    personagem.imagem[1] = texturaPersonagemEsquerda;
    personagem.imagem[2] = texturaPersonagemAtacando;
}

void unloadPersonagem()
{
    UnloadTexture(texturaPersonagemDireita);
    UnloadTexture(texturaPersonagemEsquerda);
    UnloadTexture(texturaPersonagemAtacando);
    UnloadTexture(texturaPersonagemAtacandoEsquerda);
    UnloadTexture(texturaHabilidade[0]);
    UnloadTexture(texturaHabilidade[1]);
}

void updatePersonagem()
{
    bool indoDireita = IsKeyDown(KEY_RIGHT);
    bool indoEsquerda = IsKeyDown(KEY_LEFT);

    if (indoDireita)
        personagem.olhandoDireita = true;
    if (indoEsquerda)
        personagem.olhandoDireita = false;

    if (IsKeyPressed(KEY_X) && !personagem.dados.ataque)
    {
        personagem.dados.ataque = true;
        tempoAtaque = 0.20f;
    }

    if (personagem.dados.ataque)
    {
        tempoAtaque -= GetFrameTime();
        if (tempoAtaque <= 0.0f)
        {
            personagem.dados.ataque = false;
            tempoAtaque = 0.0f;
        }
    }

    if (personagem.dados.mp < 100)
    {
        acumuladorRegenMP += GetFrameTime() * 1.0f;
        while (acumuladorRegenMP >= 1.0f && personagem.dados.mp < 100)
        {
            personagem.dados.mp += 1;
            acumuladorRegenMP -= 1.0f;
        }
    }
    else
    {
        acumuladorRegenMP = 0.0f;
    }

    if (personagem.dados.flask < 0)
        personagem.dados.flask = 0;
    if (personagem.dados.flask > 100)
        personagem.dados.flask = 100;

    bool podeCanalizarCura = personagem.dados.hp < personagem.dados.hpMax && personagem.dados.flask >= FLASK_CUSTO_POR_CURA;
    if (IsKeyDown(KEY_A) && podeCanalizarCura)
    {
        tempoCanalizandoCura += GetFrameTime();
        if (tempoCanalizandoCura > TEMPO_CURA_SEGUNDOS)
            tempoCanalizandoCura = TEMPO_CURA_SEGUNDOS;

        float progressoCura = tempoCanalizandoCura / TEMPO_CURA_SEGUNDOS;
        flaskCarga = (float)personagem.dados.flask - (FLASK_CUSTO_POR_CURA * progressoCura);
        if (flaskCarga < 0.0f)
            flaskCarga = 0.0f;
    }
    else
    {
        flaskCarga = (float)personagem.dados.flask;
    }

    if (IsKeyReleased(KEY_A))
    {
        if (tempoCanalizandoCura >= TEMPO_CURA_SEGUNDOS && podeCanalizarCura)
        {
            personagem.dados.hp += 1;
            if (personagem.dados.hp > personagem.dados.hpMax)
                personagem.dados.hp = personagem.dados.hpMax;

            personagem.dados.flask -= FLASK_CUSTO_POR_CURA;
            if (personagem.dados.flask < 0)
                personagem.dados.flask = 0;
        }

        tempoCanalizandoCura = 0.0f;
        flaskCarga = (float)personagem.dados.flask;
    }

    personagem.posicao = movimentaPersonagem(personagem.posicao);
    verificaColisaoAmuletos();
    verificaColisaoHabilidades();
    disparaHabilidade();
    atualizaHabilidade();
}

void desenhaPersonagem()
{
    Texture2D texturaAtual;
    if (personagem.dados.ataque)
    {
        if (personagem.olhandoDireita)
            texturaAtual = personagem.imagem[2];
        else
            texturaAtual = texturaPersonagemAtacandoEsquerda;
    }
    else
    {
        texturaAtual = personagem.olhandoDireita ? personagem.imagem[0] : personagem.imagem[1];
    }
    if (texturaAtual.width > 0 && texturaAtual.height > 0)
    {
        Rectangle src = {0.0f, 0.0f, (float)texturaAtual.width, (float)texturaAtual.height};
        float escalaAtaque;
        if (personagem.dados.ataque)
            escalaAtaque = 2.4f;
        else
            escalaAtaque = 1.0f;
        float drawW = (float)personagem.largura * escalaAtaque;
        float drawH = (float)personagem.altura * escalaAtaque;
        float drawX = personagem.posicao.x + ((float)personagem.largura - drawW) * 0.5f;
        float ajusteAtaqueY;
        if (personagem.dados.ataque)
            ajusteAtaqueY = 16.0f;
        else
            ajusteAtaqueY = 0.0f;
        float drawY = personagem.posicao.y + ((float)personagem.altura - drawH) + ajusteAtaqueY;
        Rectangle dst = {drawX, drawY, drawW, drawH};
        DrawTexturePro(texturaAtual, src, dst, {0, 0}, 0.0f, WHITE);
    }
    else
    {
        Color corAtual = personagem.dados.ataque ? PINK : GREEN;
        DrawRectangle((int)personagem.posicao.x, (int)personagem.posicao.y, personagem.largura, personagem.altura, corAtual);
    }
}
