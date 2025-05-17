#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "raylib.h"
#include <curl/curl.h>

#define NUM_PALAVRAS = 10
#define TAM_PALAVRA = 51

typedef struct {
    char *buffer;
    size_t size;
} MemoryStruct;

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

typedef enum { MENU, FASE, GAMEOVER } GameScreen;

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
char* geminiWordGenerator(const char *prompt);
void colocarNoArray(char *palavras[NUM_PALAVRAS], char*ingredients);
void addGeminiList(Word **head, Word **tail, char *ingredient);
void createPlayerList(Word **headPlayer, Word **tailPlayer, int lenght);
bool isPlayerListCorrect(Word *headGemini, Word *headPlayer);
bool checkLetterInStack(NodeStack *headStack, char letter);
int checkLetter(Word *headGemini, char letter);
void addPlayerList(Word **headPlayer, int index, char letter);
void push(NodeStack **headStack, char letter);
void insertionSort(NodeStack **head);
char to_uppercase(char letter);
void printPilha(NodeStack *head);

int main() {

    char palavras[MAX_PALAVRAS][TAM_PALAVRA] = {{0}};
    char *response = geminiWordGenerator("Retorne 10 ingredientes para uma receita específica, sem instruções, sem pontuação, sem caracteres especiais e separados por espaços. Apenas palavras simples como 'Leite', 'Ovo' ou 'Queijo' que façam uma receita (EM MAIUSCULO)");

    if (response != NULL) {
        int i = 0;
        char *token = strtok(response, " ");
        while (token != NULL && i < MAX_PALAVRAS) {
            strncpy(palavras[i], token, TAM_PALAVRA - 1);
            palavras[i][TAM_PALAVRA - 1] = '\0';
            i++;
            token = strtok(NULL, " ");
        }
        free(response);
    }


    const int largura = 800;
    const int altura = 600;

    InitWindow(largura, altura, "Jogo");
    GameScreen telaAtual = MENU;
    Rectangle botao = { largura / 2 - 100, altura / 2 - 25, 200, 50 };

    // tbm fazer ele retornar o nome da receita em si e mostrar no final a receita que o user fez?

    while (!WindowShouldClose()) {

        if (telaAtual == MENU){
            if (CheckCollisionPointRec(GetMousePosition(), botao) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                telaAtual = FASE;
            } // apertar botao para fase
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (telaAtual == MENU){

            DrawText("Seja bem vindo ao Enforkado!", 20, 160, 20, DARKGRAY);
            
            DrawRectangleRec(botao, LIGHTGRAY);
            if (CheckCollisionPointRec(GetMousePosition(), botao)) {
                DrawRectangleLinesEx(botao, 2, RED);
            } else {
                DrawRectangleLinesEx(botao, 2, BLACK);
            }

            DrawText("Começar", largura / 2 - MeasureText("Começar", 20) / 2, altura / 2 - 10, 20, BLACK);
        } else if (telaAtual == FASE){
            DrawText("Resposta do Gemini:", 20, 20, 20, DARKGRAY);
            if (response) {
                DrawText(response, 20, 60, 30, MAROON);
            } else {
                DrawText("Erro ao obter resposta!", 50, 60, 20, RED);
            }
        }
        
        EndDrawing();
    }

    CloseWindow();

    /*
    printf("Bem vindo ao Enforkado!\n");
    printf("Seu objetivo e acertar o ingrediente secreto\n");

    Word *headGemini = NULL;
    Word *tailGemini = NULL;

    char *ingredients[NUM_PALAVRAS];

    Word *headPlayer = NULL;
    Word *tailPlayer = NULL;

    int lives = 5;

    NodeStack *headStack = NULL;
    char *response = geminiWordGenerator("Retorne 10 ingredientes para uma receita específica, sem instruções, sem pontuação, sem caracteres especiais e separados por espaços. Apenas palavras simples como 'Leite', 'Ovo' ou 'Queijo' que façam uma receita (EM MAIUSCULO)");
    colocarNoArray( ingredients, response);
    for(int i=0;i<NUM_PALAVRA;i++){
        addGeminiList(&headGemini, &tailGemini, ingredients[i]); //fazer um for do tamanho NUM_PALAVRAS que rode 10 vezes

    createPlayerList(&headPlayer, &tailPlayer, tailGemini->index);

    while(!isPlayerListCorrect(headGemini, headPlayer) && lives != 0) {
        char letter;

        printf("\n%c %c %c %c %c %c\n", headPlayer->letter, headPlayer->next->letter, headPlayer->next->next->letter, headPlayer->next->next->next->letter, headPlayer->next->next->next->next->letter, headPlayer->next->next->next->next->next->letter);
        printf("Letras tentadas:");
        printPilha(headStack);
        printf("\nVidas: %d\n", lives);

        //Esse espaço antes é importante
        scanf("Digite um letra: ")
        scanf(" %c", &letter);
        letter = to_uppercase(letter);


        if(checkLetter(headGemini, letter) != 0) {
            addPlayerList(&headPlayer, checkLetter(headGemini, letter), letter);
        } else {
            if (!checkLetterInStack(headStack, letter)) {
                push(&headStack, letter);
                insertionSort(&headStack);
                lives--;
            }
            else {
                printf("Voce ja errou essa letra antes");
            }
        }
    }

    if(isPlayerListCorrect(headGemini, headPlayer)) {
        printf("Voce acertou!");
    } else if(lives == 0) {
        printf("Suas vidas acabaram!");
    }
    */
    }
    

    return 0;
}

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)userp;

    char *ptr = realloc(mem->buffer, mem->size + realsize + 1);
    if (ptr == NULL) return 0;

    mem->buffer = ptr;
    memcpy(&(mem->buffer[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->buffer[mem->size] = 0;

    return realsize;
}

char* geminiWordGenerator(const char *prompt) {
    CURL *curl;
    CURLcode res;
    MemoryStruct chunk;

    chunk.buffer = malloc(1);
    chunk.size = 0;

    const char *api_key = "AIzaSyAwO9mHjyuJfKTHLZMfl2TLCeOS5oSPOHk";  // substitua aqui
    const char *model = "gemini-1.5-flash-latest";

    char url[512];
    snprintf(url, sizeof(url), 
        "https://generativelanguage.googleapis.com/v1beta/models/%s:generateContent?key=%s",
        model, api_key
    );

    char json_payload[1024];
    snprintf(json_payload, sizeof(json_payload),
        "{\"contents\":[{\"parts\":[{\"text\":\"%s\"}]}]}", prompt
    );

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json; charset=UTF-8");

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "Erro na requisição: %s\n", curl_easy_strerror(res));
            free(chunk.buffer);
            chunk.buffer = NULL;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    curl_global_cleanup();

    if (chunk.buffer) {
        char *start = strstr(chunk.buffer, "\"text\": \"");
        if (start) {
            start += strlen("\"text\": \"");
            char *end = strchr(start, '"');
            if (end) {
                size_t len = end - start;
                char *result = malloc(len + 1);
                strncpy(result, start, len);
                result[len] = '\0';
                free(chunk.buffer);
                return result;
            }
        }
        free(chunk.buffer);
    }

    return NULL;
}

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

char to_uppercase(char letter) {
    if (letter >= 'a' && letter <= 'z') {
        return letter - 32;
    }
    return letter;
}

void createPlayerList(Word **headPlayer, Word **tailPlayer, int lenght) {
    if(*headPlayer == NULL && *tailPlayer == NULL) {
        for(int i = 0; i < lenght; i++) {
            Word *new = (Word*)malloc(sizeof(Word));

            if(new != NULL) {
                new->index = i+1;
                new->letter = '_';               
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

