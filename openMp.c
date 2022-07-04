#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "utils.c"

#define ARRAY_SIZE 100000

// Funcao que alterna entre a e b
void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

// Funcao para relalizar a partição do array arr[]
int partition(int arr[], int start, int end)
{
    // Declaração de variáveis
    int pivot = arr[end];
    int i = (start - 1);

    // Realiza a partição
    for (int j = start; j <= end - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[end]);

    // retorna o indice do pivo
    return (i + 1);
}
// Funcao para ordenar o array arr[] utilizando o algoritmo de quicksort
// using openmp
void quicksort(int arr[], int start, int end)
{
    // Declaration
    int index;

    if (start < end)
    {

        // Pega o indice do pivo
        // pela função partition
        index = partition(arr, start, end);

// Parallel sections
#pragma omp parallel sections
        {
#pragma omp section
            {
                // Executa o quicksort para o array esquerdo
                quicksort(arr, start, index - 1);
            }
#pragma omp section
            {
                // Executa o quicksort para o array direito
                quicksort(arr, index + 1, end);
            }
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
    clock_t tic = clock();

    //chamada da função quicksort
    quicksort(arr, 0, ARRAY_SIZE - 1);

    clock_t toc = clock();
    double timeResult = (double)(toc - tic) / CLOCKS_PER_SEC;
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