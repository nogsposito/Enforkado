#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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
bool isPlayerListCorrect(Word *headGemini, Word *headPlayer);
bool checkLetterInStack(NodeStack *headStack, char letter);
int checkLetter(Word *headGemini, char letter);
void addPlayerList(Word **headPlayer, int index, char letter);
void push(NodeStack **headStack, char letter);
void insertionSort(NodeStack **head);

void addGeminiList(Word **head, Word **tail, const char *prompt){
    
    char palavra[51];
    strcpy(palavra, prompt);

    int i = 0;
    
    while (palavra[i] != '\0'){

        if (palavra[i] == '\\' && palavra[i+1] == 'n'){
            break;
        }

        Word *novo = (Word*)malloc(sizeof(Word));
        if (novo == NULL) return;

        novo->letter = palavra[i];
        novo->index = i+1;

        if (*head == NULL){
            *head = novo;
            *tail = novo;
            novo->prev = NULL;
        } else {
            (*tail)->next = novo;
            novo->prev = *tail;
            *tail = novo;
        }
        i++;
    }
}

void printPilha(NodeStack *head){ 
	while(head != NULL) {
		printf("%c ", head->letter);
		head = head->next;
	}
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

    /*
    printf("Bem vindo ao Enforkado!\n");
    printf("Seu objetivo e acertar o ingrediente secreto\n");

    Word *headGemini = NULL;
    Word *tailGemini = NULL;

    Word *headPlayer = NULL;
    Word *tailPlayer = NULL;

    int lives = 5;

    NodeStack *headStack = NULL;

    addGeminiList(&headGemini, &tailGemini, 1, 'Q');
    addGeminiList(&headGemini, &tailGemini, 2, 'U');
    addGeminiList(&headGemini, &tailGemini, 3, 'E');
    addGeminiList(&headGemini, &tailGemini, 4, 'I');
    addGeminiList(&headGemini, &tailGemini, 5, 'J');
    addGeminiList(&headGemini, &tailGemini, 6, 'O');

    createPlayerList(&headPlayer, &tailPlayer, tailGemini->index);

    while(!isPlayerListCorrect(headGemini, headPlayer) && lives != 0) {
        char letter;

        printf("\n%c %c %c %c %c %c\n", headPlayer->letter, headPlayer->next->letter, headPlayer->next->next->letter, headPlayer->next->next->next->letter, headPlayer->next->next->next->next->letter, headPlayer->next->next->next->next->next->letter);
        printf("Letras tentadas:");
        printPilha(headStack);
        printf("\nVidas: %d\n", lives);

        //Esse espaço antes é importante
        scanf(" %c", &letter);

        if(checkLetter(headGemini, letter) != 0) {
            addPlayerList(&headPlayer, checkLetter(headGemini, letter), letter);
        } else {
            if (!checkLetterInStack(headStack, letter)) {
                push(&headStack, letter);
                insertionSort(&headStack);
                lives--;
            }
            else {
                printf("Você já errou essa letra antes");
            }
        }
    }

    if(isPlayerListCorrect(headGemini, headPlayer)) {
        printf("Voce acertou!");
    } else if(lives == 0) {
        printf("Suas vidas acabaram!");
    }
    */

    return 0;
}

void createPlayerList(Word **headPlayer, Word **tailPlayer, int lenght) {
    if(*headPlayer == NULL && *tailPlayer == NULL) {
        for(int i = 0; i < lenght; i++) {
            Word *new = (Word*)malloc(sizeof(Word));

            if(new != NULL) {
                new->index = i+1;
                new->letter = '\0';               
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

bool isPlayerListCorrect(Word *headGemini, Word *headPlayer) {
    if(headGemini != NULL && headPlayer != NULL) {
        while(headGemini != NULL && headPlayer != NULL && headGemini->letter == headPlayer->letter) {
            headGemini = headGemini->next;
            headPlayer = headPlayer->next;
        }

        return headGemini == NULL && headPlayer == NULL;
    }

    return false;
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

    while (aux != NULL) {
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

bool checkLetterInStack(NodeStack *headStack, char letter) {
    while (headStack != NULL) {
        if (headStack->letter == letter) {
            return true;
        }
        headStack = headStack->next;
    }
    return false;
}


void insertionSort(NodeStack **head) {
    if(*head != NULL) {
        NodeStack *aux1 = (*head)->next;

        while(aux1 != NULL) { 
            NodeStack *aux2 = aux1;

            while(aux2 != *head && aux2->letter < aux2->prev->letter) {
                char c = aux2->letter;

                aux2->letter = aux2->prev->letter;
                aux2->prev->letter = c;

                aux2 = aux2->prev;
            }

            aux1 = aux1->next;
        }
    } 
}
