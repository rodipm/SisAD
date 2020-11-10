echo $1 $2 $3
gcc -fopenmp -o $1 $1.c
./$1 arquivo1 $2 $3