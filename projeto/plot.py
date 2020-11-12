import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import json

resultados = {}

with open('tempos.txt', 'r') as file:
    resultados = json.load(file)

def plot_variacao_tamanho():
    labels = ['Arquivo2-0', 'Arquivo2-1000', 'Arquivo2-100000', 'Arquivo2-200000']
    ex1 = [resultados["1"]["4"]["0"][0], resultados["1"]["4"]["1000"][0], resultados["1"]["4"]["100000"][0], resultados["1"]["4"]["200000"][0]]
    ex3 = [resultados["3"]["4"]["0"][0], resultados["3"]["4"]["1000"][0], resultados["3"]["4"]["100000"][0], resultados["3"]["4"]["200000"][0]]

    x = np.arange(len(labels))  # the label locations
    width = 0.35  # the width of the bars

    fig, ax = plt.subplots()
    rects1 = ax.bar(x - width/2, ex1, width, label='Exercicio1')
    rects2 = ax.bar(x + width/2, ex3, width, label='Exercicio3')

    # Add some text for labels, title and custom x-axis tick labels, etc.
    ax.set_ylabel('Tempos (s)')
    ax.set_title('Tempos dos programas com 4 threads variando os arquivos')
    ax.set_xticks(x)
    ax.set_xticklabels(labels)
    ax.legend()

    def autolabel(rects):
        for rect in rects:
            height = rect.get_height()
            ax.annotate('{}'.format(height),
                        xy=(rect.get_x() + rect.get_width() / 4, height),
                        xytext=(0, 3),  # 3 points vertical offset
                        textcoords="offset points",
                        ha='center', va='bottom')


    autolabel(rects1)
    autolabel(rects2)

    fig.tight_layout()

    plt.show()

def plot_variacao_threads():
    labels = ['1 Thread', '2 Threads', '4 Threads', '8 Threads']
    ex1 = [resultados["1"]["1"]["200000"][0], resultados["1"]["2"]["200000"][0], resultados["1"]["4"]["200000"][0], resultados["1"]["8"]["200000"][0]]
    ex3 = [resultados["3"]["1"]["200000"][0], resultados["3"]["2"]["200000"][0], resultados["3"]["4"]["200000"][0], resultados["3"]["8"]["200000"][0]]

    x = np.arange(len(labels))  # the label locations
    width = 0.35  # the width of the bars

    fig, ax = plt.subplots()
    rects1 = ax.bar(x - width/2, ex1, width, label='Exercicio1')
    rects2 = ax.bar(x + width/2, ex3, width, label='Exercicio3')

    # Add some text for labels, title and custom x-axis tick labels, etc.
    ax.set_ylabel('Tempos (s)')
    ax.set_title('Tempos dos programas variando o número de threads')
    ax.set_xticks(x)
    ax.set_xticklabels(labels)
    ax.legend()

    def autolabel(rects):
        for rect in rects:
            height = rect.get_height()
            ax.annotate('{}'.format(height),
                        xy=(rect.get_x() + rect.get_width() / 4, height),
                        xytext=(0, 3),  # 3 points vertical offset
                        textcoords="offset points",
                        ha='center', va='bottom')


    autolabel(rects1)
    autolabel(rects2)

    fig.tight_layout()

    plt.show()

def plot_comparacao_exercicios():
    labels = ['Exercicio0', 'Exercicio1', 'Exercicio2', 'Exercicio3']
    medias = [resultados["0"]["1"]["200000"][0], resultados["1"]["4"]["200000"][0], resultados["2"]["1"]["200000"][0], resultados["3"]["8"]["200000"][0]]
    variancias = [resultados["0"]["1"]["200000"][1], resultados["1"]["8"]["200000"][1], resultados["2"]["1"]["200000"][1], resultados["3"]["8"]["200000"][1]]

    x = np.arange(len(labels))  # the label locations
    width = 0.35  # the width of the bars

    fig, ax = plt.subplots()
    rects1 = ax.bar(x - width/2, medias, width, label='Média')
    rects2 = ax.bar(x + width/2, variancias, width, label='Variância')

    # Add some text for labels, title and custom x-axis tick labels, etc.
    ax.set_ylabel('Tempos (s)')
    ax.set_title('Comparação dos tempos de execução dos exercícios')
    ax.set_xticks(x)
    ax.set_xticklabels(labels)
    ax.legend()

    def autolabel(rects):
        for rect in rects:
            height = rect.get_height()
            ax.annotate('{}'.format(height),
                        xy=(rect.get_x() + rect.get_width() / 4, height),
                        xytext=(0, 3),  # 3 points vertical offset
                        textcoords="offset points",
                        ha='center', va='bottom')


    autolabel(rects1)
    autolabel(rects2)

    fig.tight_layout()

    plt.show()

plot_comparacao_exercicios()
plot_variacao_tamanho()
plot_variacao_threads()