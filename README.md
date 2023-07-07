# Trabalho de Teoria dos Grafos - 2023.1

Trabalho da Disciplina Teoria dos Grafos (DCC059), ministrada no semestre 2023.1

Alunos:

- Lucas de Oliveira Varino (202165090AC)
- Rafael de Oliveira Zimbrão (202165124AC)
- Kayan Martis de Freitas (202176030)

## Compilação
```
g++ -std=c++11 *.cpp -o execGrupo13 -O3 -Ofast
```
Uso para a parte 01:  
```
./execGrupo13 <input_file> <output_file> <isDirected> <isWeightedEdges> <isWeightedNodes>
```
<br>

Uso para a parte 02:
```
./execGrupo13 <input_file> <output_file> <isGreedy> <numIter> <numIterAlpha> <seed>
```

Sendo:
- input_file = caminho do arquivo de entrada
- output_file = caminho do arquivo de saída

- isDirected = 0 para grafo não direcionado e 1 para grafo direcionado
- isWeightedEdges = 0 para grafo não ponderado e 1 para grafo ponderado nas arestas
- isWeightedNodes = 0 para grafo não ponderado e 1 para grafo ponderado nos vértices

```
exemplo: ./execGrupo13 instance.txt output.txt 0 1 0
```

- isGreedy = flag para indicar que a parte 02 será executada
- numIter = quantidade de vezes que os Algoritmos Gulosos Randomizados serão executados
- numIterAlpha = número de iterações para os algoritmo gulosos randomizados em cada execução
- seed = semente para o gerador de números aleatórios

```
exemplo: ./execGrupo13 instance.txt metrics.txt 1 10 5000 1201
```
