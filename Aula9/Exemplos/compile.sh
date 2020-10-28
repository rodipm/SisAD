# $1=file_name $2=num_process
mpicc -o $1 $1.c
# mpirun –np $2 $1