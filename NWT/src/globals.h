#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>
#include "constants.h"

extern int num_threads;
extern pthread_mutex_t mutex;

extern char mapaBases[5];
extern int seqMaior[maxSeq];
extern int seqMenor[maxSeq];
extern int alinhaGMaior[maxSeq];
extern int alinhaGMenor[maxSeq];
extern int matrizEscores[maxSeq + 1][maxSeq + 1];
extern int tamSeqMaior;
extern int tamSeqMenor;
extern int tamAlinha;
extern int penalGap;
extern int grauMuta;
extern int escoreDiag;
extern int escoreLin;
extern int escoreCol;
extern int matrizPesos[4][4];
extern int indRef;
extern int nTrocas;
extern int linPMaior, colPMaior, PMaior;
extern int linUMaior, colUMaior, UMaior;

#endif // GLOBALS_H