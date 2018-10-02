#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <conio.h>
#include <conio2.h>

#include "estruturas.h"
#include "funcoes.h"

#define NOCURSOR  0
#define MAXCHAR   60
#define MAXLIN    10
#define MAXCOL    25
#define MAXCUBOS  5
#define CIMA      72
#define BAIXO     80
#define ESQUERDA  75
#define DIREITA   77
#define PAUSA     'P'
#define PAREDE    'x'
#define FUNDO     '0'
#define BLOCO     '2'
#define JOGADOR   '1'
#define INVALIDO  '3'
#define GAMEOVER  '4'

//'P' (maiúsculo) não pausa o jogo
//Quando o jogo é pausado, a contagem de tempo não para

int main()
{
    char opcao;
    jogador_t jogadorAtual;
    jogador_escore_t top10[10];
    int n;

    srand(time(NULL));

    menuInicial(opcao, jogadorAtual, top10, n);

    return 0;
}
