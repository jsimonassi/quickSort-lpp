#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

/**
 * @brief      Salva o resultado do tempo em um arquivo txt para consulta futura.
 * @param      execTime   Tempo da execução atual.
*/
int saveTime(double execTime){
    FILE *file;
    file = fopen("times.txt", "a");
    if(file == NULL){
        printf("Erro ao abrir arquivo\n");
        return -1;
    }
    fprintf(file, "%f\n", execTime);
    fclose(file);
    return 0;
}

/**
 * @brief      Calcula a média dos tempos de execução.
 * @param      Tamanho do array a ser recuperado.
 * @return     Média dos dados escritos no array.
*/
double getTimesAvg(int readSize){
    FILE *file;
    double *times = (double *)malloc(sizeof(double) * readSize);
    int i = 0;
    file = fopen("times.txt", "r");
    if(file == NULL){
        printf("Erro ao abrir arquivo\n");
        return -1;
    }
    while(!feof(file)){
        fscanf(file, "%lf", &times[i]);
        i++;
    }
    fclose(file);

    double avg = 0;
    for(int j = 0; j < readSize; j++){
        avg += times[j];
    }
    avg = avg / readSize;
    return avg;
}


/**
 * @brief Limpa o arquivo para execuções futuras.
 * @return int 0 se não houver erro.
 */
int cleanFile(){
    FILE *file;
    file = fopen("times.txt", "w");
    if(file == NULL){
        printf("Erro ao abrir arquivo\n");
        return -1;
    }
    fclose(file);
    return 0;
}

/**
 * @brief Verifica a corretude das ordenações.
 * @return retorna 1(true) se está tudo certo e 0(false) se a ordenação está incorreta.
 */
int sortTest(int *array, int arraySize){
    int itsAllRight = TRUE;
    for(int i = 0; i < arraySize-1; i++){
        if(array[i] > array[i+1]){
            itsAllRight = FALSE;
        }
    }
    return itsAllRight;
}