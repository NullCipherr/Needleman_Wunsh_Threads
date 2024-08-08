#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "constants.h"
#include "globals.h"

// Protótipos das funções
int menuOpcao(void);
void trataOpcao(int op);
void program_title_hud();

int main(void) {
    int opcao;
    printf("\nDigite o numero de threads: ");
    scanf("%d", &num_threads);

    srand(time(NULL));

    do {
        program_title_hud();
        opcao = menuOpcao();
        trataOpcao(opcao);
    } while (opcao != sair);

    return 0;
}

void program_title_hud() {
    printf("\n====================================================");
    printf("\n||      ***** Needleman-Wunsch Sequencial *****   ||");
    printf("\n====================================================");
}