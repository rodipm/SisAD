# 1. Fazer download dos programas

# 2. Compilar e executar inicia-matrizes-seq.c

Esse programa contém três matrizes (a, b, c) de tamanho especificado pelo usuário (máximo 2000) que são inicializadas de maneira incremental percorrendo todas as colunas de cada uma das linhas. Os valores de cada elemento da matriz dependem do índice (i, j) e do valor que tinha no passo anterior.

Trata-se de uma versão sequencial do programa.

Pode ser compilado com:

```
> gcc -O3 inicia-matrizes-seq.c -o inicia-matrizes-seq
```

# 3. Compilar e executar inicia-matrizes-proc.c

Trata-se do mesmo programa anterior, apenas aplicando tecnicas de memória compartilhada, segmentando as linhas da matriz e as separando entre os processos gerados. Sendo assim cada processo atua paralelamente e independentemente com os diferentes segmentos da matriz (definidos pelo id de cada processo)

```
> gcc -o inicia-matrizes-proc inicia-matrizes-proc.c -O3
```

# 4. Paralelizar mult-matrizes-seq.c utilizando processos.

O programa mult-matrizes-seq.c apresenta uma algoritmo simples de multiplicação de matrizes de forma sequencial, sem nenhum tipo de paralelização. A idéia é utilizar processos para paralelizar o acesso as matrizes e permitir que a matriz c seja calculada de maneira segmentada, sendo cada processo responsável por um deles.

Utiliza-se o mesmo principio de segmentação utilizado no exemplo "inicia-matrizes-proc" designando-se uma faixa de linhas para cada processo, de acordo com o numero de processos criados e o tamanho da matrix a ser calculada. Desta maneira, cada processo atua em um conjunto de linhas diferentes do outros.

# 5. Paralelizar mult-matrizes-seq.c utilizando pthreads.

Neste caso tem-se a paralelização por threads que, diferente dos processos, compartilham memória naturalmente, além de apresentar tempos menores de criação e destruição em comparação aos processos.

A estratégia de paralelização é idêntica a dos casos anteriores, utilizando segmentação da matriz entre as threads.

```
gcc -o mult-matrizes-pthreads mult-matrizes-pthreads.c -O3 -lpthread
```
