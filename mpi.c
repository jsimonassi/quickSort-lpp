#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.c"


#define ARRAY_SIZE 1000000

void swap(int *arr, int i, int j){
    int t = arr[i];
    arr[i] = arr[j];
    arr[j] = t;
}

//Função que executa o Quick Sort
// para um array arr[] começando do
// start e terminando em end
void quicksort(int *arr, int start, int end){
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

//Função que faz o merge de dois arrays
int * merge(int *arr1, int arr1Size, int *arr2, int arr2Size){
    int *result = (int *)malloc((arr1Size + arr2Size) * sizeof(int));
    int i = 0;
    int j = 0;
    int k;

    // Itera sobre os dois arrays (Soma do tamanho dos dois arrays)
    //E faz a concatenação
    for (k = 0; k < arr1Size + arr2Size; k++){
        if (i >= arr1Size){
            result[k] = arr2[j];
            j++;
        }
        else if (j >= arr2Size){
            result[k] = arr1[i];
            i++;
        }

        else if (arr1[i] < arr2[j]){
            result[k] = arr1[i];
            i++;
        }

        else{
            result[k] = arr2[j];
            j++;
        }
    }
    return result;
}

int main(int argc, char *argv[]){
    int *data = NULL;
    int chunkSize, ownChunkSize;
    int *chunk;
    double time_taken;
    MPI_Status status;

    int numberOfProcess, rankOfProcess;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcess);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankOfProcess);

    // Calculando o tamanho das divisões
    // Se for impar, um processo não é utilizado
    chunkSize = (ARRAY_SIZE % numberOfProcess == 0) ?
                (ARRAY_SIZE / numberOfProcess) :
                (ARRAY_SIZE / (numberOfProcess - 1));

    // Apenas o mestre cria o Array
    if (rankOfProcess == 0){
    
        data = (int *)malloc(numberOfProcess * chunkSize * sizeof(int));
        
        // Gerando array com números aleatórios de 0 a 100
        for(int i = 0; i < ARRAY_SIZE; i++){
            data[i] = (rand() % 101);
        }
    
        // printf("Elementos no array: ");
        // for(int i = 0; i < ARRAY_SIZE; i++){
        //         printf("%d  ", data[i]);
        // }
        // printf("\n");
    }

    // Cria uma barreira para sincronizar os processos. (Espera todos chegarem nesse ponto).
    MPI_Barrier(MPI_COMM_WORLD);

    // Inicia o timer
    time_taken -= MPI_Wtime();

    // Envia o tamanho para todos os processos
    int arrSize = ARRAY_SIZE;
    MPI_Bcast(&arrSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
 
    // Alocando o tamanho de uma subdivisão do array
    chunk = (int *)malloc(chunkSize * sizeof(int));
 
    // Envia a subdivisão do array para todos os processos (Subdivisões de tamanho chunkSize)
    MPI_Scatter(data, chunkSize, MPI_INT, chunk, chunkSize, MPI_INT, 0, MPI_COMM_WORLD);
    free(data);
    data = NULL;
 
    // Calcula a posição final com base no tamanho do chunk
    ownChunkSize = (ARRAY_SIZE >= chunkSize * (rankOfProcess + 1)) ? chunkSize : (ARRAY_SIZE - chunkSize * rankOfProcess);
 
    // Ordena o array para cada subdivisão recebida com o quicksort
    quicksort(chunk, 0, ownChunkSize);
 
    // Itera a cada 2 * processoAtual (step) para juntar os dados novamente.
    for(int step = 1; step < numberOfProcess; step = 2 * step){
        if (rankOfProcess % (2 * step) != 0){
            MPI_Send(chunk, ownChunkSize, MPI_INT, rankOfProcess - step, 0, MPI_COMM_WORLD);
            break;
        }

        if (rankOfProcess + step < numberOfProcess){
            int receivedChunkSize = (ARRAY_SIZE >= chunkSize * (rankOfProcess + 2 * step)) ? (chunkSize * step) : (ARRAY_SIZE - chunkSize * (rankOfProcess + step));
            int *chunkReceived;
            chunkReceived = (int *)malloc(receivedChunkSize * sizeof(int));
            MPI_Recv(chunkReceived, receivedChunkSize, MPI_INT, rankOfProcess + step, 0, MPI_COMM_WORLD, &status);

            data = merge(chunk, ownChunkSize, chunkReceived, receivedChunkSize);

            free(chunk);
            free(chunkReceived);
            chunk = data;
            ownChunkSize = ownChunkSize + receivedChunkSize;
        }
  }
 
    // Para o cronômetro
    time_taken += MPI_Wtime();
 
    // O processo mestre exibe o resultado
    if(rankOfProcess == 0){
        printf("Array Ordenado \n");

        // for (int i = 0; i < ARRAY_SIZE; i++){
        //     printf("%d  ", chunk[i]);
        // }

        printf("\n\nQuicksort ordenou %d inteiros em %d processos no tempo: %f segundos\n", ARRAY_SIZE, numberOfProcess, time_taken);
        saveTime(time_taken);
        
        if(sortTest(chunk, ARRAY_SIZE)){
            printf("Array ordenado com sucesso\n");
        }else{
            printf("O array não foi ordenado corretamente\n");
        } 
    }
 
  MPI_Finalize();
  return 0;
}