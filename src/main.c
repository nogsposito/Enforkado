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
    struct NodeStack *prev;
} NodeStack;

// ADICIONA EM LISTA ENCADEADA AS LETRAS DA STRING GERADA POR GEMINI
void geminiWordGenerator(Word **headGemini, Word **tailGemini);

void createPlayerList(Word **headPlayer, Word **tailPlayer, int lenght);
int checkLetter(Word *headGemini, char letter);
void addPlayerList(Word **headPlayer, int index, char letter);
void push(NodeStack **headStack, char letter);
void insertionSort(NodeStack **headStack);

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
  NodeStack *aux = *headStack;
    while (aux) {
        if (aux->letter == letter) {
            return;
        }
        aux = aux->next;
    }
    NodeStack *new = (NodeStack*)malloc(sizeof(NodeStack));

    if(new != NULL) {
        new->letter = letter;
        new->next = *headStack;
        new->prev = NULL;

        if(*headStack != NULL) {
            (*headStack)->prev = new;
        }
        
        *headStack = new;
    }
}

void insertionSort(NodeStack **headStack) {
    if(*headStack != NULL) {
        NodeStack *aux1 = (*headStack)->next;

        while(aux1 != NULL) { 
            NodeStack *aux2 = aux1;

            while(aux2 != *headStack && aux2->letter < aux2->prev->letter) {
                char c = aux2->letter;

                aux2->letter = aux2->prev->letter;
                aux2->prev->letter = c;

                aux2 = aux2->prev;
            }

            aux1 = aux1->next;
        }
    } 
}
