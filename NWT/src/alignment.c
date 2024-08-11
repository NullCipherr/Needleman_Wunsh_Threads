#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "constants.h"
#include "globals.h"

void traceBack(int tipo)
{
    int tbLin, tbCol, peso, pos, aux, i;

    if (tipo == 1)
    {
        printf("\nGeracao do Primeiro Maior Alinhamento Global:\n");
        tbLin = linPMaior;
        tbCol = colPMaior;
    }
    else
    {
        printf("\nGeracao do Ultimo Maior Alinhamento Global:\n");
        tbLin = linUMaior;
        tbCol = colUMaior;
    }

    pos = 0;
    do
    {
        pthread_mutex_lock(&mutex); // Bloqueia o mutex
        peso = matrizPesos[(seqMenor[tbLin - 1])][(seqMaior[tbCol - 1])];
        escoreDiag = matrizEscores[tbLin - 1][tbCol - 1] + peso;
        escoreLin = matrizEscores[tbLin][tbCol - 1] - penalGap;
        escoreCol = matrizEscores[tbLin - 1][tbCol] - penalGap;

        if ((escoreDiag > escoreLin) && (escoreDiag > escoreCol))
        {
            if (seqMenor[tbLin - 1] != seqMaior[tbCol - 1])
            {
                printf("\nALERTA no TraceBack: Pos = %d Lin = %d e Col = %d\n", pos, tbLin, tbCol);

                alinhaGMenor[pos] = X;
                alinhaGMaior[pos] = seqMaior[tbCol - 1];
                tbCol--;
                pos++;

                alinhaGMenor[pos] = seqMenor[tbLin - 1];
                alinhaGMaior[pos] = X;
                tbLin--;
                pos++;
            }
            else
            {
                alinhaGMenor[pos] = seqMenor[tbLin - 1];
                tbLin--;
                alinhaGMaior[pos] = seqMaior[tbCol - 1];
                tbCol--;
                pos++;
            }
        }
        else if (escoreLin >= escoreCol)
        {
            alinhaGMenor[pos] = X;
            alinhaGMaior[pos] = seqMaior[tbCol - 1];
            tbCol--;
            pos++;
        }
        else
        {
            alinhaGMenor[pos] = seqMenor[tbLin - 1];
            alinhaGMaior[pos] = X;
            tbLin--;
            pos++;
        }
        pthread_mutex_unlock(&mutex); // Desbloqueia o mutex
    } while ((tbLin != 0) && (tbCol != 0));

    while (tbLin > 0)
    {
        alinhaGMenor[pos] = seqMenor[tbLin - 1];
        alinhaGMaior[pos] = X;
        tbLin--;
        pos++;
    }

    while (tbCol > 0)
    {
        alinhaGMenor[pos] = X;
        alinhaGMaior[pos] = seqMaior[tbCol - 1];
        tbCol--;
        pos++;
    }

    tamAlinha = pos;

    for (i = 0; i < (tamAlinha / 2); i++)
    {
        aux = alinhaGMenor[i];
        alinhaGMenor[i] = alinhaGMenor[tamAlinha - i - 1];
        alinhaGMenor[tamAlinha - i - 1] = aux;

        aux = alinhaGMaior[i];
        alinhaGMaior[i] = alinhaGMaior[tamAlinha - i - 1];
        alinhaGMaior[tamAlinha - i - 1] = aux;
    }

    printf("\nAlinhamento Global Gerado.");
}

void mostraAlinhamentoGlobal(void)
{
    int i;

    printf("\nAlinhamento Obtido - Tamanho = %d:\n", tamAlinha);

    printf("%c", mapaBases[alinhaGMaior[0]]);
    for (i = 1; i < tamAlinha; i++)
        printf("%c", mapaBases[alinhaGMaior[i]]);
    printf("\n");

    printf("%c", mapaBases[alinhaGMenor[0]]);
    for (i = 1; i < tamAlinha; i++)
        printf("%c", mapaBases[alinhaGMenor[i]]);
    printf("\n");
}
