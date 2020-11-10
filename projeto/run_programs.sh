for t in 1 2 4 8; do
    for e in {0..3}; do
        for s in 0 1000 100000 200000; do
            echo Exercíco: $e, Threads: $t, Arquivo2: $s >> out.txt
            for i in {1..10}; do
                ./compile.sh Exercicio$e arquivo2-$s | grep -Ei 'Tempo: [0-9\.]+' | grep -oEi '[0-9\.]+' >> out.txt
                # ./prog1.sh $i | grep -Ei 'Tempo: [0-9\.]+' | grep -oEi '[0-9\.]+' >> out.txt
            done;
        done;
    done;
done;