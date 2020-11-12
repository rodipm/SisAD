/*
    Nova proposta de algoritmo sequencial
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "mede_time.h"

#define N_SEQ_ARQ1 6
#define NOME_ARQ_SIZE 50
#define SIZE 100
#define TRUE 1
#define FALSE 0

char sequencias1[N_SEQ_ARQ1][SIZE];
char *sequencias2;
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
    int length = ftell(f2);
    fseek(f2, 0, SEEK_SET);
    sequencias2 = malloc(length);

    fread(sequencias2, 1, length, f2);
    fclose(f2);
}

int main(int argc, char *argv[])
{

    if (argc >= 3)
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
    printf("INICIO_TESTE\n");
    carrega_sequencias_1();
    carrega_sequencias_2();

    int END = FALSE;

    char *seq_index = sequencias2;

    // Percorre toda a variável de sequencias 2
    while (1)
    {
        char *new_line_index = strchr(seq_index, '\n');

        if (!new_line_index)
            break;

        for (int i_seq = 0; i_seq < N_SEQ_ARQ1; i_seq++)
        {
            if (new_line_index - seq_index != strlen(sequencias1[i_seq]) - 1)
                continue;

            if (!strncmp(sequencias1[i_seq], seq_index, new_line_index - seq_index))
            {
                ocorrencias[i_seq]++;
            }
        }
        seq_index = new_line_index+1;
    }

    TIMER_STOP;

    printf("=======================================\n");
    for (int i = 0; i < N_SEQ_ARQ1; i++)
        printf("Total de ocorrencias[%d] = %d\n", i, ocorrencias[i]);
    printf("=======================================\n");

    printf("Tempo: %f \n", TIMER_ELAPSED);
    return 0;
}