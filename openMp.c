#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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
    // Declaração do tamnho do array
    int N;
    // Recebendo o valor de N
    printf("Digite o tamanho do array: ");
    scanf("%d", &N);

    // Declaration of array
    int *arr = malloc(N * sizeof(int));

    // Preenchendo o array
    printf("Vetor desordenado: \n");
    for (int i = 0; i < N; i++)
    {
        arr[i] = rand() % 100;
        printf("%d ", arr[i]);
    }
    printf("\n");
    clock_t tic = clock();

    //chamada da função quicksort
    quicksort(arr, 0, N - 1);

    clock_t toc = clock();
    printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
    // Printa o array ordenado
    printf("Vetor ordenado: \n");
    for (int i = 0; i < N; i++)
    {
        printf("%d ", arr[i]);
    }

    return 0;
}