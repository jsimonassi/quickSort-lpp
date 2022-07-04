#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.c"

#define ARRAY_SIZE 1000000

/**
 * Método para fazer a troca de dados entre duas posições no vetor
 */
void swap (int * array,int i, const int j){
    int k = array[i];
    array[i] = array[j];
    array[j] = k;
}


/**
 * Método de particionamento que é o núcleo do algoritmo Quicksort.
 * É a implementação utilizando o paradigma dividir para conquistar
 */
int partition (int * array, int start, const int end){
    int i = start;

    for (int j = start; j < end; j++) {
        /* Elemento atual menor ou igual ao pivô? */
        if (array[j] <= array[end]) {
            swap(array,i++, j);
        }
    }
    swap(array,i, end);
    return i;
}

/**Função que executa o Quick Sort
 * para um array[] começando do
 * start e terminando em end
 */
void quicksort (int *array, int start, int end)
{
    if (start >= end) return;

    int pivot = partition(array, start, end);

    quicksort(array, start, pivot - 1);
    quicksort(array, pivot + 1, end);
}


int main(int argc, char *argv[]){
    int * data = (int *)malloc(ARRAY_SIZE * sizeof(int));

    // Gerando array com números aleatórios de 0 a 100
    for(int i = 0; i < ARRAY_SIZE; i++){
        data[i] = (rand() % 101);
    }

    clock_t tic = clock();
    quicksort(data, 0, ARRAY_SIZE - 1);
    clock_t toc = clock();

    double result = (double)(toc - tic) / CLOCKS_PER_SEC;

    printf("\n\nQuicksort ordenou %d inteiros de forma sequencial em: %f segundos\n", ARRAY_SIZE, result);

    saveTime(result);
    if(sortTest(data, ARRAY_SIZE)){
        printf("Array ordenado com sucesso\n");
    }else{
        printf("O array não foi ordenado corretamente\n");
    } 
    return 0;
}