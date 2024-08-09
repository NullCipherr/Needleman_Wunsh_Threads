#include "globals.h"
#include "constants.h"

int num_threads;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

char mapaBases[5] = {'A', 'T', 'G', 'C', '-'};
int seqMaior[maxSeq] = {A, A, C, T, T, A};
int seqMenor[maxSeq] = {A, C, T, T, G, A};
int alinhaGMaior[maxSeq];
int alinhaGMenor[maxSeq];
int matrizEscores[maxSeq + 1][maxSeq + 1];
int tamSeqMaior = 6;
int tamSeqMenor = 6;
int tamAlinha;
int penalGap = 0;
int grauMuta = 0;
int escoreDiag;
int escoreLin;
int escoreCol;
int matrizPesos[4][4] = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
};
int indRef = -1;
int nTrocas = -1;
int linPMaior, colPMaior, PMaior;
int linUMaior, colUMaior, UMaior;
