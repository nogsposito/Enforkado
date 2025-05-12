#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

typedef struct Word{
    char letter;
    int position;
    struct Word *next;
} Word;


// ADICIONA EM LISTA ENCADEADA AS LETRAS DA STRING GERADA POR GEMINI
void geminiWordGenerator(Word **head, Word **tail);

// CRIA QUANTIDADE DE NÓS NECESSÁRIAS NA LISTA DO JOGADOR COM CHARS VAZIOS PARA ARMAZENAR ONDE
// ESTÁ CADA PALAVRA ADIVINHADA PELO JOGADOR.
void createPlayerList(Word *head_pilha_word, Word **tail_player);

// CHECA SE PALAVRA ADIVINHADA PELO JOGADOR == A DO GEMINI (ESTAO NO MESMO INDEX)
int checkLetter(Word **head_gemini, Word **head_player){
    // ADAPTAR PARA BOOLEAN (1 ou 0)
    return 1;
}

// ADICIONAR EM INDEX A PALAVRA ADIVINHADA PELO JOGADOR
void addLetterPlayerList(Word **head_player, int index, char letter);

// ADICIONA LETRA EM PILHA (DE PALAVRAS ADIVINHADAS POR JOGADOR)
void addPilha(Word **head_pilha);

// REORGANIZA EM ORDEM ALFABETICA NA PILHA DE PALAVRAS ADICIONADAS PELO JOGADOR
void bubblesort(Word **head_pilha);

void startGame();

void endGame();

int main() {
    InitWindow(800, 600, "Teste Raylib");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello, Raylib!", 300, 280, 20, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}


void addLetterPlayerList(Word **head_player, int index, char letter){
    Word *aux = *head_player;

    while(aux->position != index){
        aux= aux->next;
    }

    aux->letter = letter;
}