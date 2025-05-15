#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

typedef struct Word{
    char letter;
    int index;
    struct Word *next;
    struct Word *prev;
} Word;

// ADICIONA EM LISTA ENCADEADA AS LETRAS DA STRING GERADA POR GEMINI
void geminiWordGenerator(Word **head, Word **tail);
void createPlayerList(Word **headPlayer, Word **tailPlayer, int lenght);

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

void createPlayerList(Word **head, Word **tail, int lenght) {
    if(*head == NULL && *tail == NULL) {
        for(int i = 0; i < lenght; i++) {
            Word *new = (Word*)malloc(sizeof(Word));

            if(new != NULL) {
                new->letter = '\0';
                new->index = i+1;
                new->next = NULL;
                new->prev = *tail;

                if(*head == NULL) {
                    *head = new;
                } else {
                    (*tail)->next = new;
                }

                *tail = new;
            }
        }
    }
}

int checkLetter(Word **head_gemini, Word **head_player) {
    Word *g = *head_gemini;
    Word *p = *head_player;

    while (g != NULL && p != NULL) {
        if (g->letter != p->letter) {
            return 0;
        }
        g = g->next;
        p = p->next;
    }

    return 1;
}