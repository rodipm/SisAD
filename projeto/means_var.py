import re
import pprint

exercicios = {
    "0": {
        "1": {"0": [], "1000": [], "100000": [], "200000": []}, 
        "2": {"0": [], "1000": [], "100000": [], "200000": []}, 
        "4": {"0": [], "1000": [], "100000": [], "200000": []}, 
        "8": {"0": [], "1000": [], "100000": [], "200000": []}
    },
    "1": {
        "1": {"0": [], "1000": [], "100000": [], "200000": []}, 
        "2": {"0": [], "1000": [], "100000": [], "200000": []}, 
        "4": {"0": [], "1000": [], "100000": [], "200000": []}, 
        "8": {"0": [], "1000": [], "100000": [], "200000": []}
    },
    "2": {
        "1": {"0": [], "1000": [], "100000": [], "200000": []}, 
        "2": {"0": [], "1000": [], "100000": [], "200000": []}, 
        "4": {"0": [], "1000": [], "100000": [], "200000": []}, 
        "8": {"0": [], "1000": [], "100000": [], "200000": []}
    },
    "3": {
        "1": {"0": [], "1000": [], "100000": [], "200000": []}, 
        "2": {"0": [], "1000": [], "100000": [], "200000": []}, 
        "4": {"0": [], "1000": [], "100000": [], "200000": []}, 
        "8": {"0": [], "1000": [], "100000": [], "200000": []}
    }
}

def avr_var_method(lst):
    n = len(lst)
    if n == 0:
        return [0, 0]
    mean = sum(lst) / float(n)
    return [mean, sum((mean - x) ** 2 for x in lst) / float(n - 1)]

with open("out.txt") as f:
    while True:
        header = f.readline()
        print(header)
        if len(header) == 0:
            break

        # Exercíco: $e, Threads: $t, Arquivo2: $s
        m = re.search(r'Exercíco: (\d), Threads: (\d), Arquivo2: (\d)', header)
        _ex = m.group(1)
        _threads = m.group(2)
        _size = m.group(3)
        print(f"Exercíco: {_ex}, Threads: {_threads}, Arquivo2: {_size}")
        
        for i in range(10):
            t = float(f.readline())
            exercicios[_ex][_threads][_size].append(t)
    
for e in range(0, 4):
    for t in [1, 2, 4, 8]:
        for s in [0,1000,100000,200000]:
            exercicios[str(e)][str(t)][str(s)] = avr_var_method(exercicios[str(e)][str(t)][str(s)])

pprint.pprint(exercicios)