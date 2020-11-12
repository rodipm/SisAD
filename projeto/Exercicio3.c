/*
    Paralelização da nova proposta sequencial
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "mede_time.h"
#include <omp.h>

#define N_SEQ_ARQ1 6
#define NOME_ARQ_SIZE 50
#define SIZE 100
#define TRUE 1
#define FALSE 0

char sequencias1[N_SEQ_ARQ1][SIZE];
char *sequencias2;
int sequencias2_size;
char arq1[NOME_ARQ_SIZE];
char arq2[NOME_ARQ_SIZE];
int ocorrencias[N_SEQ_ARQ1];

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

    // Populando o array de sequencias1
    for (int i = 0; i < N_SEQ_ARQ1; i++)
    {
        fgets(sequencias1[i], SIZE, f1);
        ocorrencias[i] = 0;
    }

    fclose(f1);
}

void carrega_sequencias_2()
{
    FILE *f2;
    f2 = fopen(arq2, "r");

    // Verificacao do nome do arquivo
    if (f2 == NULL)
    {
        printf("Arquivo 2 nao encontrado\n");
        exit(1);
    }

    fseek(f2, 0, SEEK_END);
    sequencias2_size = ftell(f2);
    fseek(f2, 0, SEEK_SET);
    sequencias2 = malloc(sequencias2_size);

    fread(sequencias2, 1, sequencias2_size, f2);
    fclose(f2);
}

int main(int argc, char *argv[])
{

    int n_threads = 2;
    if (argc >= 3)
    {
        strcpy(arq1, argv[1]);
        strcpy(arq2, argv[2]);
    }
    if (argc == 4)
    {
        n_threads = strtol(argv[3], NULL, 10);
    }
    else if (argc < 3)
    {
        printf("Digite o nome do arquivo 1 (seqs a serem buscadas): ");
        scanf("%s", &arq1);
        printf("Digite o nome do arquivo 2 (em que as seqs serao buscadas): ");
        scanf("%s", &arq2);
    }

    omp_set_num_threads(n_threads);

    TIMER_CLEAR;
    TIMER_START;
    printf("INICIO_TESTE\n");
    carrega_sequencias_1();
    carrega_sequencias_2();

    int END = FALSE;

    int chunk_size = sequencias2_size / n_threads;
    // int chunk_size = 570;

    char *seq_index = sequencias2;
    char *seq_index_end = seq_index + chunk_size;

#pragma omp parallel shared(ocorrencias)
    {
#pragma omp single
        {
            // Percorre toda a variável de sequencias 2
            while (END==FALSE)
            {
                // Divide o arquivo de sequencias 2 entre as threads
                if (seq_index_end == &sequencias2[sequencias2_size])
                    END = TRUE;
                char *next_new_line_index = strchr(seq_index_end, '\n');
                char *seq_index_task = seq_index;
                char *seq_index_task_end = seq_index_end;

                if (END == 0)
                    seq_index_task_end += next_new_line_index - seq_index_end;

                // printf("seq_index_task=%p, seq_index_task_end=%p, next_new_line_index=%p\n", seq_index_task, seq_index_task_end, next_new_line_index);
                seq_index = seq_index_task_end;
                if (seq_index + chunk_size >= &sequencias2[sequencias2_size])
                    seq_index_end = &sequencias2[sequencias2_size];
                else
                    seq_index_end = seq_index + chunk_size;

                // printf("seq_index=%p, seq_index_end=%p,final=%p\n", seq_index, seq_index_end, &sequencias2[sequencias2_size]);
                // Lê as sequencias2 conforme
                #pragma omp task firstprivate(seq_index_task, seq_index_task_end)
                                {
                                    int ocorrencias_task[N_SEQ_ARQ1];
                                    for (int i = 0; i < N_SEQ_ARQ1; i++)
                                        ocorrencias_task[i] = 0;

                                    char *new_line_index = NULL;
                                    while (seq_index_task <= seq_index_task_end)
                                    {
                                        new_line_index = strchr(seq_index_task, '\n');

                                        for (int i_seq = 0; i_seq < N_SEQ_ARQ1; i_seq++)
                                        {
                                            if (new_line_index - seq_index_task != strlen(sequencias1[i_seq]) - 1)
                                                continue;

                                            if (!strncmp(sequencias1[i_seq], seq_index_task, new_line_index - seq_index_task))
                                            {

                                                ocorrencias_task[i_seq]++;
                                            }
                                        }
                                        seq_index_task = new_line_index + 1;

                                        if (!new_line_index)
                                            break;
                                    }

                                    for (int i = 0; i < N_SEQ_ARQ1; i++) {
#pragma omp critical
                                        ocorrencias[i] += ocorrencias_task[i];
                                    }
                                }
            }
        }
    }
    TIMER_STOP;

    printf("=======================================\n");
    for (int i = 0; i < N_SEQ_ARQ1; i++)
        printf("Total de ocorrencias[%d] = %d\n", i, ocorrencias[i]);
    printf("=======================================\n");

    printf("Tempo: %f \n", TIMER_ELAPSED);
    return 0;
}