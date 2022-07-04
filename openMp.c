#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "utils.c"

#define ARRAY_SIZE 100000000

/**
 * Impleentação do algoritmo de Quicksort - openMP.
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
        //Swap
		if(i <= j) {
			aux = array[i];
			array[i] = array[j];
			array[j] = aux;
			i++;
			j--;
		}
	}

// Parallel sections
#pragma omp parallel sections
        {
#pragma omp section
            {
                if(j > start)
		            quickSort(array, start, j+1);
            }
#pragma omp section
            {
       	        if(i < end)
		            quickSort(array, i, end);
            }
        }
}

int main()
{
    // Declaration of array
    int *arr = malloc(ARRAY_SIZE * sizeof(int));

    // Preenchendo o array
    // printf("Vetor desordenado: \n");
    for (int i = 0; i < ARRAY_SIZE; i++){
        arr[i] = rand() % 100;
        // printf("%d ", arr[i]);
    }
    printf("\n");
    double startTime = omp_get_wtime();

    //chamada da função quicksort
    quickSort(arr, 0, ARRAY_SIZE - 1);

    double endTime = omp_get_wtime();
    double timeResult = endTime - startTime;
    printf("\n\nQuicksort ordenou %d inteiros usando openMp com 8 threads (Via Export) em: %f segundos\n", ARRAY_SIZE, timeResult);
    
    //Printa o array ordenado
    // printf("Vetor ordenado: \n");
    // for (int i = 0; i < ARRAY_SIZE; i++)
    // {
    //     printf("%d ", arr[i]);
    // }

    if(sortTest(arr, ARRAY_SIZE)){
        printf("Array ordenado com sucesso\n");
    }else{
        printf("O array não foi ordenado corretamente\n");
    } 

    saveTime(timeResult);
    return 0;
}