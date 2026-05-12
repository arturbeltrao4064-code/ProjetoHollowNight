#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

int main()
{
	const int largura = 1200;
	const int altura = 800;

	char titulo[14] = {"Hollow Knight"};

	InitWindow(largura,altura,titulo);

	Texture2D imagem = LoadTexture("Imagem.png");

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		BeginDrawing();

			ClearBackground(RAYWHITE);

			DrawTexture(imagem, 100, 100, WHITE);

		EndDrawing();
	}
	UnloadTexture(imagem);

	CloseWindow();
	
	return 0;
}