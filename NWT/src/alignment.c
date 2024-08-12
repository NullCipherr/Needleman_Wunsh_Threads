#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "constants.h"
#include "globals.h"

// Função para realizar o traceback do alinhamento global
void traceBack(int tipo) {
    int tbLin, tbCol, peso, pos, aux, i;

    // Determina o tipo de alinhamento global a ser gerado
    if (tipo == 1) {
        printf("\nGeracao do Primeiro Maior Alinhamento Global:\n");
        tbLin = linPMaior;
        tbCol = colPMaior;
    } else {
        printf("\nGeracao do Ultimo Maior Alinhamento Global:\n");
        tbLin = linUMaior;
        tbCol = colUMaior;
    }

    pos = 0;
    do {
        // Bloqueia o mutex para garantir acesso exclusivo às variáveis compartilhadas
        pthread_mutex_lock(&mutex);

        // Calcula os escores para a diagonal, linha e coluna
        peso = matrizPesos[(seqMenor[tbLin - 1])][(seqMaior[tbCol - 1])];
        escoreDiag = matrizEscores[tbLin - 1][tbCol - 1] + peso;
        escoreLin = matrizEscores[tbLin][tbCol - 1] - penalGap;
        escoreCol = matrizEscores[tbLin - 1][tbCol] - penalGap;

        // Determina a direção do traceback com base nos escores calculados
        if ((escoreDiag > escoreLin) && (escoreDiag > escoreCol)) {
            if (seqMenor[tbLin - 1] != seqMaior[tbCol - 1]) {
                printf("\nALERTA no TraceBack: Pos = %d Lin = %d e Col = %d\n", pos, tbLin, tbCol);

                // Insere um gap na sequência menor
                alinhaGMenor[pos] = X;
                alinhaGMaior[pos] = seqMaior[tbCol - 1];
                tbCol--;
                pos++;

                // Insere um gap na sequência maior
                alinhaGMenor[pos] = seqMenor[tbLin - 1];
                alinhaGMaior[pos] = X;
                tbLin--;
                pos++;
            } else {
                // Insere os caracteres correspondentes nas sequências
                alinhaGMenor[pos] = seqMenor[tbLin - 1];
                tbLin--;
                alinhaGMaior[pos] = seqMaior[tbCol - 1];
                tbCol--;
                pos++;
            }
        } else if (escoreLin >= escoreCol) {
            // Insere um gap na sequência menor
            alinhaGMenor[pos] = X;
            alinhaGMaior[pos] = seqMaior[tbCol - 1];
            tbCol--;
            pos++;
        } else {
            // Insere um gap na sequência maior
            alinhaGMenor[pos] = seqMenor[tbLin - 1];
            alinhaGMaior[pos] = X;
            tbLin--;
            pos++;
        }

        // Desbloqueia o mutex para permitir o acesso de outras threads
        pthread_mutex_unlock(&mutex);
    } while ((tbLin != 0) && (tbCol != 0));

    // Trata os caracteres restantes na sequência menor
    while (tbLin > 0) {
        alinhaGMenor[pos] = seqMenor[tbLin - 1];
        alinhaGMaior[pos] = X;
        tbLin--;
        pos++;
    }

    // Trata os caracteres restantes na sequência maior
    while (tbCol > 0) {
        alinhaGMenor[pos] = X;
        alinhaGMaior[pos] = seqMaior[tbCol - 1];
        tbCol--;
        pos++;
    }

    // Define o tamanho do alinhamento global
    tamAlinha = pos;

    // Inverte os arrays de alinhamento para obter a ordem correta
    for (i = 0; i < (tamAlinha / 2); i++) {
        aux = alinhaGMenor[i];
        alinhaGMenor[i] = alinhaGMenor[tamAlinha - i - 1];
        alinhaGMenor[tamAlinha - i - 1] = aux;

        aux = alinhaGMaior[i];
        alinhaGMaior[i] = alinhaGMaior[tamAlinha - i - 1];
        alinhaGMaior[tamAlinha - i - 1] = aux;
    }

    printf("\nAlinhamento Global Gerado.");
}

// Função para mostrar o alinhamento global gerado
void mostraAlinhamentoGlobal(void) {
    int i;

    printf("\nAlinhamento Obtido - Tamanho = %d:\n", tamAlinha);

    // Exibe a sequência maior alinhada
    printf("%c", mapaBases[alinhaGMaior[0]]);
    for (i = 1; i < tamAlinha; i++)
        printf("%c", mapaBases[alinhaGMaior[i]]);
    printf("\n");

    // Exibe a sequência menor alinhada
    printf("%c", mapaBases[alinhaGMenor[0]]);
    for (i = 1; i < tamAlinha; i++)
        printf("%c", mapaBases[alinhaGMenor[i]]);
    printf("\n");
}