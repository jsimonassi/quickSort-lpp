#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.c"

#define ARRAY_SIZE 5000000

/**
 * Impleentação sequencial do algoritmo de Quicksort.
 */
void quickSort(int *array, int start, int end) {
	int i, j, pivo, aux;
	i = start;
	j = end-1;
	pivo = array[(start + end) / 2];
	while(i <= j) {
		while(array[i] < pivo && i < end) {
			i++;
		}
		while(array[j] > pivo && j > start) {
			j--;
		}
		if(i <= j) {
			aux = array[i];
			array[i] = array[j];
			array[j] = aux;
			i++;
			j--;
		}
	}
	if(j > start)
		quickSort(array, start, j+1);
	if(i < end)
		quickSort(array, i, end);
}


int main(int argc, char *argv[]){
    int * data = (int *)malloc(ARRAY_SIZE * sizeof(int));

    // Gerando array com números aleatórios de 0 a 100
    for(int i = 0; i < ARRAY_SIZE; i++){
        data[i] = (rand() % 101);
    }

    clock_t tic = clock();
    quickSort(data, 0, ARRAY_SIZE - 1);
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