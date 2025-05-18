#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "raylib.h"
#include <curl/curl.h>

#define NUM_PALAVRAS 10
#define TAM_PALAVRA 51

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
void putIntoArray(char *words[NUM_PALAVRAS], char *ingredients);
void addGeminiList(Word **head, Word **tail, char *ingredient);
void createPlayerList(Word **headPlayer, Word **tailPlayer, int lenght);
bool isPlayerListCorrect(Word *headGemini, Word *headPlayer);
bool checkLetterInStack(NodeStack *headStack, char letter);
int* checkLetter(Word *headGemini, char letter);
void addPlayerList(Word **headPlayer, int *indexes, char letter);
void push(NodeStack **headStack, char letter);
void insertionSort(NodeStack **head);
char to_uppercase(char letter);
void printPilha(NodeStack *head);

int main() {
    const int largura = 800;
    const int altura = 600;

    InitWindow(largura, altura, "Enforkado");
    SetTargetFPS(60);

    GameScreen telaAtual = FASE;

    Word *headGemini = NULL;
    Word *tailGemini = NULL;
    Word *headPlayer = NULL;
    Word *tailPlayer = NULL;
    NodeStack *headStack = NULL;

    int vidas = 5;
    bool palavraCompleta = false;

    char *ingredientes[NUM_PALAVRAS];
    char *resposta = geminiWordGenerator("Retorne 10 ingredientes para uma receita específica, sem instruções, sem pontuação, sem caracteres especiais e separados por espaços. Apenas palavras simples como 'Leite', 'Ovo' ou 'Queijo' que façam uma receita (EM MAIUSCULO)");
    putIntoArray(ingredientes, resposta);

    int palavraAtual = 0;
    addGeminiList(&headGemini, &tailGemini, ingredientes[palavraAtual]);
    createPlayerList(&headPlayer, &tailPlayer, tailGemini->index);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (telaAtual == FASE) {
            DrawText("Acerte o ingrediente secreto!", 20, 20, 20, DARKGRAY);

            // Desenha letras da palavra
            Word *temp = headPlayer;
            int x = 40;
            while (temp != NULL) {
                char letra[2] = { temp->letter, '\0' };
                DrawText(letra, x, 100, 40, BLACK);
                x += 45;
                temp = temp->next;
            }

            // Desenha letras erradas
            DrawText("Letras erradas:", 20, 180, 20, RED);
            NodeStack *erro = headStack;
            int xErro = 200;
            while (erro != NULL) {
                char letra[2] = { erro->letter, '\0' };
                DrawText(letra, xErro, 180, 20, RED);
                xErro += 25;
                erro = erro->next;
            }

            // Mostra vidas
            char vidasTexto[20];
            sprintf(vidasTexto, "Vidas: %d", vidas);
            DrawText(vidasTexto, 20, 220, 20, BLUE);

            // Entrada de teclado
            int key = GetCharPressed();
            if (key >= 'a' && key <= 'z') key -= 32; // transforma para maiúsculo
            if (key >= 'A' && key <= 'Z' && vidas > 0 && !palavraCompleta) {
                char letra = (char)key;
                int *indexes = checkLetter(headGemini, letra);
                if (indexes != NULL && indexes[0] != -1) {
                    addPlayerList(&headPlayer, indexes, letra);
                } else {
                    if (!checkLetterInStack(headStack, letra)) {
                        push(&headStack, letra);
                        insertionSort(&headStack);
                        vidas--;
                    }
                }
                free(indexes);
            }

            if (isPlayerListCorrect(headGemini, headPlayer)) {
                palavraCompleta = true;
                DrawText("Voce acertou!", 20, 300, 30, DARKGREEN);
                DrawText("Pressione ENTER para continuar", 20, 340, 20, DARKGRAY);

                // Aguarda a tecla ENTER para avançar para a próxima palavra
                if (IsKeyPressed(KEY_ENTER)) {
                    // Libera as listas atuais
                    Word *temp = headGemini;
                    while (temp != NULL) {
                        Word *next = temp->next;
                        free(temp);
                        temp = next;
                    }
                    headGemini = NULL;
                    tailGemini = NULL;

                    temp = headPlayer;
                    while (temp != NULL) {
                        Word *next = temp->next;
                        free(temp);
                        temp = next;
                    }
                    headPlayer = NULL;
                    tailPlayer = NULL;

                    NodeStack *tempStack = headStack;
                    while (tempStack != NULL) {
                        NodeStack *next = tempStack->next;
                        free(tempStack);
                        tempStack = next;
                    }
                    headStack = NULL;

                    // Avança para a próxima palavra
                    palavraAtual++;
                    if (palavraAtual < NUM_PALAVRAS && ingredientes[palavraAtual] != NULL) {
                        // Reinicia o estado do jogo
                        addGeminiList(&headGemini, &tailGemini, ingredientes[palavraAtual]);
                        createPlayerList(&headPlayer, &tailPlayer, tailGemini->index);
                        vidas = 5;
                        palavraCompleta = false;
                    } else {
                        // Todas as palavras foram completadas
                        telaAtual = GAMEOVER;
                    }
                }
            } else if (vidas == 0) {
                DrawText("Suas vidas acabaram!", 20, 300, 30, MAROON);
                DrawText("Pressione ENTER para continuar", 20, 340, 20, DARKGRAY);
                // Aguarda a tecla ENTER para ir para GAMEOVER
                if (IsKeyPressed(KEY_ENTER)) {
                    telaAtual = GAMEOVER;
                }
            }
        } else if (telaAtual == GAMEOVER) {
            DrawText("Fim de Jogo!", 20, 300, 30, MAROON);
            DrawText("Pressione ENTER para nova partida ou ESC para sair", 20, 350, 20, DARKGRAY);

            if (IsKeyPressed(KEY_ENTER)) {
                // Libera as listas atuais
                Word *temp = headGemini;
                while (temp != NULL) {
                    Word *next = temp->next;
                    free(temp);
                    temp = next;
                }
                headGemini = NULL;
                tailGemini = NULL;

                temp = headPlayer;
                while (temp != NULL) {
                    Word *next = temp->next;
                    free(temp);
                    temp = next;
                }
                headPlayer = NULL;
                tailPlayer = NULL;

                NodeStack *tempStack = headStack;
                while (tempStack != NULL) {
                    NodeStack *next = tempStack->next;
                    free(tempStack);
                    tempStack = next;
                }
                headStack = NULL;

                // Libera o array de palavras anterior
                for (int i = 0; i < NUM_PALAVRAS; i++) {
                    if (ingredientes[i] != NULL) {
                        free(ingredientes[i]);
                        ingredientes[i] = NULL;
                    }
                }
                if (resposta != NULL) {
                    free(resposta);
                    resposta = NULL;
                }

                // Gera uma nova lista de palavras
                resposta = geminiWordGenerator("Retorne 10 ingredientes para uma receita específica, sem instruções, sem pontuação, sem caracteres especiais e separados por espaços. Apenas palavras simples como 'Leite', 'Ovo' ou 'Queijo' que façam uma receita (EM MAIUSCULO)");
                if (resposta == NULL) {
                    DrawText("Erro ao carregar palavras!", 20, 400, 20, RED);
                    continue;
                }
                putIntoArray(ingredientes, resposta);

                // Reinicia o estado do jogo
                palavraAtual = 0;
                addGeminiList(&headGemini, &tailGemini, ingredientes[palavraAtual]);
                createPlayerList(&headPlayer, &tailPlayer, tailGemini->index);
                vidas = 5;
                palavraCompleta = false;
                telaAtual = FASE;
            }
        }

        EndDrawing();
    }

    // Libera memória restante
    Word *temp = headGemini;
    while (temp != NULL) {
        Word *next = temp->next;
        free(temp);
        temp = next;
    }
    temp = headPlayer;
    while (temp != NULL) {
        Word *next = temp->next;
        free(temp);
        temp = next;
    }
    NodeStack *tempStack = headStack;
    while (tempStack != NULL) {
        NodeStack *next = tempStack->next;
        free(tempStack);
        tempStack = next;
    }
    for (int i = 0; i < NUM_PALAVRAS; i++) {
        if (ingredientes[i] != NULL) free(ingredientes[i]);
    }
    if (resposta != NULL) free(resposta);

    CloseWindow();
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

void addGeminiList(Word **head, Word **tail, char *ingredient){
    
    char palavra[51];
    strcpy(palavra, ingredient);

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

int* checkLetter(Word *headGemini, char letter) {
    int *indexes = (int*)malloc(sizeof(int) * (TAM_PALAVRA + 1));
    if (indexes == NULL) return NULL;

    int i = 0;
    while (headGemini != NULL) {
        if (headGemini->letter == letter) {
            indexes[i] = headGemini->index;
            i++;
        }
        headGemini = headGemini->next;
    }
    indexes[i] = -1;
    return indexes;
}


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

void putIntoArray(char *words[NUM_PALAVRAS], char *ingredients) {
    int i = 0;
    char *token = strtok(ingredients, " ");
    
    while (token != NULL && i < NUM_PALAVRAS) {
        words[i] = malloc(strlen(token) + 1);
        if (words[i] == NULL) {
            fprintf(stderr, "Memory allocation error.\n");
            exit(1);
        }
        strcpy(words[i], token);
        i++;
        token = strtok(NULL, " ");
    }
}