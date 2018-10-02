#ifndef _FUNCOES_H_
#define _FUNCOES_H_

//Funções utilizadas

#include "estruturas.h"

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

void ordenaJogadores(jogador_escore_t top10[], int *numeroJogadores);

void leNome(char nomeDoJogador[]);

void leMapa (char nivel[MAXLIN][MAXCOL], int lvl);

int sorteiaPosicoes(char nivel[MAXLIN][MAXCOL], POSFINAL_T posicoesFinais[MAXCUBOS]);

void ampliarMapa(char nivel[MAXLIN][MAXCOL], char mapa[MAXLIN*2][MAXCOL*2]);

void imprimirMapa(char mapa[MAXLIN*2][MAXCOL*2]);

float calculaEscore(int niveisCompletos, int numeroDeMovimentos, float tempoPassado);

void salvaEstadoAtualTXT(char mapa[MAXLIN+MAXLIN][MAXCOL+MAXCOL], int tempo, int movimentos);

void escreveHighscore(jogador_escore_t jogador, int lidos);

void salvaEscore(char *nome, float escore);

void pausaBin(char mapa[MAXLIN*2][MAXCOL*2], jogador_t jogador, float escore);

void carregaPausa(char mapa[MAXLIN*2][MAXCOL*2], char nome[MAXCHAR], float *pontuacao, int *niveis, int *tempo, int *movimentos);

void imprimeHighscore(jogador_escore_t jogadores[10], int nJogadores);

void menuInicial(char opcao, jogador_t jogadorAtual, jogador_escore_t top10[10], int n);

void imprimeMenu(int *tab, jogador_t jogadorAtual, char mapa[MAXLIN*2][MAXCOL*2], int *pause);

float imprimeInfo(int movimentos, int totalCubos, int cubosFixos, char nomeDoJogador[MAXCHAR], int nivelAtual, float tempoPassado, float tempo, float *acumuladorEscore, int *verificaFim);

int movimento(char input, char nivel[MAXLIN][MAXCOL], int *andou, POSFINAL_T posicoesFinais[MAXCUBOS], int nroCubos, int cubosFixos, int *pause);

int setGameOver (char nivel[MAXLIN][MAXCOL]);

void novoJogo(int level, jogador_t jogadorAtual, int nivelAtual);

#endif // _FUNCOES_H_
