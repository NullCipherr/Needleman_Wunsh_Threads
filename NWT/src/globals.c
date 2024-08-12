#include "globals.h"
#include "constants.h"

// Número de threads a serem usadas
int num_threads;

// Mutex para sincronização de threads, inicializado com PTHREAD_MUTEX_INITIALIZER
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Array que mapeia as bases (A, T, G, C e gap '-')
char mapaBases[5] = {'A', 'T', 'G', 'C', '-'};

// Sequência maior inicializada com valores específicos
int seqMaior[maxSeq] = {A, A, C, T, T, A};

// Sequência menor inicializada com valores específicos
int seqMenor[maxSeq] = {A, C, T, T, G, A};

// Array para armazenar o alinhamento global da sequência maior
int alinhaGMaior[maxSeq];

// Array para armazenar o alinhamento global da sequência menor
int alinhaGMenor[maxSeq];

// Matriz de escores para o alinhamento global
int matrizEscores[maxSeq + 1][maxSeq + 1];

// Tamanho da sequência maior
int tamSeqMaior = 6;

// Tamanho da sequência menor
int tamSeqMenor = 6;

// Tamanho do alinhamento global
int tamAlinha;

// Penalidade por gap
int penalGap = 0;

// Grau de mutação
int grauMuta = 0;

// Escores para a diagonal, linha e coluna
int escoreDiag;
int escoreLin;
int escoreCol;

// Matriz de pesos para o alinhamento global
int matrizPesos[4][4] = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
};

// Índice de referência para o alinhamento
int indRef = -1;

// Número de trocas realizadas no alinhamento
int nTrocas = -1;

// Posições e valor máximo na matriz P
int linPMaior, colPMaior, PMaior;

// Posições e valor máximo na matriz U
int linUMaior, colUMaior, UMaior;