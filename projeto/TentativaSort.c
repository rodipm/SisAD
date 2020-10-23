/*
    Paralelização do modelo sequencial do Exercicio2
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "mede_time.h"
#include <omp.h>

#define NUM_THREADS 4
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

// int partition(char a[][SIZE], int p, int r)
// {
//     char *lt[r - p];
//     char *gt[r - p];
//     int i;
//     int j;
//     char *key = a[r];
//     int lt_n = 0;
//     int gt_n = 0;

// #pragma omp parallel for
//     for (i = p; i < r; i++)
//     {
//         if (strlen(a[i]) < strlen(a[r]))
//         {
//             strcpy(lt[lt_n++], a[i]);
//         }
//         else
//         {
//             strcpy(gt[gt_n++], a[i]);
//         }
//     }

//     for (i = 0; i < lt_n; i++)
//     {
//         strcpy(a[p + i], lt[i]);
//     }

//     strcpy(a[p + lt_n], key);

//     for (j = 0; j < gt_n; j++)
//     {
//         strcpy(a[p + lt_n + j + 1], gt[j]);
//     }

//     return p + lt_n;
// }

// void quicksort(char a[][SIZE], int p, int r)
// {
//     int div;

// #pragma omp parallel
//     {
// #pragma omp single
//         {
//             if (p < r)
//             {
//                 div = partition(a, p, r);
//                 {
// #pragma omp task
//                     {
//                         quicksort(a, p, div - 1);
//                     }
// #pragma omp task
//                     {
//                         quicksort(a, div + 1, r);
//                     }
//                 }
//             }
//         }
//     }
// }

void carrega_sequencias()
{
    FILE *f1;
    FILE *f2;
    f1 = fopen(arq1, "r");
    f2 = fopen(arq2, "r");

    // Verificacao do nome do arquivo
    if (f1 == NULL)
    {
        printf("Arquivo 1 nao encontrado\n");
        exit(1);
    }
    if (f2 == NULL)
    {
        printf("Arquivo 2 nao encontrado\n");
        exit(1);
    }

    // Numero de linhas
    linhas_f1 = -1;
    linhas_f2 = -1;

    // Populando os Arrays
    while (!feof(f1))
    {
        fgets(sequencias1[++linhas_f1], SIZE, f1);
        ocorrencias[linhas_f1] = 0;
    }
    while (!feof(f2))
    {
        fgets(sequencias2[++linhas_f2], SIZE, f2);
    }

    fclose(f1);
    fclose(f2);
    printf("Numero de linhas: f1=%d, f2=%d\n", linhas_f1, linhas_f2);
}

int main(int argc, char *argv[])
{
    omp_set_num_threads(NUM_THREADS);

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

    TIMER_CLEAR;
    TIMER_START;
    carrega_sequencias();
    quicksort(sequencias2, 0, linhas_f2);

    for (int i = 0; i < linhas_f2; i++)
        printf("sequencias2[%d]=%s\n", i, sequencias2[i]);
    printf("\n");
    printf("INICIO\n");

#pragma omp parallel for shared(sequencias1, sequencias2, ocorrencias) schedule(dynamic)
    for (int i_seq = 0; i_seq < linhas_f1; i_seq++)
    {
        printf("thread=%d, i_seq=%d\n", omp_get_thread_num(), i_seq);
        int achou = FALSE;
        for (int i_seq_busca = 0; i_seq_busca < linhas_f2; i_seq_busca++)
        {
            char *seq1 = sequencias1[i_seq];
            char *seq2 = sequencias2[i_seq_busca];
            int lenSeq1 = strlen(seq1);
            int lenSeq2 = strlen(seq2);

            if (lenSeq1 > lenSeq2) continue;
            if (lenSeq1 < lenSeq2) break;
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
        }
    }

    printf("=======================================\n");
    for (int i = 0; i < N_SEQ_ARQ1; i++)
        printf("Total de ocorrencias[%d] = %d\n", i, ocorrencias[i]);
    printf("=======================================\n");

    TIMER_STOP;
    printf("Tempo: %f \n", TIMER_ELAPSED);

    // for (int i = 0; i < linhas_f1; i++)
    //     free(sequencias1[i]);
    // free(sequencias1);
    // for (int i = 0; i < linhas_f2; i++)
    //     free(sequencias2[i]);
    // free(sequencias2);

    return 0;
}