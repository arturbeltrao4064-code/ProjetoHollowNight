#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
bool colide = false;

int main(void)
{
	struct infoTela
	{
		const int largura = 1200;//pixel
		const int altura = 800;//pixel
		char titulo[14] = { "Hollow Knight" };
	}tela;

	InitWindow(tela.largura, tela.altura, tela.titulo);

	struct personagemInfo
	{
		const int largura = 175;//pixel
		const int altura = 175;//pixel
		Vector2 posicao =
		{
			10,//x 
			700,//y -> (tela.altura - personagem.altura)
		};
		int quantidadeVida = 4;
		int vida = 100;
		int energia = 0;
		Texture2D imagem = LoadTexture("TexturaPersonagem/Personagem1.png");
	}personagem;

	Vector2 movimentaPersoangem(Vector2 personagemPosicao);

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		
		personagem.posicao = movimentaPersoangem(personagem.posicao);

		BeginDrawing();

			ClearBackground(BLACK);

			DrawRectangle(10,10,100,20,RED);
			DrawRectangle(10,30,100,20,BLUE);

			DrawTextureEx(
				personagem.imagem,
				personagem.posicao,
				0.0f,
				0.5f,
				WHITE
			);
		EndDrawing();
	}
	UnloadTexture(personagem.imagem);

	CloseWindow();
	
	return 0;
}


Vector2 movimentaPersoangem(Vector2 personagemPosicao) {
	if (colide == false) {
		if (IsKeyDown(KEY_RIGHT))
		{
			personagemPosicao.x += 2.0f;
		}
		if (IsKeyDown(KEY_LEFT))
		{
			personagemPosicao.x -= 2.0f;
		}
		if (IsKeyDown(KEY_UP))
		{
			personagemPosicao.y -= 2.0f;
		}
		if (IsKeyDown(KEY_DOWN))
		{
			personagemPosicao.y += 2.0f;
		}
	}
		return personagemPosicao;
	
};