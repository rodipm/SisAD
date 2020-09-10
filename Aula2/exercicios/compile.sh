#!/bin/bash

gcc -o mult-matrizes-seq ./mult-matrizes-seq.c -O3; gcc -o mult-matrizes-proc mult-matrizes-proc.c -O3; gcc -o mult-matrizes-pthreads mult-matrizes-pthreads.c -O3 -pthread