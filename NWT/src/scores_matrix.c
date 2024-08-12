#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "constants.h"
#include "globals.h"

// Definição da estrutura ThreadArgs para passar argumentos para as threads
typedef struct
{
    int start; // Índice de início para a thread
    int end;   // Índice de fim para a thread
} ThreadArgs;

// Função que preenche a matriz de escores
void *preenche_matriz(void *arg)
{
    ThreadArgs *args = (ThreadArgs *)arg; // Converte o argumento para o tipo ThreadArgs
    int start = args->start; // Obtém o índice de início
    int end = args->end;     // Obtém o índice de fim
    int lin, col, peso;

    // Preenche a matriz de escores para o intervalo de linhas atribuído à thread
    for (lin = start; lin <= end; lin++)
    {
        for (col = 1; col <= tamSeqMaior; col++)
        {
            pthread_mutex_lock(&mutex); // Bloqueia o mutex para garantir acesso exclusivo às variáveis compartilhadas
            peso = matrizPesos[(seqMenor[lin - 1])][(seqMaior[col - 1])]; // Obtém o peso da matriz de pesos
            escoreDiag = matrizEscores[lin - 1][col - 1] + peso; // Calcula o escore diagonal
            escoreLin = matrizEscores[lin][col - 1] - penalGap; // Calcula o escore da linha
            escoreCol = matrizEscores[lin - 1][col] - penalGap; // Calcula o escore da coluna

            // Determina o maior escore e atualiza a matriz de escores
            if ((escoreDiag > escoreLin) && (escoreDiag > escoreCol))
                matrizEscores[lin][col] = escoreDiag;
            else if (escoreLin > escoreCol)
                matrizEscores[lin][col] = escoreLin;
            else
                matrizEscores[lin][col] = escoreCol;
            pthread_mutex_unlock(&mutex); // Desbloqueia o mutex
        }
    }

    pthread_exit(NULL); // Finaliza a thread
}

// Função para gerar a matriz de escores
void geraMatrizEscores(void)
{
    int lin, col, peso;
    pthread_t threads[num_threads]; // Array de threads
    ThreadArgs args[num_threads];    // Array de argumentos para as threads
    int chunk_size = (tamSeqMenor + 1) / num_threads; // Tamanho do bloco de linhas para cada thread

    printf("\nGeracao da Matriz de escores:\n");

    // Inicializa a primeira linha e a primeira coluna da matriz de escores
    for (col = 0; col <= tamSeqMaior; col++)
        matrizEscores[0][col] = -1 * (col * penalGap);

    for (lin = 0; lin <= tamSeqMenor; lin++)
        matrizEscores[lin][0] = -1 * (lin * penalGap);

    // Cria e inicializa as threads
    for (int i = 0; i < num_threads; i++)
    {
        args[i].start = i * chunk_size + 1; // Define o índice de início para a thread
        args[i].end = (i + 1) * chunk_size + 1; // Define o índice de fim para a thread
        if (i == num_threads - 1)
            args[i].end = tamSeqMenor + 1; // Ajusta o índice de fim para a última thread
        pthread_create(&threads[i], NULL, preenche_matriz, (void *)&args[i]); // Cria a thread
    }

    // Espera pela finalização das threads
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Inicializa as variáveis para armazenar o maior escore e suas posições
    linPMaior = 1;
    colPMaior = 1;
    PMaior = matrizEscores[1][1];

    linUMaior = 1;
    colUMaior = 1;
    UMaior = matrizEscores[1][1];

    // Encontra o maior escore na matriz de escores
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

// Função para gravar a matriz de escores em um arquivo
void gravaMatrizEscores()
{
    FILE *file;
    int i, lin, col;

    file = fopen("matriz_escores.txt", "w"); // Abre o arquivo para escrita
    if (file == NULL)
    {
        printf("\nErro ao abrir o arquivo para escrita.");
        return;
    }

    fprintf(file, "Matriz de escores Atual:\n\n");

    // Escreve o cabeçalho da matriz de escores no arquivo
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

    // Escreve a matriz de escores no arquivo
    for (lin = 1; lin <= tamSeqMenor; lin++)
    {
        fprintf(file, "%4d%4c", lin, mapaBases[(seqMenor[lin - 1])]);
        for (col = 0; col <= tamSeqMaior; col++)
        {
            fprintf(file, "%4d", matrizEscores[lin][col]);
        }
        fprintf(file, "\n");
    }

    fclose(file); // Fecha o arquivo
    printf("\nMatriz de escores gravada em 'matriz_escores.txt'.");
}

// Função para mostrar a matriz de escores atual
void mostraMatrizEscores()
{
    int i, lin, col;

    printf("\nMatriz de escores Atual:\n");

    // Exibe o cabeçalho da matriz de escores
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

    // Exibe a matriz de escores
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