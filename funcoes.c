#include "funcoes.h"
#include "estruturas.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <conio.h>
#include <conio2.h>

void ordenaJogadores(jogador_escore_t top10[], int *numeroJogadores) //Lê os highscores salvos e os ordena
{
    FILE *arq;
    jogador_escore_t buffer, temp[50], aux;
    int cont=0, i, j;

    if ( !(arq = fopen("highscore.bin", "r+b")) ) //Abre o arquivo binário de highscores para leitura
    {
        printf("Erro de abertura do arquivo\n");
    }
    else
    {
        while( !feof(arq) )
        {
            if ( (fread(&buffer, sizeof(jogador_escore_t), 1, arq)) != 1)
            {
                //printf("Erro de leitura do arquivo\n");
            }
            else
            {
                temp[cont] = buffer; //Armazena os escores lidos em um vetor
                cont++;
            }
        }

        fclose(arq);
    }

    *numeroJogadores = cont; //Armazena o número de escores lidos em variável

    for (i=1; i<cont; i++)
    {
        for (j=0; j<cont-1; j++)
        {
            if (temp[j].escore > temp[j+1].escore)
            {
                aux = temp[j];
                temp[j] = temp[j+1]; //Ordena os escores do vetor de forma crescente
                temp[j+1] = aux;
            }
        }
    }

    if (cont < 10)
    {
        for (i=0; i<cont; i++)
        {
            top10[i] = temp[i];
        }
    }
    else
    {
        for (i=0; i<10; i++)
        {
            top10[i] = temp[i];
        }
    }

}

void leNome(char nomeDoJogador[]) //Lê o nome do jogador atual
{
    printf("\nInforme o nome do jogador: ");
    fflush(stdin);
    fgets(nomeDoJogador, MAXCHAR, stdin);
    nomeDoJogador[strlen(nomeDoJogador) - 1] = '\0'; //Limpa a quebra de linha no fim da string
}

void leMapa (char nivel[MAXLIN][MAXCOL], int lvl) //Lê o mapa do jogo de acordo com o nível indicado
{
    char buffer;
    FILE* arq;
    int i, j;

    if(lvl == 1)
    {
        arq = fopen("nivel1.txt", "r+"); //Abre o arquivo com o mapa do nível 1
    }
    else
    {
        if(lvl == 2)
        {
            arq = fopen("nivel2.txt", "r+"); //Abre o arquivo com o mapa do nível 2
        }
        else
        {
            arq = fopen("nivel3.txt", "r+"); //Abre o arquivo com o mapa do nível 3
        }
    }

    if(arq == NULL)
    {
        printf("Erro na abertura do arquivo."); //Testa a abertura do arquivo
    }
    else
    {
        for(i=0; i<MAXLIN; i++)
        {
            for(j=0; j<MAXCOL; j++)
            {
                buffer = getc(arq); //Lê caractere por caractere do mapa
                nivel[i][j] = buffer;
            }
        }
        fclose(arq); //Fecha o .txt do nível
    }

    for(i=0; i<MAXLIN; i++)
    {
        nivel[i][24] = '\0'; //Limpa as quebras de linhas da matriz
    }
}

int sorteiaPosicoes(char nivel[MAXLIN][MAXCOL], POSFINAL_T posicoesFinais[MAXCUBOS]) //Sorteia as posições finais para os blocos móveis
{
    int n, pos1, pos2;
    int i, j;
    int posInvalida;

    n = 0;
    for(i=0; i<MAXLIN; i++)
    {
        for(j=0; j<MAXCOL; j++)
        {
            if(nivel[i][j] == '2')
            {
                n++; //Conta o número de posições que precisam ser sorteadas
            }
        }
    }

    for(i=0; i<n; i++)
    {
        do
        {
            pos1 = 1 + rand()%10;
            pos2 = 1 + rand()%25; //Gera duas coordenadas aleatórias
        } while(nivel[pos1][pos2] != '0');
        //Verifica se as coordenadas sorteadas condizem com a área de movimento do mapa

        posicoesFinais[i].posFinalY = pos1; //Adiciona as coordenadas ao vetor de posições finais
        posicoesFinais[i].posFinalX = pos2;
    }
    return n;
}

void ampliarMapa(char nivel[MAXLIN][MAXCOL], char mapa[MAXLIN*2][MAXCOL*2]) //Amplia o mapa lido
{
    int i, j;

    for(i=0; i<MAXLIN; i++)
    {
        for(j=0; j<MAXCOL; j++)
        {
            mapa[i*2][j*2] = nivel[i][j];     //Duplica o tamanho do mapa original
            mapa[i*2][j*2+1] = nivel[i][j];
            mapa[i*2+1][j*2] = nivel[i][j];
            mapa[i*2+1][j*2+1] = nivel[i][j];
        }
    }
}

void imprimirMapa(char mapa[MAXLIN*2][MAXCOL*2]) //Imprime o mapa ampliado
{
    int i, j;

    printf("\n");
    for(i=0; i<MAXLIN*2; i++)
    {
        for(j=0; j<MAXCOL*2; j++)
        {
           switch(mapa[i][j])
           {
           case PAREDE:
            textcolor(WHITE);
            printf("%c", mapa[i][j]); //Imprime as paredes (caractere 'x') na cor branca
            break;
           case FUNDO:
           case INVALIDO:
            textcolor(BLACK);
            printf("%c", mapa[i][j]); //Imprime o fundo da área de movimento (caractere '0') na cor preta
            break;
           case JOGADOR:
            textcolor(LIGHTBLUE);
            printf("%c", mapa[i][j]); //Imprime o jogador (caractere '1') na cor azul
            break;
           case BLOCO:
            textcolor(LIGHTRED);
            printf("%c", mapa[i][j]); //Imprime os blocos móveis (caractere '2') na cor vermelho
            break;
           }
        }
        printf("\n");
    }
    printf("\n");
}

float calculaEscore(int niveisCompletos, int numeroDeMovimentos, float tempoPassado) //Calcula o escore total do jogo atual
{
    float r2, result;
    int r1;

    r1 = pow(10, 3) * pow(niveisCompletos, 3);
    r2 = numeroDeMovimentos + (tempoPassado / (2 * 1.0));
    result = r1 / r2;

    return result;
}

void salvaEstadoAtualTXT(char mapa[MAXLIN+MAXLIN][MAXCOL+MAXCOL], int tempo, int movimentos) //Salva o estado atual do jogo em um arquivo de texto
{
    //Salva tempo, movimentos e mapa do momento em arquivo de
    //texto com o  nome do jogador de acordo com a disponibilidade
    //Recebe matriz do mapa atualizado, variável onde é salvo o tempo e o número de movimentos
    char nomeJogador[MAXCHAR+1], nomeTestado[MAXCHAR+3];
    FILE *arq;
    int i, j;
    char *cat;

    *cat = '1';

    printf("\nDigite o nome do jogador:\n");
    fgets(nomeJogador, MAXCHAR, stdin);
    nomeJogador[strlen(nomeJogador)-1] = '\0';

    //Verifica se existe arquivo com os nomes testados
    //e salva com o primeiro possível

    strcpy(nomeTestado, nomeJogador);
    strcat(nomeTestado, ".txt");

    arq = fopen(nomeTestado, "r");

    while(arq != NULL) //Testa se o arquivo com o nome dado já existe
    {
        fclose(arq);

        nomeJogador[strlen(nomeJogador)] = *cat;
        nomeJogador[strlen(nomeJogador) + 1] = '\0';

        strcpy(nomeTestado, nomeJogador);
        strcat(nomeTestado, ".txt");

        //Acrescenta um índice ao fim do nome caso já exista
        cat++;

        arq = fopen(nomeTestado, "r");
    }

    fclose(arq);
    arq = fopen(nomeTestado, "w");

    if(arq == NULL)
    {
        printf("\nErro na abertura do arquivo.\n");
    }
    else
    {
        //Salva nome do jogador
        fprintf(arq, "Nome: %s\n", nomeJogador);
        //Salva tempo decorrido no jogo
        fprintf(arq, "Tempo: %d\n", tempo);
        //Salva movimentos feitos no jogo
        fprintf(arq, "Numero de movimentos: %d\n", movimentos);
        //Salva mapa
        fprintf(arq, "%s\n", "Mapa: ");

        for (i=0; i<MAXLIN*2; i++)
        {
            for (j=0; j<MAXCOL*2; j++)
            {
                putc((int) mapa[i][j], arq);
            }
            putc('\n', arq);
        }
    }

    fclose(arq);
}

void escreveHighscore(jogador_escore_t jogador, int lidos)
{
    FILE *arq;

    if ( !(arq = fopen("highscore.bin", "a+b")) )
    {
        printf("Erro de abertura do arquivo\n");
    }
    else
    {
        fseek(arq, lidos*sizeof(jogador_escore_t), SEEK_SET);

        if ( (fwrite(&jogador, sizeof(jogador_escore_t), 1, arq)) != 1)
        {
            printf("Erro de escrita no arquivo\n");
        }

        fclose(arq);
    }
}

void salvaEscore(char *nome, float escore) //Salva o escore do jogo
{
    FILE *arq;
    jogador_escore_t jogador;
    jogador_escore_t buffer;
    int lidos = 0, achou = 0;

    strcpy(jogador.nome, nome); //Copia o nome recebido para a estrutura a ser salva
    jogador.escore = escore;

    if (!(arq = fopen("highscore.bin", "r+b"))) //Abre o arquivo binário de highscores no modo de leitura
    {
        //printf("Erro 4 de leitura do arquivo\n");
    }
    else
    {
        while(!(feof(arq)))
        {
            if ((fread(&buffer, sizeof(jogador_escore_t), 1, arq)) == 1)
            {
                if (!(strcmp(buffer.nome, jogador.nome))) //Verifica se já existe um jogador salvo com aquele nome
                {
                    achou = 1;
                    if (jogador.escore > buffer.escore) //Se o escore atual for maior que o escore já salvo desse jogador
                    { //Salva o escore novo
                        fclose(arq);
                        escreveHighscore (jogador, lidos);

                        arq = fopen("highscore.bin", "r+b");

                        fseek(arq, (lidos+1)*sizeof(jogador_escore_t), SEEK_SET);
                    }
                    else
                    {
                        fseek(arq, sizeof(jogador), SEEK_CUR);
                    }
                }
                else
                {
                    fseek(arq, sizeof(jogador), SEEK_CUR);
                }
                lidos++; //Incrementa a quantidade de jogadores lidos
            }
        }
        fclose(arq);
    }
    if (!achou) //Caso não tenha encontrado nenhum jogador com aquele nome
    {
        if ( !(arq = fopen("highscore.bin", "a+b") ) )
        {
            printf("Erro 2 de abertura de arquivo\n");
        }
        else
        {
            if( (fwrite(&jogador, sizeof(jogador_escore_t), 1, arq)) != 1) //Salva o nome o escore do jogador
            {
                printf("Erro de escrita no arquivo\n");
            }
            fclose(arq);
        }
    }
}

void pausaBin(char mapa[MAXLIN*2][MAXCOL*2], jogador_t jogador, float escore) //Salva um arquivo binário para a pausa do jogo
{
    //Salva tempo, movimentos e mapa em arquivo pausa.bin
    //Recebe matriz com o mapa no momento e estrututa do jogador atualizada
    FILE *arq;
    int i,j;
    save_t buffer;

    strcpy(buffer.nome, jogador.nome);
    buffer.pontuacao = jogador.pontuacao;
    buffer.niveis = jogador.niveis;
    buffer.tempo = jogador.tempo;
    buffer.movimentos = jogador.movimentos;
    buffer.escore = escore;

    for (i=0; i<MAXLIN*2; i++)
    {
        for (j=0; j<MAXCOL*2; j++)
        {
            buffer.mapa[i][j] = mapa[i][j];
        }
    }

    if ( !(arq = fopen("pausa.bin", "a+b")) )
    {
        printf("Erro de abertura de arquiv\n");
    }
    else
    {
        if ( fwrite(&buffer, sizeof(save_t), 1, arq) != 1)
        {
            printf("Erro de escrita no arquivo\n");
        }

        fclose(arq);
    }
}

void carregaPausa(char mapa[MAXLIN*2][MAXCOL*2], char nome[MAXCHAR], float *pontuacao, int *niveis, int *tempo, int *movimentos)
{
    FILE *arq;
    int i,j;
    save_t buffer;

    arq = fopen("pausa.bin", "rb");

    if (arq == NULL)
    {
        printf("Erro de abertura do arquivo\n");
    }
    else
    {
        if (fread(&buffer, sizeof(save_t), 1, arq) != 1)
        {
            printf("Erro de leitura do arquivo\n");
        }
        else
        {
            for (i=0; i<MAXLIN*2; i++)
            {
                for (j=0; j<MAXCOL*2; j++)
                {
                    mapa[i][j] = buffer.mapa[i][j];
                }
            }
            strcpy(nome, buffer.nome);
            *pontuacao = buffer.pontuacao;
            *niveis = buffer.niveis;
            *tempo = buffer.tempo;
            *movimentos = buffer.movimentos;
        }

        fclose(arq);
    }
}

void imprimeHighscore(jogador_escore_t jogadores[10], int nJogadores) //Imprime os 10 maiores escores do jogo
{
    //Imprime o top 10
    //Recebe vetor de estruturas jogador_escore_t ordenadas de maneira decrescente
    int i;

    printf("\n    Top 10 maiores escores:\n\n");

    printf("    %-19s %-16s\n", "Nome", "Pontuacao");

    for (i = 0 ; i < nJogadores ; i++)
    {
        printf("    %-19s %-16.2f\n", jogadores[i].nome, jogadores[i].escore);
    }

}

void menuInicial(char opcao, jogador_t jogadorAtual, jogador_escore_t top10[10], int n) //Imprime o menu inicial
{
    do
    {
        clrscr();

        printf("\n  O que deseja fazer?\n");
        printf("\n  (N) Novo Jogo");
        printf("\n  (E) Escore");
        printf("\n  (S) Sair\n");

        opcao = getch();

        switch(opcao)
        {
        case 'n':
        case 'N':
            clrscr();
            leNome(jogadorAtual.nome); //Lê o nome do jogador
            clrscr();
            novoJogo(1, jogadorAtual, 1); //Inicia um jogo novo
            break;
        case 'e':
        case 'E':
            ordenaJogadores(top10, &n);
            imprimeHighscore(top10, n); //Lê os 10 maiores escores registrados e os imprime na tela
            break;
        case 's':
        case 'S':
            salvaEscore(jogadorAtual.nome, jogadorAtual.pontuacao); //Salva o escore atual do jogo
            remove("pausa.bin"); //Deleta o arquivo de pausa
            clrscr();
            printf("\n  Ate logo!\n");
            exit(0); //Termina a execução do programa
            break;
        default:
            clrscr();
            printf("\n  Opcao invalida!\n");
        }

        printf("\n\nPressione 'M' para retornar ao menu.\n");

        getch(); //Espera até que uma nova tecla seja pressionada

    } while(opcao != 'q' && opcao != 'Q');
}

void imprimeMenu(int *tab, jogador_t jogadorAtual, char mapa[MAXLIN*2][MAXCOL*2], int *pause) //Imprime e coordena as funções do menu durante o jogo
{
    char letra;
    jogador_escore_t top10[10];
    int n;

    clrscr();

    textbackground(BLACK);
    textcolor(WHITE);
    printf("(N) Novo Jogo   ");
    printf("(S) Salvar   ");
    printf("(P) Pausar   ");
    printf("(E) Escore   ");
    printf("(Q) Quit\n");

    if (*tab == 1)
    {
        printf("Pressione 'Tab' para fechar o menu. \n");
        do {

            letra = getch();

            if (letra == 9)
            {
                *tab = 0;
            }

            if (*tab != 0) //Caso a tecla tab tenha sido pressionada
            {
                switch (letra)
                {
                    case 'n':
                    case 'N':
                        salvaEscore(jogadorAtual.nome, jogadorAtual.pontuacao);
                        clrscr();
                        leNome(jogadorAtual.nome);
                        clrscr();
                        novoJogo(1, jogadorAtual, 1); //Inicializa um jogo novo
                        break;
                    case 'S':
                    case 's':
                        salvaEscore(jogadorAtual.nome, jogadorAtual.pontuacao);
                        salvaEstadoAtualTXT(mapa, jogadorAtual.tempo, jogadorAtual.movimentos);
                        printf("\n     Jogo salvo!\n");
                        break;
                    case 'P':
                    case 'p':
                        pausaBin(mapa, jogadorAtual, jogadorAtual.pontuacao); //Cria um arquivo de pausa
                        break;
                    case 'E':
                    case 'e':
                        ordenaJogadores(top10, &n);
                        imprimeHighscore(top10, n); //Lê os 10 maiores escores registrados e os imprime na tela
                        break;
                    case 'Q':
                    case 'q':
                        salvaEscore(jogadorAtual.nome, jogadorAtual.pontuacao); //Salva o escore atual do jogo
                        remove("pausa.bin"); //Deleta o arquivo de pausa
                        clrscr();
                        printf("Ate logo!\n");
                        exit(0); //Termina a execução do programa
                        break;
                    default:
                    break;
                }
            }
        } while (*tab != 0);
        clrscr();
    }
}

float imprimeInfo(int movimentos, int totalCubos, int cubosFixos, char nomeDoJogador[MAXCHAR], int nivelAtual, float tempoPassado, float tempo, float *acumuladorEscore, int *verificaFim)
{ //Imprime as informações do jogo atual
    float pontuacao;

    if (*verificaFim == 1)
    {
        *acumuladorEscore = *acumuladorEscore + calculaEscore(nivelAtual, movimentos, tempoPassado);
        pontuacao = calculaEscore(nivelAtual, movimentos, tempoPassado);
    }
    else
    {
        pontuacao = calculaEscore(nivelAtual, movimentos, tempoPassado); //Calcula a pontuação do jogador
    }

    printf("Nivel: %d  ", nivelAtual); //Mostra nível em que o jogador se encontra
    printf("%s: %.2f  ", nomeDoJogador, pontuacao); //Mostra nome do jogador e sua pontuação
    printf("Tempo: %.0fs  ", tempo); //Mostra o tempo restante de jogo
    printf("Movimentos: %d  ", movimentos); //Mostra quantos movimentos já foram feitos
    printf("Cubos: %d/%d", cubosFixos, totalCubos); //Mostra quantos cubos já foram fixados e quantos cubos há no total

    return pontuacao;
}

int movimento(char input, char nivel[MAXLIN][MAXCOL], int *andou, POSFINAL_T posicoesFinais[MAXCUBOS], int nroCubos, int cubosFixos, int *pause)
{ //Gerencia o movimento do jogador
    int i, j, cont;
    int movPossivel = 1;
    int x, y;

    if (*pause == 0)
    {
        switch (input)
        {
            case 'A':
            case 'a':
            case ESQUERDA:
                for (i = 0; i < MAXLIN; i++)
                {
                    for (j = 0; j < MAXCOL; j++)
                    {
                        if (nivel[i][j] == JOGADOR)
                        {
                            if ((nivel[i][j-1] != PAREDE) && (*andou != 1))
                            { //Se o jogador ainda não tiver andado e o bloco à esquerda não for uma parede
                                if((nivel[i][j-1] == BLOCO) && (nivel[i][j-2] != PAREDE) && (nivel[i][j-2] != BLOCO))
                                { //Caso um bloco móvel tenha sido movimentado
                                    nivel[i][j] = FUNDO;
                                    nivel[i][j-2] = BLOCO;
                                    nivel[i][j-1] = JOGADOR;
                                    for(cont = 0; cont < nroCubos; cont++)
                                    {
                                        if((i == posicoesFinais[cont].posFinalY) && (j-2 == posicoesFinais[cont].posFinalX))
                                        { //Caso o bloco móvel tenha sido posicionado numa posição final
                                            cubosFixos++;
                                            nivel[i][j-2] = PAREDE; //Incrementa a contagem de cubos fixos e o transforma em uma parede
                                        }
                                    }
                                }
                                else
                                { //Caso um bloco não tenha sido movimentado
                                    if(nivel[i][j-1] != BLOCO)
                                    {
                                        nivel[i][j] = FUNDO;
                                        nivel[i][j-1] = JOGADOR;
                                    }
                                }

                                *andou = 1;
                            }
                        }
                    }
                }
            break;

            case 'W':
            case 'w':
            case CIMA:
                for (i = 0; i < MAXLIN; i++)
                {
                    for (j = 0; j < MAXCOL; j++)
                    {
                        if (nivel[i][j] == JOGADOR)
                        {
                            if ((nivel[i-1][j] != PAREDE) && (*andou != 1))
                            { //Se o jogador ainda não tiver andado e o bloco acima não for uma parede
                                if((nivel[i-1][j] == BLOCO) && (nivel[i-2][j] != PAREDE) && (nivel[i-2][j] != BLOCO))
                                { //Caso um bloco móvel tenha sido movimentado
                                    nivel[i][j] = FUNDO;
                                    nivel[i-2][j] = BLOCO;
                                    nivel[i-1][j] = JOGADOR;
                                    for(cont = 0; cont < nroCubos; cont++)
                                    {
                                        if((i-2 == posicoesFinais[cont].posFinalY) && (j == posicoesFinais[cont].posFinalX))
                                        {
                                            cubosFixos++;
                                            nivel[i-2][j] = PAREDE; //Incrementa a contagem de cubos fixos e o transforma em uma parede
                                        }
                                    }
                                }
                                else
                                { //Caso um bloco móvel não tenha sido movimentado
                                    if(nivel[i-1][j] != BLOCO)
                                    {
                                        nivel[i][j] = FUNDO;
                                        nivel[i-1][j] = JOGADOR;
                                    }
                                }

                                *andou = 1;
                            }
                        }
                    }
                }
            break;

            case 'D':
            case 'd':
            case DIREITA:
                for (i = 0; i < MAXLIN; i++)
                {
                    for (j = 0; j < MAXCOL; j++)
                    {
                        if (nivel[i][j] == JOGADOR)
                        {
                            if ((nivel[i][j+1] != PAREDE) && (*andou != 1))
                            { //Se o jogador ainda não tiver andado e o bloco à direita não for uma parede
                                if((nivel[i][j+1] == BLOCO) && (nivel[i][j+2] != PAREDE) && (nivel[i][j+2] != BLOCO))
                                { //Caso um bloco móvel tenha sido movimentado
                                    nivel[i][j] = FUNDO;
                                    nivel[i][j+2] = BLOCO;
                                    nivel[i][j+1] = JOGADOR;
                                    for(cont = 0; cont < nroCubos; cont++)
                                    {
                                        if((i == posicoesFinais[cont].posFinalY) && (j+2 == posicoesFinais[cont].posFinalX))
                                        {
                                            cubosFixos++;
                                            nivel[i][j+2] = PAREDE; //Incrementa a contagem de cubos fixos e o transforma em uma parede
                                        }
                                    }
                                }
                                else
                                { //Caso um bloco móvel não tenha sido movimentado
                                    if(nivel[i][j+1] != BLOCO)
                                    {
                                        nivel[i][j] = FUNDO;
                                        nivel[i][j+1] = JOGADOR;
                                    }
                                }

                                *andou = 1;
                            }
                        }
                    }
                }
            break;

            case 'S':
            case 's':
            case BAIXO:
                for (i = 0; i < MAXLIN; i++)
                {
                    for (j = 0; j < MAXCOL; j++)
                    {
                        if (nivel[i][j] == JOGADOR)
                        {
                            if ((nivel[i+1][j] != PAREDE) && (*andou != 1))
                            { //Se o jogador ainda não tiver andado e o bloco abaixo não for uma parede
                                if((nivel[i+1][j] == BLOCO) && (nivel[i+2][j] != PAREDE) && (nivel[i+2][j] != BLOCO))
                                { //Caso um bloco móvel tenha sido movimentado
                                    nivel[i][j] = FUNDO;
                                    nivel[i+2][j] = BLOCO;
                                    nivel[i+1][j] = JOGADOR;
                                    for(cont = 0; cont < nroCubos; cont++)
                                    {
                                        if((i+2 == posicoesFinais[cont].posFinalY) && (j == posicoesFinais[cont].posFinalX))
                                        {
                                            cubosFixos++;
                                            nivel[i+2][j] = PAREDE; //Incrementa a contagem de cubos fixos e o transforma em uma parede
                                        }
                                    }
                                }
                                else
                                { //Caso um bloco móvel não tenha sido movimentado
                                    if(nivel[i+1][j] != BLOCO)
                                    {
                                        nivel[i][j] = FUNDO;
                                        nivel[i+1][j] = JOGADOR;
                                    }
                                }

                                *andou = 1;
                            }
                        }
                    }
                }
            break;

            default:
            break;
        }
    }

    return cubosFixos;
}

int setGameOver (char nivel[MAXLIN][MAXCOL])
{
    int isOver = 0;
    int i, j, k;
    int contCantos, blocosPerdidos = 0;

    for(i = 0; i < MAXLIN; i++)
    {
        for (j = 0; j < MAXCOL; j++)
        {
            if (nivel[i][j] == BLOCO)
            {
                if(nivel[i+1][j] == PAREDE || nivel[i-1][j] == PAREDE)
                {
                    if(nivel[i][j+1] == PAREDE || nivel[i][j-1] == PAREDE)
                    {
                        blocosPerdidos++;
                    }
                }
            }
        }
    }

    if(blocosPerdidos != 0)
    {
        isOver = 1;
    }

    return isOver;
}

void novoJogo(int level, jogador_t jogadorAtual, int nivelAtual) //Inicia um novo nível
{
    char nivel[MAXLIN][MAXCOL];
    char mapa[MAXLIN*2][MAXCOL*2];
    char input;
    int tab = 0, condicaoDeTermino = 0, andou = 0, movimentos = 0, cubosFixos = 0, levelAtual;
    int nroCubos;
    POSFINAL_T posicoesFinais[MAXCUBOS];

    int pause = 0;

    float tempoLimite = 120;
    float tempoIncial = clock();
    float tempoElapsed, tempoLeft;
    static float acumuladorEscore;

    if (nivelAtual == 1)
    {
        acumuladorEscore = 0;
    }

    int verificaFim = 0;

    levelAtual = level;
    leMapa(nivel, levelAtual);
    nroCubos = sorteiaPosicoes(nivel, posicoesFinais);

    do {

        if (tab != 0)
        {
            imprimeMenu(&tab, jogadorAtual, mapa, &pause); //Disponibiliza o menu caso a tecla TAB senha sido pressionada
        }
        imprimeMenu(&tab, jogadorAtual, mapa, &pause);

        if(pause == 0)
        {
            tempoElapsed = (clock() - tempoIncial) / 1000.0; //Calcula o tempo que já passou (segundos)
            tempoLeft = tempoLimite - tempoElapsed; //Calcula o tempo restante de jogo
        }

        printf("\n");
        jogadorAtual.pontuacao = imprimeInfo(movimentos, nroCubos, cubosFixos, jogadorAtual.nome, nivelAtual, tempoElapsed, tempoLeft, &acumuladorEscore, &verificaFim);
        //Calcula a pontuação do jogador atual
        printf("\n");

        ampliarMapa(nivel, mapa);
        imprimirMapa(mapa);
        //Amplia e imprime o mapa do jogo

        andou = 0; //Flag para movimento do jogador
        input = getch();

        if (input == 9)
        {
            tab++;
        }

        if(input == 'p')
        {
            if(pause == 0)
            {
                pause = 1;
                pausaBin(mapa, jogadorAtual, jogadorAtual.pontuacao); //Cria um arquivo de pausa
            }
            else if (pause == 1)
            {
                carregaPausa(mapa, jogadorAtual.nome, &jogadorAtual.pontuacao, &levelAtual, &jogadorAtual.tempo, &jogadorAtual.movimentos);
                //Carrega o arquivo de pausa criado
                pause = 0;
            }
        }

        cubosFixos = movimento(input, nivel, &andou, posicoesFinais, nroCubos, cubosFixos, &pause);
        jogadorAtual.tempo = tempoLeft; //Armazena o tempo restante na estrutura do jogador atual

        if(andou == 1)
        {
            movimentos++; //Incrementa a quantidade de movimentos dados
            jogadorAtual.movimentos = movimentos; //Armazena o número de movimentos na estrutura do jogador atual
        }

        if(cubosFixos == nroCubos) //Verifica se todos os cubos móveis já foram fixados
        {
            condicaoDeTermino = 1;
            verificaFim = 1;
        }

        if (tempoLeft <= 0) //Se o tempo de jogo tiver se esgotado
        {
            condicaoDeTermino = 3;
        }

        if (setGameOver(nivel) == 1) //Se um dos blocos móveis não puder ser colocado em alguma posição final
        {
            condicaoDeTermino = 2;
        }

        if(condicaoDeTermino == 1)
        {
            clrscr();
            imprimeMenu(&tab, jogadorAtual, mapa, &pause);
            printf("\n");
            jogadorAtual.pontuacao = imprimeInfo(movimentos, nroCubos, cubosFixos, jogadorAtual.nome, nivelAtual, tempoElapsed, tempoLeft, &acumuladorEscore, &verificaFim);
            printf("\n");
            ampliarMapa(nivel, mapa);
            imprimirMapa(mapa);
            sleep(1);
        }

        clrscr();
    } while (condicaoDeTermino == 0);

    if(condicaoDeTermino == 1) //Caso o nível tenha sido vencido
    {
        levelAtual++; //Incrementa o nível atual
        if(levelAtual < 4) //Se o nível finalizado não tiver sido o último
        {
            novoJogo(levelAtual, jogadorAtual, levelAtual); //Inicia o próximo nível
        }
        else //Caso todos os níveis estejam completos
        {
            salvaEscore(jogadorAtual.nome, jogadorAtual.pontuacao); //Salva escore do jogo
            printf("\n  JOGO VENCIDO!\n\n");
            printf("  Total de pontos: %.2f", acumuladorEscore); //Exibe a pontuação total
        }
    }
    else if(condicaoDeTermino == 2) //Caso algum bloco móvel não possa mais ser colocado em uma posição final
    {
        salvaEscore(jogadorAtual.nome, jogadorAtual.pontuacao);
        printf("\n  Nao ha como tirar a caixa da parede. ):\n");
    }
    else if(condicaoDeTermino == 3) //Caso o temp
    {
        salvaEscore(jogadorAtual.nome, jogadorAtual.pontuacao);
        printf("\n  O tempo acabou :,C\n");
    }
}
