#ifndef GLOBALS_H
#define GLOBALS_H

// Inclui a biblioteca pthread.h para suporte a threads POSIX
#include <pthread.h>

// Inclui o arquivo constants.h, que contém definições de constantes
#include "constants.h"

// Declaração de variáveis globais que serão definidas em outro arquivo de implementação

// Número de threads a serem usadas
extern int num_threads;
// Mutex para sincronização de threads
extern pthread_mutex_t mutex;

// Array que mapeia as bases (possivelmente nucleotídeos ou aminoácidos)
extern char mapaBases[5];
// Sequência maior
extern int seqMaior[maxSeq];
// Sequência menor
extern int seqMenor[maxSeq];
// Alinhamento global da sequência maior
extern int alinhaGMaior[maxSeq];
// Alinhamento global da sequência menor
extern int alinhaGMenor[maxSeq];
// Matriz de escores para alinhamento
extern int matrizEscores[maxSeq + 1][maxSeq + 1];
// Tamanho da sequência maior
extern int tamSeqMaior;
// Tamanho da sequência menor
extern int tamSeqMenor;
// Tamanho do alinhamento
extern int tamAlinha;
// Penalidade por gap
extern int penalGap;
// Grau de mutação
extern int grauMuta;
// Escore na diagonal
extern int escoreDiag;
// Escore na linha
extern int escoreLin;
// Escore na coluna
extern int escoreCol;
// Matriz de pesos para alinhamento
extern int matrizPesos[4][4];
// Índice de referência
extern int indRef;
// Número de trocas
extern int nTrocas;
// Posições e valor máximo na matriz P
extern int linPMaior, colPMaior, PMaior;
// Posições e valor máximo na matriz U
extern int linUMaior, colUMaior, UMaior;

#endif // GLOBALS_H