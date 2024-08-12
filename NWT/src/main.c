#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

// Inclui o arquivo constants.h, que contém definições de constantes
#include "constants.h"
// Inclui o arquivo globals.h, que contém declarações de variáveis globais
#include "globals.h"

// Declaração das funções
int menuOpcao(void);
void trataOpcao(int op);
void program_title_hud();

int main(void) {
    int opcao;

    // Solicita ao usuário o número de threads
    printf("\nDigite o numero de threads: ");
    scanf("%d", &num_threads);

    // Inicializa a semente do gerador de números aleatórios com o tempo atual
    srand(time(NULL));

    // Loop principal do programa
    do {
        // Exibe o título do programa
        program_title_hud();
        // Obtém a opção do menu selecionada pelo usuário
        opcao = menuOpcao();
        // Trata a opção selecionada
        trataOpcao(opcao);
    } while (opcao != sair); // Continua o loop até que a opção de sair seja selecionada

    return 0;
}

// Função que exibe o título do programa
void program_title_hud() {
    printf("\n====================================================");
    printf("\n||      ***** Needleman-Wunsch Sequencial *****   ||");
    printf("\n====================================================");
}