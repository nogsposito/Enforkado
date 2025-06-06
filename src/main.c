#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "raylib.h"

#define NUM_INGREDIENTS 10
#define WORD_SIZE 51

typedef enum { MENU, GAME, GAMEOVER } GameScreen;
typedef enum { LIGHTTHEME, DARKTHEME } ColorMode;

// Serve para execução de funções para as requisições do Gemini.
typedef struct MemoryStruct {
    char *buffer;
    size_t size;
} MemoryStruct;

// Lista duplamente encadeada para armazenar letras de uma palavra
typedef struct Word{
    int index;
    char letter;
    struct Word *next;
    struct Word *prev;
} Word;

// Pilha duplamente encadeada para armazenar letras
typedef struct NodeStack{
    char letter;
    struct NodeStack *next;
    struct NodeStack *prev;
} NodeStack;

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
char* geminiWordGenerator(const char *prompt);
void stringToArray(char *ingredients, char *ingredientArray[NUM_INGREDIENTS]);
void addSecretIngredient(Word **headSecret, Word **tailSecret, char *ingredient);
void createPlayerList(Word **headPlayer, Word **tailPlayer, int lenght);
bool isPlayerListCorrect(Word *headSecret, Word *headPlayer);
char toUppercase(char letter);
int* checkLetterInSecret(Word *headSecret, char letter);
void addPlayerList(Word **headPlayer, int *indexes, char letter);
bool checkLetterInStack(NodeStack *headStack, char letter);
void push(NodeStack **headStack, char letter);
void insertionSort(NodeStack **head);
void freeWordList(Word **head, Word **tail);
void freeNodeStackList(NodeStack **head);

int main() {
    char *ingredients[NUM_INGREDIENTS];

    Word *headSecret = NULL;
    Word *tailSecret = NULL;
    Word *headPlayer = NULL;
    Word *tailPlayer = NULL;

    int currentIngredientIndex = 0;

    const int width = 800;
    const int height = 600;

    GameScreen currentScreen = MENU;

    ColorMode currentTheme = LIGHTTHEME;

    Rectangle button = { width / 2 - 100, height / 2 - 25, 200, 50 };
    Rectangle darkThemeButton = { width / 2 - 100, height / 2 + 50, 200, 50 };

    NodeStack *headStack = NULL;

    int lives = 5;

    char *ingredientsString = geminiWordGenerator("Gere exatamente 10 ingredientes típicos para uma receita em português brasileiro. - As palavras devem ser todas em MAIÚSCULAS. Apenas caracteres A-Z, sem acentos, cedilha ou símbolos especiais. Separe os ingredientes com UM espaço apenas. NÃO USE palavras estrangeiras ou espanholas como AZUCAR. Use apenas palavras do português brasileiro. Por exemplo, use ACUCAR. Não adicione nenhuma explicação ou texto extra. Apenas a lista das 10 palavras.");
    stringToArray(ingredientsString, ingredients);

    addSecretIngredient(&headSecret, &tailSecret, ingredients[currentIngredientIndex]);
    createPlayerList(&headPlayer, &tailPlayer, tailSecret->index);

    InitWindow(width, height, "Enforkado");
    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        Color backgroundColor = (currentTheme == LIGHTTHEME) ? RAYWHITE : (Color){30, 30, 30, 255};
        Color textColor = (currentTheme == LIGHTTHEME) ? BLACK : RAYWHITE;
        Color buttonColor = (currentTheme == LIGHTTHEME) ? LIGHTGRAY : DARKGRAY;
        Color titleColor = (currentTheme == LIGHTTHEME) ? DARKGRAY : RAYWHITE;
        Color livesColor = (currentTheme == LIGHTTHEME) ? RED : RED;
        Color correctColor = (currentTheme == LIGHTTHEME) ? DARKGREEN : GREEN;
        Color wrongColor = (currentTheme == LIGHTTHEME) ? RED : RED;

        BeginDrawing();
        ClearBackground(backgroundColor);

        if(currentScreen == MENU){
            DrawText("Bem vindo ao Enforkado!", width / 2 - MeasureText("Bem vindo ao Enforkado!", 20) / 2, 100, 20, titleColor);

            DrawRectangleRec(button, buttonColor);
            DrawText("Começar", width / 2 - MeasureText("Começar", 20) / 2, height / 2 - 10, 20, BLACK);

            DrawRectangleRec(darkThemeButton, buttonColor);
            DrawText(currentTheme == LIGHTTHEME ? "Modo Escuro" : "Modo Claro", width / 2 - MeasureText("Modo Escuro", 20) / 2, height / 2 + 65, 20, BLACK);

            if(CheckCollisionPointRec(GetMousePosition(), button)) {
                DrawRectangleLinesEx(button, 2, RED);

                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    currentScreen = GAME;
                }
            } else {
                DrawRectangleLinesEx(button, 2, BLACK);
            }

            if(CheckCollisionPointRec(GetMousePosition(), darkThemeButton)) {
                DrawRectangleLinesEx(darkThemeButton, 2, RED);

                if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    currentTheme = (currentTheme == LIGHTTHEME) ? DARKTHEME : LIGHTTHEME;
                }
            } else {
                DrawRectangleLinesEx(darkThemeButton, 2, BLACK);
            }
        } else if(currentScreen == GAME) {
            DrawText("Acerte o ingrediente secreto!", 20, 20, 20, textColor);

            // Desenha letras da palavra
            Word *temp = headPlayer;
            int x = 40;
            while (temp != NULL) {
                char letra[2] = { temp->letter, '\0' };
                DrawText(letra, x, 100, 40, textColor);
                x += 45;
                temp = temp->next;
            }

            // Desenha letras erradas
            DrawText("Letras erradas:", 20, 180, 20, textColor);
            NodeStack *erro = headStack;
            int xErro = 200;
            while (erro != NULL) {
                char letra[2] = { erro->letter, '\0' };
                DrawText(letra, xErro, 180, 20, wrongColor);
                xErro += 25;
                erro = erro->next;
            }

            // Mostra vidas
            char vidasTexto[20];
            sprintf(vidasTexto, "Vidas: %d", lives);
            DrawText(vidasTexto, 20, 220, 20, livesColor);

            // Entrada de teclado
            int key = toUppercase(GetCharPressed());
            if (key >= 'A' && key <= 'Z' && lives > 0 && !isPlayerListCorrect(headSecret, headPlayer)) {
                char letra = (char)key;
                int *indexes = checkLetterInSecret(headSecret, letra);
                if (indexes != NULL && indexes[0] != -1) {
                    addPlayerList(&headPlayer, indexes, letra);
                } else {
                    if (!checkLetterInStack(headStack, letra)) {
                        push(&headStack, letra);
                        insertionSort(&headStack);
                        lives--;
                    }
                }
                free(indexes);
            }

            if (isPlayerListCorrect(headSecret, headPlayer)) {
                DrawText("Voce acertou!", 20, 300, 30, correctColor);
                DrawText("Pressione ENTER para continuar", 20, 340, 20, textColor);

                // Aguarda a tecla ENTER para avançar para a próxima palavra
                if (IsKeyPressed(KEY_ENTER)) {
                    // Libera as listas atuais
                    freeWordList(&headSecret, &tailSecret);
                    freeWordList(&headPlayer, &tailPlayer);
                    freeNodeStackList(&headStack);

                    // Avança para a próxima palavra
                    currentIngredientIndex++;
                    if (currentIngredientIndex < NUM_INGREDIENTS && ingredients[currentIngredientIndex] != NULL) {
                        // Reinicia o estado do jogo
                        addSecretIngredient(&headSecret, &tailSecret, ingredients[currentIngredientIndex]);
                        createPlayerList(&headPlayer, &tailPlayer, tailSecret->index);
                        lives = 5;
                    } else {
                        // Todas as palavras foram completadas
                        currentScreen = GAMEOVER;
                    }
                }
            } else if (lives == 0) {
                DrawText("Suas vidas acabaram!", 20, 300, 30, wrongColor);
                DrawText("Pressione ENTER para continuar", 20, 340, 20, textColor);

                // Aguarda a tecla ENTER para ir para GAMEOVER
                if (IsKeyPressed(KEY_ENTER)) {
                    currentScreen = GAMEOVER;
                }
            }
        } else if (currentScreen == GAMEOVER) {
            DrawText("Fim de Jogo!", 20, 300, 30, wrongColor);
            DrawText("Pressione ENTER para nova partida ou ESC para sair", 20, 350, 20, textColor);

            if (IsKeyPressed(KEY_ENTER)) {
                // Libera as listas atuais
                freeWordList(&headSecret, &tailSecret);
                freeWordList(&headPlayer, &tailPlayer);
                freeNodeStackList(&headStack);

                // Libera o array de palavras anterior
                for (int i = 0; i < NUM_INGREDIENTS; i++) {
                    if (ingredients[i] != NULL) {
                        free(ingredients[i]);
                        ingredients[i] = NULL;
                    }
                }

                if (ingredientsString != NULL) {
                    free(ingredientsString);
                    ingredientsString = NULL;
                }

                // Gera uma nova lista de palavras
                ingredientsString = geminiWordGenerator("Gere exatamente 10 ingredientes típicos para uma receita em português brasileiro. - As palavras devem ser todas em MAIÚSCULAS. Apenas caracteres A-Z, sem acentos, cedilha ou símbolos especiais. Separe os ingredientes com UM espaço apenas. NÃO USE palavras estrangeiras ou espanholas como AZUCAR. Use apenas palavras do português brasileiro. Por exemplo, use ACUCAR. Não adicione nenhuma explicação ou texto extra. Apenas a lista das 10 palavras.");
                stringToArray(ingredientsString, ingredients);

                // Reinicia o estado do jogo
                currentIngredientIndex = 0;
                lives = 5;

                addSecretIngredient(&headSecret, &tailSecret, ingredients[currentIngredientIndex]);
                createPlayerList(&headPlayer, &tailPlayer, tailSecret->index);

                currentScreen = MENU;
            }
        }

        EndDrawing();
    }

    CloseWindow();

    // Libera memória restante
    freeWordList(&headSecret, &tailSecret);
    freeWordList(&headPlayer, &tailPlayer);
    freeNodeStackList(&headStack);

    return 0;
}

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)userp;

    char *ptr = realloc(mem->buffer, mem->size + realsize + 1);
    if(ptr == NULL) return 0;

    mem->buffer = ptr;
    memcpy(&(mem->buffer[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->buffer[mem->size] = 0;

    return realsize;
}

// Envia prompt à API Gemini e retorna a resposta (uma string)
char* geminiWordGenerator(const char *prompt) {
    CURL *curl;
    CURLcode res;
    MemoryStruct chunk;

    chunk.buffer = malloc(1);
    chunk.size = 0;

    const char *api_key = "AIzaSyAwO9mHjyuJfKTHLZMfl2TLCeOS5oSPOHk";
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

    if(curl) {
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

        if(res != CURLE_OK) {
            fprintf(stderr, "Erro na requisição: %s\n", curl_easy_strerror(res));
            free(chunk.buffer);
            chunk.buffer = NULL;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    curl_global_cleanup();

    if(chunk.buffer) {
        char *start = strstr(chunk.buffer, "\"text\": \"");
        if(start) {
            start += strlen("\"text\": \"");
            char *end = strchr(start, '"');
            if(end) {
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

// Converte string de ingredientes em array de strings
void stringToArray(char *ingredients, char *ingredientArray[NUM_INGREDIENTS]) {
    int i = 0;
    char *token = strtok(ingredients, " ");
    
    while(token != NULL && i < NUM_INGREDIENTS) {
        ingredientArray[i] = malloc(strlen(token) + 1);

        if(ingredientArray[i] != NULL) {
            strcpy(ingredientArray[i], token);
            i++;
            token = strtok(NULL, " ");
        }
    }
}

// Constrói lista duplamente encadeada com as letras da palavra secreta
void addSecretIngredient(Word **headSecret, Word **tailSecret, char *ingredient){
    int i = 0;
    
    while(ingredient[i] != '\0' && ingredient[i] != '\\' && ingredient[i+1] != 'n'){
        Word *novo = (Word*)malloc(sizeof(Word));

        if(novo != NULL) {
            novo->index = i+1;
            novo->letter = ingredient[i];

            if(*headSecret == NULL){
                *headSecret = novo;
            } else {
                (*tailSecret)->next = novo;  
            }

            novo->prev = *tailSecret;
            *tailSecret = novo;
            (*tailSecret)->next = NULL;
        }

        i++;
    }
}

// Cria lista do jogador com letras em branco ('_')
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

// Verifica se a palavra do jogador está igual à secreta
bool isPlayerListCorrect(Word *headSecret, Word *headPlayer) {
    if(headSecret != NULL && headPlayer != NULL) {
        while(headSecret != NULL && headPlayer != NULL && headSecret->letter == headPlayer->letter) {
            headSecret = headSecret->next;
            headPlayer = headPlayer->next;
        }

        return headSecret == NULL && headPlayer == NULL;
    }

    return false;
}

// Converte letra minúscula para maiúscula
char toUppercase(char letter) {
    if (letter >= 'a' && letter <= 'z') {
        return letter - 32;
    }

    return letter;
}

// Retorna índices onde a letra aparece na palavra secreta
int* checkLetterInSecret(Word *headSecret, char letter) {
    int *indexes = (int*)malloc(sizeof(int) * (WORD_SIZE + 1));
    if(indexes == NULL) return NULL;

    int i = 0;

    while(headSecret != NULL) {
        if(headSecret->letter == letter) {
            indexes[i] = headSecret->index;
            i++;
        }

        headSecret = headSecret->next;
    }

    indexes[i] = -1; // marca final do array

    return indexes;
}

// Atualiza as letras corretas na palavra do jogador
void addPlayerList(Word **headPlayer, int *indexes, char letter) {
    if (*headPlayer == NULL || indexes == NULL) return;

    for (int i = 0; indexes[i] != -1; i++) {
        Word *aux = *headPlayer;

        while (aux != NULL && aux->index != indexes[i]) {
            aux = aux->next;
        }

        if (aux != NULL && aux->index == indexes[i]) {
            aux->letter = letter;
        }
    }
}

// Verifica se a letra já foi usada (está na pilha)
bool checkLetterInStack(NodeStack *headStack, char letter) {
    while(headStack != NULL) {
        if(headStack->letter == letter) {
            return true;
        }

        headStack = headStack->next;
    }

    return false;
}

// Insere letra na pilha
void push(NodeStack **headStack, char letter){
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

// Ordena a pilha de letras em ordem alfabética (insertion sort)
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

void freeWordList(Word **head, Word **tail) {
    Word *aux = *head;

    while(aux != NULL) {
        Word *next = aux->next;
        free(aux);
        aux = next;
    }

    *head = NULL;
    *tail = NULL;
}

void freeNodeStackList(NodeStack **head) {
    NodeStack *aux = *head;

    while(aux != NULL) {
        NodeStack *next = aux->next;
        free(aux);
        aux = next;
    }

    *head = NULL;
}