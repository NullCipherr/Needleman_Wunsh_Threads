#include <stdio.h>
#include "constants.h"
#include "globals.h"

// Protótipos das funções
void leMatrizPesos();
void mostraMatrizPesos(void);
int lePenalidade(void);
void leSequencias(void);
void leSequenciasDeArquivo(void);
void geraSequencias(void);
void mostraSequencias(void);
void geraMatrizEscores(void);
void mostraMatrizEscores();
void gravaMatrizEscores();
void traceBack(int tipo);
void mostraAlinhamentoGlobal(void);
int leTamMaior(void);
int leTamMenor(void);
int leGrauMutacao(void);

int menuOpcao(void) {
    int op;
    char enter;
    do {
        // Exibe o menu de opções
        printf("\n====================================================");
        printf("\n||      <01> Ler Matriz de Pesos                  ||");
        printf("\n||      <02> Mostrar Matriz de Pesos              ||");
        printf("\n||      <03> Ler Penalidade de Gap                ||");
        printf("\n||      <04> Mostrar Penalidade                   ||");
        printf("\n||      <05> Definir Sequencias Genomicas         ||");
        printf("\n||      <06> Mostrar Sequencias                   ||");
        printf("\n||      <07> Gerar Matriz de Escores              ||");
        printf("\n||      <08> Mostrar Matriz de Escores            ||");
        printf("\n||      <09> Gravar Matriz de Escores em Arquivo  ||"); // Nova opção adicionada
        printf("\n||      <10> Gerar Alinhamento Global             ||");
        printf("\n||      <11> Mostrar Alinhamento Global           ||");
        printf("\n||      <12> Sair                                 ||");
        printf("\n====================================================");
        printf("\n");

        // Exibe informações sobre o número de threads, penalidade de gap e grau de mutação
        printf("Threads = %d\n", num_threads);
        printf("Penalidade de Gap = %d\n", penalGap);
        printf("Grau de Mutacao = %d\n", grauMuta);

        printf("\n");

        // Solicita ao usuário que selecione uma opção
        printf("\nSelecione => ");
        scanf("%d", &op);
        scanf("%c", &enter);
    } while ((op < 1) || (op > sair)); // Continua o loop até que uma opção válida seja selecionada
    return (op);
}

void trataOpcao(int op) {
    int resp;
    char enter;
    switch (op) {
    case 1:
        leMatrizPesos();
        break;
    case 2:
        mostraMatrizPesos();
        break;
    case 3:
        penalGap = lePenalidade();
        break;
    case 4:
        printf("\nPenalidade = %d", penalGap);
        break;
    case 5:
        // Solicita ao usuário o método de definição das sequências
        printf("\nDeseja Definicao: <1>MANUAL, <2>ALEATORIA, ou <3>ARQUIVO? = ");
        scanf("%d", &resp);
        scanf("%c", &enter);
        if (resp == 1) {
            leSequencias();
        } else if (resp == 2) {
            leTamMaior();
            leTamMenor();
            grauMuta = leGrauMutacao();
            geraSequencias();
        } else if (resp == 3) {
            leSequenciasDeArquivo();
        }
        break;
    case 6:
        mostraSequencias();
        break;
    case 7:
        geraMatrizEscores();
        break;
    case 8:
        mostraMatrizEscores();
        break;
    case 9: // Opção para gravar a matriz de escores
        gravaMatrizEscores();
        break;
    case 10:
        // Solicita ao usuário o tipo de alinhamento global
        printf("\nDeseja: <1> Primeiro Maior ou <2> Ultimo Maior? = ");
        scanf("%d", &resp);
        scanf("%c", &enter);
        traceBack(resp);
        break;
    case 11:
        mostraAlinhamentoGlobal();
        break;
    }
}