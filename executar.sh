#!/bin/bash

# Compilar o código C++
g++ *.cpp -o main

# Pasta de entrada
input_dir="input"

# Arquivo de saída
output_file="output.txt"

# Executar o programa para cada arquivo de entrada
for input_file in $input_dir/*
do
    ./main $input_file $output_file
done
