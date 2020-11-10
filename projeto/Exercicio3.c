/*
    Paralelização do algoritmo sequencial do Exercicio2
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "mede_time.h"
#include <omp.h>

#define N_SEQ_ARQ1 6
#define SIZE 100
#define NOME_ARQ_SIZE 50
#define TRUE 1
#define FALSE 0
// #define CHUNK 200
#define CHUNK 10

char sequencias1[N_SEQ_ARQ1][SIZE];
char sequencias2[CHUNK][SIZE];
int linhas_f1;
char arq1[NOME_ARQ_SIZE];
char arq2[NOME_ARQ_SIZE];
int ocorrencias[N_SEQ_ARQ1];
int min_len = 2147483647;
int max_len = -1;
char nova_seq[SIZE];
int n_threads = 8;

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

    if (argc >= 3)
    {
        strcpy(arq1, argv[1]);
        strcpy(arq2, argv[2]);
    }
    if (argc == 4)
    {
        n_threads = strtol(argv[3], NULL, 10);
    }
    else
    {
        printf("Digite o nome do arquivo 1 (seqs a serem buscadas): ");
        scanf("%s", &arq1);
        printf("Digite o nome do arquivo 2 (em que as seqs serao buscadas): ");
        scanf("%s", &arq2);
    }

    omp_set_num_threads(n_threads);

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

    int num_seqs = 0;
    int END = 0;

#pragma omp parallel firstprivate(num_seqs)
#pragma omp single
    {
        printf("Usando %d threads\n", omp_get_num_threads());
        while (1)
        {
            END = !fgets(nova_seq, SIZE, f2);
            if (!END)
            {
                int _len = strlen(nova_seq);
                if (_len > max_len || _len < min_len)
                    continue;
                strcpy(sequencias2[num_seqs++], nova_seq);
            }
            if (END || num_seqs == CHUNK)
            {
#pragma omp task firstprivate(sequencias2, num_seqs) shared(ocorrencias)
                {
                    for (int i_seq = 0; i_seq < linhas_f1; i_seq++)
                    {
                        char *seq1 = sequencias1[i_seq];
                        int _len1 = strlen(seq1);
                        for (int i_seq_busca = 0; i_seq_busca < num_seqs; i_seq_busca++)
                        {
                            int achou = FALSE;
                            char *seq2 = sequencias2[i_seq_busca];
                            int _len2 = strlen(seq2);
                            if (_len1 == _len2)
                            {
                                achou = TRUE;

                                for (int j = 0; j < _len1; j++)
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
                        }
                    }
                }
#pragma omp critical
                num_seqs = 0;
                if (END)
                    break;
            }
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