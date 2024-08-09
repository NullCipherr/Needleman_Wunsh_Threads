#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "globals.h"

int leTamMaior(void) {
    printf("\nLeitura do Tamanho da Sequencia Maior:");
    do {
        printf("\nDigite 0 < valor < %d = ", maxSeq);
        scanf("%d", &tamSeqMaior);
    } while ((tamSeqMaior < 1) || (tamSeqMaior > maxSeq));
    return tamSeqMaior;
}

int leTamMenor(void) {
    printf("\nLeitura do Tamanho da Sequencia Menor:");
    do {
        printf("\nDigite 0 < valor <= %d = ", tamSeqMaior);
        scanf("%d", &tamSeqMenor);
    } while ((tamSeqMenor < 1) || (tamSeqMenor > tamSeqMaior));
    return tamSeqMenor;
}

void leSequencias(void) {
    int i, erro;
    char seqMaiorAux[maxSeq], seqMenorAux[maxSeq];

    indRef = -1;
    nTrocas = -1;
    printf("\nLeitura das Sequencias:\n");

    // lendo a sequencia maior
    do {
        printf("\nPara a Sequencia Maior,");
        printf("\nDigite apenas caracteres 'A', 'T', 'G' e 'C'");
        do {
            printf("\n> ");
            fgets(seqMaiorAux, maxSeq, stdin);
            tamSeqMaior = strlen(seqMaiorAux) - 1; // remove o enter
        } while (tamSeqMaior < 1);
        printf("\ntamSeqMaior = %d\n", tamSeqMaior);
        i = 0;
        erro = 0;
        do {
            switch (seqMaiorAux[i]) {
            case 'A':
                seqMaior[i] = A;
                break;
            case 'T':
                seqMaior[i] = T;
                break;
            case 'G':
                seqMaior[i] = G;
                break;
            case 'C':
                seqMaior[i] = C;
                break;
            default:
                erro = 1; // nao eh permitido qquer outro caractere
            }
            i++;
        } while ((erro == 0) && (i < tamSeqMaior));
    } while (erro == 1);

    // lendo a sequencia menor
    do {
        printf("\nPara a Sequencia Menor, ");
        printf("\nDigite apenas caracteres 'A', 'T', 'G' e 'C'");
        do {
            printf("\n> ");
            fgets(seqMenorAux, maxSeq, stdin);
            tamSeqMenor = strlen(seqMenorAux) - 1; // remove o enter
        } while ((tamSeqMenor < 1) || (tamSeqMenor > tamSeqMaior));
        printf("\ntamSeqMenor = %d\n", tamSeqMenor);

        i = 0;
        erro = 0;
        do {
            switch (seqMenorAux[i]) {
            case 'A':
                seqMenor[i] = A;
                break;
            case 'T':
                seqMenor[i] = T;
                break;
            case 'G':
                seqMenor[i] = G;
                break;
            case 'C':
                seqMenor[i] = C;
                break;
            default:
                erro = 1;
            }
            i++;
        } while ((erro == 0) && (i < tamSeqMenor));
    } while (erro == 1);
}

void leSequenciasDeArquivo(void) {
    FILE *file;
    char filename[100];
    int i;

    printf("\nDigite o nome do arquivo: ");
    scanf("%s", filename);

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("\nErro ao abrir o arquivo.");
        return;
    }

    printf("\nLeitura das Sequencias do Arquivo:\n");

    fscanf(file, "%d %d", &tamSeqMaior, &tamSeqMenor);
    if (tamSeqMaior > maxSeq || tamSeqMenor > maxSeq || tamSeqMenor > tamSeqMaior) {
        printf("\nErro nos tamanhos das sequencias.");
        fclose(file);
        return;
    }

    for (i = 0; i < tamSeqMaior; i++) {
        fscanf(file, " %c", &mapaBases[seqMaior[i]]);
    }

    for (i = 0; i < tamSeqMenor; i++) {
        fscanf(file, " %c", &mapaBases[seqMenor[i]]);
    }

    fclose(file);
}

void geraSequencias(void) 
{
    int i, dif, probAux;
    char base;

    printf("\nGeracao Aleatoria das Sequencias:\n");

    // gerando a sequencia maior
    for (i = 0; i < tamSeqMaior; i++) {
        base = rand() % 4; // produz valores de 0 a 3
        seqMaior[i] = base;
    }

    dif = tamSeqMaior - tamSeqMenor; // diferenca entre os tamanhos das sequencias

    indRef = 0;
    if (dif > 0)
        indRef = rand() % dif; // produz um indice aleatorio para indexar a sequencia maior,
                               // para a partir dele extrair a primeira versao da sequencia
                               // menor

    // gerando a sequencia menor a partir da maior. Copia trecho da sequencia
    // maior, a partir de um indice aleatorio que nao ultrapasse os limites do
    // vetor maior
    for (i = 0; i < tamSeqMenor; i++)
        seqMenor[i] = seqMaior[indRef + i];

    // causa mutacoes aleatorias na sequencia menor para gerar "gaps",
    // sobre cada base, de acordo com o grau (porcentagem) informado.
    // A mutacao causa a troca da base original por outra base aleatoria
    // necessariamente diferente. Gera-se uma probabilidade aleatoria
    // ateh 100 e se ela estiver dentro do grau informado, a mutacao
    // ocorre na base, caso contrario, mantem a base copiada.

    i = 0;
    nTrocas = 0;
    while ((i < tamSeqMenor) && (nTrocas < ((grauMuta * tamSeqMenor) / 100))) {
        probAux = rand() % 100 + 1;

        if (probAux <= grauMuta) {
            seqMenor[i] = (seqMenor[i] + (rand() % 3) + 1) % 4;
            nTrocas++;
        }
        i++;
    }

    printf("\nSequencias Geradas: Dif = %d, IndRef = %d, NTrocas = %d\n ", dif, indRef, nTrocas);
}

void mostraSequencias(void) {
    int i;

    printf("\nSequencias Atuais:\n");
    printf("\nSequencia Maior, Tam = %d\n", tamSeqMaior);
    for (i = 0; i < tamSeqMaior; i++)
        printf("%c", mapaBases[seqMaior[i]]);
    printf("\n");

    for (i = 0; i < tamSeqMaior; i++)
        if (i != indRef)
            printf(" ");
        else
            printf("^");
    printf("\nIndice de Referencia = %d\n", indRef);

    printf("\nSequencia Menor, Tam = %d\n", tamSeqMenor);
    for (i = 0; i < tamSeqMenor; i++)
        printf("%c", mapaBases[seqMenor[i]]);
    printf("\n");

    for (i = 0; i < tamSeqMenor; i++)
        if (seqMenor[i] != seqMaior[indRef + i])
            printf("^");
        else
            printf(" ");
    printf("\nQuantidade de trocas = %d\n", nTrocas);
}