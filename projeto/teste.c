/*
    Paralelização do modelo sequencial do Exercicio2
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "mede_time.h"
#include <omp.h>

#define N_SEQ_ARQ1 6
#define N_SEQ_ARQ2_MAX 20000000
#define SIZE 100
#define NOME_ARQ_SIZE 50
#define TRUE 1
#define FALSE 0

char sequencias1[N_SEQ_ARQ1][SIZE];
char sequencias2[N_SEQ_ARQ2_MAX][SIZE];
int linhas_f1, linhas_f2;
char arq1[NOME_ARQ_SIZE];
char arq2[NOME_ARQ_SIZE];
int ocorrencias[N_SEQ_ARQ1];
int min_len = 2147483647;
int max_len = -1;
char nova_seq[SIZE];
void carrega_sequencias_1()
{
    FILE *f1;
    f1 = fopen(arq1, "r");

    // Verificacao do nome do arquivo
    if (f1 == NULL)
    {
        printf("Arquivo 1 nao encontrado\n");
        exit(1);
    }

    // Numero de linhas
    linhas_f1 = -1;
    linhas_f2 = 0;

    // Populando o array de sequencias1
    while (!feof(f1))
    {
        fgets(sequencias1[++linhas_f1], SIZE, f1);
        ocorrencias[linhas_f1] = 0;
        int _len = strlen(sequencias1[linhas_f1]);
        if (_len > max_len)
            max_len = _len;
        else if (_len < min_len)
            min_len = _len;
    }

    fclose(f1);
}

int main(int argc, char *argv[])
{
    omp_set_num_threads(8);

    if (argc == 3)
    {
        strcpy(arq1, argv[1]);
        strcpy(arq2, argv[2]);
    }
    else
    {
        printf("Digite o nome do arquivo 1 (seqs a serem buscadas): ");
        scanf("%s", &arq1);
        printf("Digite o nome do arquivo 2 (em que as seqs serao buscadas): ");
        scanf("%s", &arq2);
    }

    FILE *f2;
    f2 = fopen(arq2, "r");

    if (f2 == NULL)
    {
        printf("Arquivo 2 nao encontrado\n");
        exit(1);
    }

    TIMER_CLEAR;
    TIMER_START;
    carrega_sequencias_1();

    printf("INICIO\n");
    for (int i_seq = 0; i_seq < linhas_f1; i_seq++)
    {
        int achou = FALSE;
        int i_seq_busca = 0;
        while (1)
        {
            if (i_seq == 0)
            {
                char nova_seq[SIZE];
                int _len;
                if (feof(f2))
                {
                    printf("Numero de Linhas = %d\n", linhas_f2);
                    break;
                }
                fgets(nova_seq, SIZE, f2);
                _len = strlen(nova_seq);
                if (_len > max_len || _len < min_len) continue;
                strcpy(sequencias2[++linhas_f2], nova_seq);
            }
            else
            {
                if (i_seq_busca == linhas_f2)
                    break;
            }

            char *seq1 = sequencias1[i_seq];
            char *seq2 = sequencias2[i_seq_busca];
            if (strlen(seq1) == strlen(seq2))
            {
                achou = TRUE;

                for (int j = 0; j < strlen(seq1); j++)
                {

                    if (seq1[j] != seq2[j])
                    {
                        achou = FALSE;
                        break;
                    }
                }

                if (achou == TRUE)
                {
                    #pragma omp critical
                    ocorrencias[i_seq]++;
                }
            }
            i_seq_busca++;
        }
    }

    printf("=======================================\n");
    for (int i = 0; i < N_SEQ_ARQ1; i++)
        printf("Total de ocorrencias[%d] = %d\n", i, ocorrencias[i]);
    printf("=======================================\n");

    TIMER_STOP;
    printf("Tempo: %f \n", TIMER_ELAPSED);
    fclose(f2);
    return 0;
}