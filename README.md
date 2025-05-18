<p align="center">
  <img
    src="https://img.shields.io/github/repo-size/nogsposito/Enforkado?style=flat"
    alt="Repository Size"
  />
  <img
    src="https://img.shields.io/github/languages/count/nogsposito/Enforkado?style=flat&logo=c"
    alt="Language Count"
  />
  <img
    src="https://img.shields.io/github/commit-activity/t/nogsposito/Enforkado?style=flat&logo=github"
    alt="Commit Activity"
  />
</p>

# 🖥️ Enforkado

## 📄 Descrição

O jogo gira em torno de tentar acertar as letras que compõem as palavras que serão ingredientes para uma receita, caso o usuário tente ,mais de 5 vezes e não acerte as letras, ele irá colocar ingredientes errados e sua receita será um fracasso.

## 🎲 Objetivo do Jogo

Acertar as palavras secretas para cozinhar uma receita de alta pontuação, de acordo com quantas palavras você acertar.

## 🕹️ Como Jogar

Tente acertar o ingrediente secreto inserindo letra por letra. Mas lembre: Você tem apenas 5 vidas para cada ingrediente secreto. Se errar 5 vezes, perde!

## ♟️ Executando o Jogo

Supondo que você já tenha as dependências do Raylib e Curl para o Gemini instalados no seu pc, basta:

1. Ir ao terminal do seu PC e rodar o comanndo `git clone https://github.com/nogsposito/Enforkado` para clonar o repositório em sua máquina
   
2. Entrar no diretório do projeto (Pod-se fazer com `cd Enforkado/` (Atente-se que alguns comandos podem variar dependendo do seu Sistema Operacional)
   
3. Quando estiver dentro do diretório, rodar o comando `make` para compilar o código, por meio da Makefile do projeto.
   
4. Rodar o comando `make run` para, então, rodar o jogo. Assim, deverá aparecer uma interface gráfica com as telas do jogo.
   
5. Quando o jogo acabar e você sair, você pode opcionalmente rodar o comando `make clean` para limpar os arquivos compilados.

## Sobre o código:

O código do jogo está inteiramente em um arquivo (Excluindo suas dependências) e é organizado em diversas funções que se comunicam para fazer o jogo rodar como o esperado. O jogo, em si, aparece por meio de uma interface gráfica utilizando a biblioteca Raylib e também utiliza de integração com Inteligência Artificial ao usar a API do Google Gemini para geração de texto a partir de prompts. 

### Bibliotecas Inclusas

- <stdio.h> para inputs e outputs, assim como testes para o desenvolvimento.

- <stdlib.h> para a utilização de memória dinâmica e funções como "malloc" nas estruturas de dados

- <stdbool.h> para o uso de booleanos.

- <string.h> para manipulção de Strings.

- "raylib.h" para a interface gráfica e acessar funçoes do Raylib.

- <curl/curl.h> para requisições para o Gemini API.

### Estruturas de dados criadas

- MemoryStruct: para execução de funções para as requisições do Gemini.

- Word: para simular uma palavra (Lista duplamente encadeada de letras)

- NodeStack: para simular pilha para letras erradas adivinhadas pelo jogador

### Enums criados

- GameScreen: Determina a tela que está em disposição para o jogador, podendo ser MENU, FASE ou GAMEOVER.

- ColorMode: Determina o modo em que o jogador está jogando o jogo. sendo LIGHTMODE para modo claro ou DARKMODE para escuro.

### Funções Criadas:

- WriteMemoryCallback (static size_t): Para uso interno de geminiWordGenerator.
  
- geminiWordGenerator (char*): Função que chama o Gemini API, que recebe uma String que contém um prompt, requisitando algo à API e retornando uma resposta do Gemini em String.
  
- stringToArray (void): Recebe String (no uso do código, o prompt dado pelo Gemini de 10 ingredientes listados para uma receita aleatória) e uma array, preenchendo o array com cada elemento sendo um dos ingredientes dados.
  
- addSecretIngredient(void): Recebe uma head, uma tail e uma string de uma lista encadeada e um ingrediente (No contxto do código, da lista de letras "palavra" gerada pelo Gemini, a que o jogador deve acertar) e adiciona nela uma nova letra (nó na lista). 
  
- createPlayerList (void): Recebe uma head e uma tail de uma lista encadeada e uma quantidade e cria uma quantidade (recebida no parâmetro) de elemetos na lista.
  
- isPlayerListCorrect(bool): Recebe a head da palavra do Gemini (lista encadeada com letras) e head da palavra do Jogador (que ele constroi com base em suas tentativas) e verifica se a palavra está correta, comparando caso as duas sejam iguais. caso esteja, retorna true, senão retorna false.
  
- toUpperCase (char): Recebe um caractére e o retorna em uppercase.
  
- checkLetterInSecret (int *): Recebe a head da palavra do Gemini e o char dado como input pelo jogador (Quando tenta adivinhar letra) e retorna um array de inteiros que indica em que posições uma letra aparece.
  
- addPlayerList (void): Recebe a head de uma lista e um caractére dado pelo jogador, e adiciona na lista a letra adivinhada pelo jogador (No contexto da lógica, só caso estiver certo).
  
- checkLetterInStack(bool): Recebe uma head de uma pilha e uma letra (char) e verifica se a letra está na pilha, retornando true ou false.
  
- push (void): Recebe uma pilha e uma letra (char) e insere a letra na pilha.
  
- insertionSort (void): recebe um pilha e a reorganiza em ordem de acordo com um algoritmo InsertionSort.


## 👩‍💻 Membros

* Célio Pereira Dias Júnior - cpdj@cesar.school 📩
* João Danilo Correia Jota - jdcj@cesar.school 📩
* João Vitor Moraes Barbosa - jvmb@cesar.school 📩
* Pedro da Costa Carvalho Caldas - pccc@cesar.school 📩
* Vinícius Spósito Antonino Nogueira - vsan@cesar.school 📩
