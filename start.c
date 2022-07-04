#include <stdio.h>
#include <stdlib.h>
#include "utils.c"

#define CALLS_NUMBER 10

int callProgram(char *systemArgs)
{
    cleanFile();
    for (int i = 0; i < CALLS_NUMBER; i++)
    {
        printf("Executando %s na iteração %d\n", systemArgs, i);
        system(systemArgs);
    }
    double avg = getTimesAvg(CALLS_NUMBER);
    printf("Tempo médio de execução: %f\n", avg);
}

int main()
{

    int op = -1;
    while (op != 0)
    {
        printf("Escolha uma opcao:\n");
        printf("1 - Sequencial\n2 - MPI\n3 - OpenMP\n");
        scanf("%d", &op);

        if (op == 1)
        {
            callProgram("./quicksort_seq");
        }
        else if (op == 2)
        {
            callProgram("mpirun -np 8 ./mpi");
        }
        else if (op == 3)
        {
            callProgram("./openMp");
        }
        else if(op == 0){
            printf("Saindo...\n");
        }
        else{
            printf("Opcao invalida\n");
        }
    }

    return 0;
}