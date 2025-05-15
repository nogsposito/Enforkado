#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

typedef struct Word{
    int index;
    char letter;
    struct Word *next;
    struct Word *prev;
} Word;

typedef struct NodeStack{
    char letter;
    struct NodeStack *next;
} NodeStack;

// ADICIONA EM LISTA ENCADEADA AS LETRAS DA STRING GERADA POR GEMINI
void geminiWordGenerator(Word **headGemini, Word **tailGemini);

void createPlayerList(Word **headPlayer, Word **tailPlayer, int lenght);
int checkLetter(Word *headGemini, char letter);
void addPlayerList(Word **headPlayer, int index, char letter);
void push(NodeStack **headStack, char letter);


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

int checkLetter(Word *headGemini, char letter) {
    if(headGemini != NULL) {
        while(headGemini->letter != letter && headGemini->next != NULL) {
            headGemini = headGemini->next;
        }

        if(headGemini->letter == letter) {
            return headGemini->index;
        }
    }

    return 0;
}

void addPlayerList(Word **headPlayer, int index, char letter) {
    if(*headPlayer != NULL) {
        Word *aux = *headPlayer;

        while(aux->index != index && aux->next != NULL) {
            aux= aux->next;
        }

        if(aux->index == index) {
            aux->letter = letter;
        }
    }
}

void push(NodeStack **headStack, char letter){
    NodeStack *new = (NodeStack*)malloc(sizeof(NodeStack));

    if (new != NULL) {
        new->letter = letter;
        new->next = *headStack;
        *headStack = new;
    }
}