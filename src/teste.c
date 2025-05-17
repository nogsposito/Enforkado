#include "raylib.h"

int main(void)
{
    // Inicializa a janela
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Teste Raylib");

    SetTargetFPS(60); // Limita a 60 frames por segundo

    while (!WindowShouldClose()) // Detecta o fechamento da janela (ESC ou botão fechar)
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Raylib funcionando!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow(); // Fecha a janela

    return 0;
}