#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 1000000

void swap(int *arr, int i, int j)
{
    int t = arr[i];
    arr[i] = arr[j];
    arr[j] = t;
}

//Função que executa o Quick Sort
// para um array arr[] começando do
// start e terminando em end
void quicksort(int *arr, int start, int end)
{
    int pivot, index;

    // Caso base
    if (end <= 1)
        return;

    // Escolhe o pivô e troca com o primeiro
    // elemento Pivot é o elemento do meio
    pivot = arr[start + end / 2];
    swap(arr, start, start + end / 2);

    // Etapa de particionamento
    index = start;

    // Iterando no intervalo - início <-> fim
    for (int i = start + 1; i < start + end; i++){

        // Troca se o elemento for menor
        // do que o elemento pivô
        if (arr[i] < pivot){
            index++;
            swap(arr, i, index);
        }
    }

    // Troca usando o pivô
    swap(arr, start, index);

    // Chama recursivamente para os novos intervalos
    quicksort(arr, start, index - start);
    quicksort(arr, index + 1, start + end - index - 1);
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

    printf("\n\nQuicksort ordenou %d inteiros de forma sequencial em: %f segundos\n", ARRAY_SIZE, (double)(toc - tic) / CLOCKS_PER_SEC);
}