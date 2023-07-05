#!/bin/bash

# Compilar o código C++
g++ *.cpp -o main

# Pasta de entrada
input_dir="input"

# Extensão do arquivo de entrada
input_ext=".txt"

# Executar o programa para cada arquivo de entrada
for input_file in $input_dir/*
do
    # Obter o nome do arquivo de entrada sem a extensão
    filename=$(basename "$input_file")
    filename="${filename%$input_ext}"
    
    # Construir o nome do arquivo de saída
    output_file="output/adaptative/metrics_${filename}.txt"
    
    # Executar o programa com o arquivo de entrada e saída correspondentes
    ./main "$input_file" "$output_file"
done