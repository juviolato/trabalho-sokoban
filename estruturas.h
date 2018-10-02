#ifndef _ESTRUTURAS_H_
#define _ESTRUTURAS_H_

//Definições das estruturas utilizadas

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

typedef struct stJogador //Jogadores
{
    char nome[MAXCHAR+1];
    float pontuacao;
    int niveis;
    int tempo;
    int movimentos;
} jogador_t;

typedef struct struct_jogador_escore //Escores dos jogadores
{
    char nome[MAXCHAR+1];
    float escore;
} jogador_escore_t;

typedef struct stPosFinal //Posições finais para os cubos
{
    int posFinalX;
    int posFinalY;
} POSFINAL_T;

typedef struct sTsave
{
    char nome[MAXCHAR+1];
    float pontuacao;
    int niveis;
    int tempo;
    int movimentos;
    float escore;
    char mapa[MAXLIN*2][MAXCOL*2];
} save_t;

#endif // _ESTRUTURAS_H_
