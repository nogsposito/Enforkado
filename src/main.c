#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"

typedef struct Word{
    char letter;
    int index;
    struct Word *next;
    struct Word *prev;
} Word;

// ADICIONA EM LISTA ENCADEADA AS LETRAS DA STRING GERADA POR GEMINI
void geminiWordGenerator(Word **headGemini, Word **tailGemini);

void createPlayerList(Word **headPlayer, Word **tailPlayer, int lenght);
bool checkLetter(Word *headGemini, char letter);

// ADICIONAR EM INDEX A PALAVRA ADIVINHADA PELO JOGADOR
void addLetterPlayerList(Word **head_player, int index, char letter);

// ADICIONA LETRA EM PILHA (DE PALAVRAS ADIVINHADAS POR JOGADOR)
void addPilha(Word **head_pilha, char letter){

    Word *new = (Word*)malloc(sizeof(Word));

    if (new == NULL) return;

    new->letter = letter;
    new->next = *head_pilha;
    (*head_pilha)->prev = new;
    new->prev = NULL;
    *head_pilha = new; // só isso (pilha)?

}

// REORGANIZA EM ORDEM ALFABETICA NA PILHA DE PALAVRAS ADICIONADAS PELO JOGADOR null
void bubblesort(Word **head_pilha, int tamanho){
    int n = 1;
    int troca = 1;
    while (n <= tamanho && troca == 1){
        troca = 0;
        Word *aux= *head_pilha;
        for (int i = 0; i < (tamanho - 1); i++){
            /* code  bubble sort em pilha?*/
        }
        
    }
};

int tamanho(Word **head_pilha){

    Word *aux = *head_pilha;
    int count = 0;

    while (aux != NULL){
        count++;
        aux = aux->next;
    }
    
    return count;
}

void startGame();

void endGame();


void addLetterPlayerList(Word **head_player, int index, char letter){
    Word *aux = *head_player;

    while(aux->index != index){
        aux= aux->next;
    }

    aux->letter = letter;
}



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

void createPlayerList(Word **headPlayer, Word **tailPlayer, int lenght) {
    if(*headPlayer == NULL && *tailPlayer == NULL) {
        for(int i = 0; i < lenght; i++) {
            Word *new = (Word*)malloc(sizeof(Word));

            if(new != NULL) {
                new->letter = '\0';
                new->index = i+1;
                new->next = NULL;
                new->prev = *tailPlayer;

                if(*headPlayer == NULL) {
                    *headPlayer = new;
                } else {
                    (*tailPlayer)->next = new;
                }

                *tailPlayer = new;
            }
        }
    }
}

bool checkLetter(Word *headGemini, char letter) {
    if(headGemini != NULL) {
        while(headGemini->letter != letter && headGemini->next != NULL) {
            headGemini = headGemini->next;
        }

        if(headGemini->letter == letter) {
            return true;
        }
    }

    return false;
}