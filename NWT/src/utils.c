#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "globals.h"

// Função para ler a penalidade de gap
int lePenalidade(void) {
    int penal; // Variável para armazenar a penalidade de gap

    printf("\nLeitura da Penalidade de Gap:");
    do {
        // Solicita ao usuário que insira a penalidade de gap
        printf("\nDigite valor >= 0 = ");
        scanf("%d", &penal);
    } while (penal < 0); // Continua o loop até que um valor válido seja inserido

    return penal; // Retorna a penalidade de gap
}

// Função para ler a matriz de pesos
void leMatrizPesos() {
    int i, j;

    printf("\nLeitura da Matriz de Pesos:\n");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            // Solicita ao usuário que insira os valores da matriz de pesos
            printf("Digite valor %c x %c = ", mapaBases[i], mapaBases[j]);
            scanf("%d", &(matrizPesos[i][j]));
        }
        printf("\n");
    }
}

// Função para mostrar a matriz de pesos atual
void mostraMatrizPesos(void) {
    int i, j;

    printf("\nMatriz de Pesos Atual:");
    // Exibe o cabeçalho da matriz de pesos
    printf("\n%4c%4c%4c%4c%4c\n", ' ', 'A', 'T', 'G', 'C');
    for (i = 0; i < 4; i++) {
        // Exibe a linha da matriz de pesos
        printf("%4c", mapaBases[i]);
        for (j = 0; j < 4; j++)
            printf("%4d", matrizPesos[i][j]);
        printf("\n");
    }
}

// Função para ler a porcentagem máxima de mutação aleatória
int leGrauMutacao(void) {
    int prob; // Variável para armazenar a porcentagem de mutação

    printf("\nLeitura da Porcentagem Maxima de Mutacao Aleatoria:\n");
    do {
        // Solicita ao usuário que insira a porcentagem de mutação
        printf("\nDigite 0 <= valor <= 100 = ");
        scanf("%d", &prob);
    } while ((prob < 0) || (prob > 100)); // Continua o loop até que um valor válido seja inserido

    return prob; // Retorna a porcentagem de mutação
}