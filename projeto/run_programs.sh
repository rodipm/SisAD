for e in 0 2; do
    for s in 0 1000 100000 200000; do
        echo Exercíco: $e, Threads: 1, Arquivo2: $s | tee -a out.txt
        for i in {1..10}; do
            ./compile.sh Exercicio$e arquivo2-$s 1 | grep -Ei 'Tempo: [0-9\.]+' | grep -oEi '[0-9\.]+' | tee -a out.txt
        done;
    done;
done;

for t in 1 2 4 8; do
    for e in 1 3; do
        for s in 0 1000 100000 200000; do
            echo Exercíco: $e, Threads: $t, Arquivo2: $s | tee -a out.txt
            for i in {1..10}; do
                ./compile.sh Exercicio$e arquivo2-$s $t | grep -Ei 'Tempo: [0-9\.]+' | grep -oEi '[0-9\.]+' | tee -a out.txt
            done;
        done;
    done;
done;

python3 means_var.py