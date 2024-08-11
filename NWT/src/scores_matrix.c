#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "constants.h"
#include "globals.h"

typedef struct
{
    int start;
    int end;
} ThreadArgs;

void *preenche_matriz(void *arg)
{
    ThreadArgs *args = (ThreadArgs *)arg;
    int start = args->start;
    int end = args->end;
    int lin, col, peso;

    for (lin = start; lin <= end; lin++)
    {
        for (col = 1; col <= tamSeqMaior; col++)
        {
            pthread_mutex_lock(&mutex); // Bloqueia o mutex
            peso = matrizPesos[(seqMenor[lin - 1])][(seqMaior[col - 1])];
            escoreDiag = matrizEscores[lin - 1][col - 1] + peso;
            escoreLin = matrizEscores[lin][col - 1] - penalGap;
            escoreCol = matrizEscores[lin - 1][col] - penalGap;

            if ((escoreDiag > escoreLin) && (escoreDiag > escoreCol))
                matrizEscores[lin][col] = escoreDiag;
            else if (escoreLin > escoreCol)
                matrizEscores[lin][col] = escoreLin;
            else
                matrizEscores[lin][col] = escoreCol;
            pthread_mutex_unlock(&mutex); // Desbloqueia o mutex
        }
    }

    pthread_exit(NULL);
}

void geraMatrizEscores(void)
{
    int lin, col, peso;
    pthread_t threads[num_threads];
    ThreadArgs args[num_threads];
    int chunk_size = (tamSeqMenor + 1) / num_threads;

    printf("\nGeracao da Matriz de escores:\n");

    for (col = 0; col <= tamSeqMaior; col++)
        matrizEscores[0][col] = -1 * (col * penalGap);

    for (lin = 0; lin <= tamSeqMenor; lin++)
        matrizEscores[lin][0] = -1 * (lin * penalGap);

    for (int i = 0; i < num_threads; i++)
    {
        args[i].start = i * chunk_size + 1;
        args[i].end = (i + 1) * chunk_size + 1;
        if (i == num_threads - 1)
            args[i].end = tamSeqMenor + 1;
        pthread_create(&threads[i], NULL, preenche_matriz, (void *)&args[i]);
    }

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    linPMaior = 1;
    colPMaior = 1;
    PMaior = matrizEscores[1][1];

    linUMaior = 1;
    colUMaior = 1;
    UMaior = matrizEscores[1][1];

    for (lin = 1; lin <= tamSeqMenor; lin++)
    {
        for (col = 1; col <= tamSeqMaior; col++)
        {
            if (PMaior < matrizEscores[lin][col])
            {
                linPMaior = lin;
                colPMaior = col;
                PMaior = matrizEscores[lin][col];
            }
            if (UMaior <= matrizEscores[lin][col])
            {
                linUMaior = lin;
                colUMaior = col;
                UMaior = matrizEscores[lin][col];
            }
        }
    }
    printf("\nMatriz de escores Gerada.");
    printf("\nPrimeiro Maior escore = %d na celula [%d,%d]", PMaior, linPMaior, colPMaior);
    printf("\nUltimo Maior escore = %d na celula [%d,%d]", UMaior, linUMaior, colUMaior);
}

void gravaMatrizEscores()
{
    FILE *file;
    int i, lin, col;

    file = fopen("matriz_escores.txt", "w");
    if (file == NULL)
    {
        printf("\nErro ao abrir o arquivo para escrita.");
        return;
    }

    fprintf(file, "Matriz de escores Atual:\n\n");

    fprintf(file, "%4c%4c", ' ', ' ');
    for (i = 0; i <= tamSeqMaior; i++)
        fprintf(file, "%4d", i);
    fprintf(file, "\n");

    fprintf(file, "%4c%4c%4c", ' ', ' ', '-');
    for (i = 0; i < tamSeqMaior; i++)
        fprintf(file, "%4c", mapaBases[(seqMaior[i])]);
    fprintf(file, "\n");

    fprintf(file, "%4c%4c", '0', '-');
    for (col = 0; col <= tamSeqMaior; col++)
        fprintf(file, "%4d", matrizEscores[0][col]);
    fprintf(file, "\n");

    for (lin = 1; lin <= tamSeqMenor; lin++)
    {
        fprintf(file, "%4d%4c", lin, mapaBases[(seqMenor[lin - 1])]);
        for (col = 0; col <= tamSeqMaior; col++)
        {
            fprintf(file, "%4d", matrizEscores[lin][col]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("\nMatriz de escores gravada em 'matriz_escores.txt'.");
}

void mostraMatrizEscores()
{
    int i, lin, col;

    printf("\nMatriz de escores Atual:\n");

    printf("%4c%4c", ' ', ' ');
    for (i = 0; i <= tamSeqMaior; i++)
        printf("%4d", i);
    printf("\n");

    printf("%4c%4c%4c", ' ', ' ', '-');
    for (i = 0; i < tamSeqMaior; i++)
        printf("%4c", mapaBases[(seqMaior[i])]);
    printf("\n");

    printf("%4c%4c", '0', '-');
    for (col = 0; col <= tamSeqMaior; col++)
        printf("%4d", matrizEscores[0][col]);
    printf("\n");

    for (lin = 1; lin <= tamSeqMenor; lin++)
    {
        printf("%4d%4c", lin, mapaBases[(seqMenor[lin - 1])]);
        for (col = 0; col <= tamSeqMaior; col++)
        {
            printf("%4d", matrizEscores[lin][col]);
        }
        printf("\n");
    }
}
