#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

// Definição das bases nitrogenadas
#define A 0
#define T 1
#define G 2
#define C 3

#define sair 11
#define MAX_SEQ 100
#define MAX_THREADS 64

/* baseMapa mapeia indices em caracteres que representam as bases,
   sendo 0='A', 1='T', 2='G', 3='C' e 4='-' representando gap */
char baseMapa[5] = {'A', 'T', 'G', 'C', '-'};

/* seqMaior e seqMenor representam as duas sequencias de bases de
   entrada, a serem comparadas, inicializadas conforme segue. Elas
   conterao os indices aos inves dos proprios caracteres. seqMenor
   deve ser menor ou igual a seqMaior. */
char seqMaior[MAX_SEQ] = {A, A, C, T, T, A},
     seqMenor[MAX_SEQ] = {A, C, T, T, G, A},
     alinhaMaior[MAX_SEQ],
     alinhaMenor[MAX_SEQ];

/* matrizScores representa a matriz de scores que sera preenchida
   pelo metodo. A matriz, ao final de seu preenchimento, permitira
   obter o melhor alinhamento global entre as sequencias seqMaior e
   seqMenor, por meio de uma operacao denominada TraceBack. Uma linha
   e uma coluna extras sao adicionadas na matriz para correlacionar
   as bases com gaps. Trata-se da linha 0 e coluna 0. A matriz de scores
   tera tamSeqMenor+1 linhas e tamSeqMaior+1 colunas. Considera-se a
   primeira dimensao da matriz como linhas e a segunda como colunas.*/
int matrizScores[MAX_SEQ + 1][MAX_SEQ + 1];

int tamSeqMaior = 6, /* tamanho da sequencia maior, inicializado como 6 */
    tamSeqMenor = 6, /* tamanho da sequencia menor, inicializado como 6 */
    tamAlinha,       /* tamanho do alinhamento global obtido */
    penalGap = 0,    /* penalidade de gap, a ser descontada no score acumulado */
    grauMuta = 0,    /* porcentagem maxima de mutacao na geracao aleatoria
                        da sequencia menor, a qual eh copiada da maior e
                        sofre algumas trocas de bases */
    diagScore,       /* score da diagonal anterior da matriz de scores */
    linScore,        /* score da linha anterior da matriz de scores */
    colScore;        /* score da coluna anterior da matriz de scores */

/*  matrizPesos contem os pesos do pareamento de bases. Estruturada e
    inicializada conforme segue, onde cada linha ou coluna se refere a
    uma das bases A, T, G ou C. Considera-se a primeira dimensao da
    matriz como linhas e a segunda como colunas.

       A T G C
       0 1 2 3
   A 0 1 0 0 0
   T 1 0 1 0 0
   G 2 0 0 1 0
   C 3 0 0 0 1
*/
int matrizPesos[4][4] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

// Número de threads a serem utilizadas
int num_threads = 1;

// Estrutura para passar argumentos para as threads
typedef struct
{
    int thread_id;
} ThreadArgs;

// Protótipos de funções
void leMatrizPesos(void);
void mostraMatrizPesos(void);
int lePenalidade(void);
void definirSequencias(void);
void mostraSequencias(void);
void *preencheMatrizScoresThread(void *arg);
void geraMatrizScoresParalelo(void);
void mostraMatrizScores(void);
void gravaMatrizScores(void);
void traceBack(void);
void mostraAlinhamentoGlobal(void);
void geraSequenciasAleatorias(void);
void leSequenciasArquivo(void);
void leSequenciasTeclado(void);

/* -------------------------------------------------------- Funções -------------------------------------------------------- */

/* leitura do tamanho da sequencia maior */
int leTamMaior(void)
{
    printf("\nLeitura do Tamanho da Sequencia Maior:");
    do
    {
        printf("\nDigite 0 < valor < %d = ", MAX_SEQ);
        scanf("%d", &tamSeqMaior);
    } while ((tamSeqMaior < 1) || (tamSeqMaior > MAX_SEQ));
}

/* leitura do tamanho da sequencia menor */
int leTamMenor(void)
{
    printf("\nLeitura do Tamanho da Sequencia Menor:");
    do
    {
        printf("\nDigite 0 < valor <= %d = ", tamSeqMaior);
        scanf("%d", &tamSeqMenor);
    } while ((tamSeqMenor < 1) || (tamSeqMenor > tamSeqMaior));
}

/* leitura do valor da penalidade de gap */
int lePenalidade(void)
{
    int penal;

    printf("\nLeitura da Penalidade de Gap:");
    do
    {
        printf("\nDigite valor >= 0 = ");
        scanf("%d", &penal);
    } while (penal < 0);

    return penal;
}

/* leitura da matriz de pesos */
void leMatrizPesos()
{
    int i, j;

    printf("\nLeitura da Matriz de Pesos:\n");
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            printf("Digite valor %c x %c = ", baseMapa[i], baseMapa[j]);
            scanf("%d", &(matrizPesos[i][j]));
        }
        printf("\n");
    }
}

/* mostra da matriz de pesos */
void mostraMatrizPesos(void)
{
    int i, j;

    printf("\nMatriz de Pesos Atual:");
    printf("\n%4c%4c%4c%4c%4c\n", ' ', 'A', 'T', 'G', 'C');
    for (i = 0; i < 4; i++)
    {
        printf("%4c", baseMapa[i]);
        for (j = 0; j < 4; j++)
            printf("%4d", matrizPesos[i][j]);
        printf("\n");
    }
}

/* leitura da porcentagem maxima (grau) de mutacao aleatoria. Essa
   porcentagem eh usada na geracao aleatoria da seqMenor. A seqMenor,
   apohs extraida da seqMaior, sobre algumas trocas de bases, para
   causar alguma diferenciacao aleatoria. A quantidade de trocas
   realizadas eh no maximo a porcentagem aqui informada. */

int leGrauMutacao(void)
{
    int prob;

    printf("\nLeitura da Porcentagem Maxima de Mutacao Aleatoria:\n");
    do
    {
        printf("\nDigite 0 <= valor <= 100 = ");
        scanf("%d", &prob);
    } while ((prob < 0) || (prob > 100));

    return prob;
}

/* leitura manual das sequencias de entrada seqMaior e seqMenor */
void leSequencias(void)
{
    int i, erro;

    printf("\nLeitura das Sequencias:\n");

    /* lendo a sequencia maior */
    do
    {
        printf("\nPara a Sequencia Maior,");
        printf("\nDigite apenas caracteres 'A', 'T', 'G' e 'C'");
        do
        {
            printf("\n> ");
            fgets(seqMaior, MAX_SEQ, stdin);
            tamSeqMaior = strlen(seqMaior) - 1; /* remove o enter */
        } while (tamSeqMaior < 1);
        printf("\ntamSeqMaior = %d\n", tamSeqMaior);
        i = 0;
        erro = 0;
        do
        {
            switch (seqMaior[i])
            {
            case 'A':
                seqMaior[i] = (char)A;
                break;
            case 'T':
                seqMaior[i] = (char)T;
                break;
            case 'G':
                seqMaior[i] = (char)G;
                break;
            case 'C':
                seqMaior[i] = (char)C;
                break;
            default:
                erro = 1; /* nao eh permitido qquer outro caractere */
            }
            i++;
        } while ((erro == 0) && (i < tamSeqMaior));
    } while (erro == 1);

    /* lendo a sequencia maior */
    do
    {
        printf("\nPara a Sequencia Menor, ");
        printf("\nDigite apenas caracteres 'A', 'T', 'G' e 'C'");
        do
        {
            printf("\n> ");
            fgets(seqMenor, MAX_SEQ, stdin);
            tamSeqMenor = strlen(seqMenor) - 1; /* remove o enter */
        } while ((tamSeqMenor < 1) || (tamSeqMenor > tamSeqMaior));
        printf("\ntamSeqMenor = %d\n", tamSeqMenor);

        i = 0;
        erro = 0;
        do
        {
            switch (seqMenor[i])
            {
            case 'A':
                seqMenor[i] = (char)A;
                break;
            case 'T':
                seqMenor[i] = (char)T;
                break;
            case 'G':
                seqMenor[i] = (char)G;
                break;
            case 'C':
                seqMenor[i] = (char)C;
                break;
            default:
                erro = 1;
            }
            i++;
        } while ((erro == 0) && (i < tamSeqMenor));
    } while (erro == 1);
}

/* geracao das sequencias aleatorias, conforme tamanho. Gera-se numeros
   aleatorios de 0 a 3 representando as bases 'A', 'T', 'G' e 'C'.
   Gera-se primeiramente a maior sequencia e desta extrai a menor
   sequencia. A menor sequencia ainda sofre algumas trocas de bases ou
   mutacoes, de acordo com o grau de mutacao informado. A ideia eh
   gerar sequencias parecidas, mas com certo grau de diferenca. */

void geraSequencias(void)
{
    int i, dif, probAux, ind, nTrocas;
    char base;

    srand(time(NULL));

    printf("\nGeracao Aleatoria das Sequencias:\n");

    /* gerando a sequencia maior */
    for (i = 0; i < tamSeqMaior; i++)
    {
        base = (char)(rand() % 4); /* produz valores de 0 a 3 e os converte
                                      em char */
        seqMaior[i] = base;
    }

    dif = tamSeqMaior - tamSeqMenor; /* diferenca entre os tamanhos das sequencias */

    ind = 0;
    if (dif > 0)
        ind = rand() % dif; /* produz um indice aleatorio na sequencia maior
                             para a partir dela copiar a sequencia menor */

    /* gerando a sequencia menor a partir da maior. Copia trecho da
       sequencia maior, a partir de um indice aleatorio que nao
       ultrapasse os limites do vetor maior */
    for (i = 0; i < tamSeqMenor; i++)
        seqMenor[i] = seqMaior[ind + i];

    /* causa mutacoes aleatorias na sequencia menor para gerar "gaps",
       sobre cada base, de acordo com o grau (porcentagem) informado.
       A mutacao causa a troca da base original por outra base aleatoria
       necessariamente diferente. Gera-se uma probabilidade aleatoria
       ateh 100 e se ela estiver dentro do grau informado, a mutacao
       ocorre na base, caso contrario, mantem a base copiada. */

    i = 0;
    nTrocas = 0;
    while ((i < tamSeqMenor) && (nTrocas < grauMuta))
    {
        probAux = rand() % 100;

        if (probAux < grauMuta)
        {
            seqMenor[i] = (seqMenor[i] + (rand() % 3) + 1) % 4;
            nTrocas++;
        }
    }

    printf("\nSequencias Geradas, Dif = %d Ind = %d\n", dif, ind);
}

/* mostra das sequencias seqMaior e seqMenor */
void mostraSequencias(void)
{
    int i;

    printf("\nSequencias Atuais:\n");
    printf("\nSequencia Maior, Tam = %d\n", tamSeqMaior);
    printf("%c", baseMapa[(int)seqMaior[0]]);
    for (i = 1; i < tamSeqMaior; i++)
        printf("%c", baseMapa[(int)seqMaior[i]]);
    printf("\n");

    printf("\nSequencia Menor, Tam = %d\n", tamSeqMenor);
    printf("%c", baseMapa[(int)seqMenor[0]]);
    for (i = 1; i < tamSeqMenor; i++)
        printf("%c", baseMapa[(int)seqMenor[i]]);
    printf("\n");
}

/* geraMatrizScores gera a matriz de scores. A matriz de scores tera
   tamSeqMenor+1 linhas e tamSeqMaior+1 colunas. A linha 0 e a coluna
   0 s�o adicionadas para representar gaps e conter penalidades. As
   demais linhas e colunas s�o associadas as bases da seqMenor e da
   SeqMaior, respectivamente. */

void geraMatrizScores(void)
{
    int lin, col, peso, linMaior, colMaior, maior;

    printf("\nGeracao da Matriz de Scores:\n");
    /*  A matriz ser� gerada considerando que ela representa o cruzamento
        da seqMenor[] associada as linhas e a seqMaior[] associada as
        colunas. */

    /* inicializando a linha de penalidades/gaps */
    for (col = 0; col <= tamSeqMaior; col++)
        matrizScores[0][col] = -1 * (col * penalGap);

    /* inicializando a coluna de penalidades/gaps */
    for (lin = 0; lin <= tamSeqMenor; lin++)
        matrizScores[lin][0] = -1 * (lin * penalGap);

    /* calculando os demais scores, percorrendo todas as posicoes
       da matriz, linha por linha, coluna por coluna, aplicando
       a seguinte f�rmula:
                               / f(lin-1,col-1)+matrizPesos[lin,col]
       f(lin,col) = m�ximo de {  f(lin,col-1)-penalGap
                               \ f(lin-1,col)-penalGap
    */

    for (lin = 1; lin <= tamSeqMenor; lin++)
    {
        for (col = 1; col <= tamSeqMaior; col++)
        {
            peso = matrizPesos[(int)(seqMenor[lin - 1])][(int)(seqMaior[col - 1])];
            diagScore = matrizScores[lin - 1][col - 1] + peso;
            linScore = matrizScores[lin][col - 1] - penalGap;
            colScore = matrizScores[lin - 1][col] - penalGap;

            if ((diagScore >= linScore) && (diagScore >= colScore))
                matrizScores[lin][col] = diagScore;
            else if (linScore > colScore)
                matrizScores[lin][col] = linScore;
            else
                matrizScores[lin][col] = colScore;
        }
    }

    /* localiza o ultimo maior score, e sua posicao */

    linMaior = 1;
    colMaior = 1;
    maior = matrizScores[1][1];
    for (lin = 1; lin <= tamSeqMenor; lin++)
    {
        for (col = 1; col <= tamSeqMaior; col++)
        {
            if (maior <= matrizScores[lin][col])
            {
                linMaior = lin;
                colMaior = col;
                maior = matrizScores[lin][col];
            }
        }
    }
    printf("\nMatriz de Scores Gerada.");
    printf("\nUltimo Maior Score = %d na celula [%d,%d]", maior, linMaior, colMaior);
}

/* imprime a matriz de scores de acordo */
void mostraMatrizScores()
{
    int i, lin, col;

    printf("\nMatriz de Scores Atual:\n");

    printf("%4c%4c", ' ', ' ');
    for (i = 0; i <= tamSeqMaior; i++)
        printf("%4d", i);
    printf("\n");

    printf("%4c%4c%4c", ' ', ' ', '-');
    for (i = 0; i < tamSeqMaior; i++)
        printf("%4c", baseMapa[(int)(seqMaior[i])]);
    printf("\n");

    printf("%4c%4c", '0', '-');
    for (col = 0; col <= tamSeqMaior; col++)
        printf("%4d", matrizScores[0][col]);
    printf("\n");

    for (lin = 1; lin <= tamSeqMenor; lin++)
    {
        printf("%4d%4c", lin, baseMapa[(int)(seqMenor[lin - 1])]);
        for (col = 0; col <= tamSeqMaior; col++)
        {
            printf("%4d", matrizScores[lin][col]);
        }
        printf("\n");
    }
}

/* mostra os alinhamentos */
void mostraAlinhamentoGlobal(void)
{
    int i;

    printf("\nAlinhamentos Atuais - Tamanho = %d:\n", tamAlinha);

    printf("%c", baseMapa[(int)alinhaMaior[0]]);
    for (i = 1; i < tamAlinha; i++)
        printf("%c", baseMapa[(int)alinhaMaior[i]]);
    printf("\n");

    printf("%c", baseMapa[(int)alinhaMenor[0]]);
    for (i = 1; i < tamAlinha; i++)
        printf("%c", baseMapa[(int)alinhaMenor[i]]);
    printf("\n");
}

/* gera o alinhamento global por meio do retorno na Matriz de Scores
   da celula final (tamSeqMenor,tamSeqMaior) ateh a celula inicial (0,0).
   A partir da celula final, retorna-se para a celula de onde o score
   atual foi derivado. Repete-se esse processo a partir desta celula
   retornada e assim sucessivamente, ateh alcancar a celula inicial.
   O alinhamento global � composto por duas sequencias alinhaMenor e
   alinhaMaior. */
void traceBack()
{
    int tbLin, tbCol, peso, pos, posAux, aux, i;

    printf("\nGeracao do Alinhamento Global:\n");
    tbLin = tamSeqMenor;
    tbCol = tamSeqMaior;
    pos = 0;
    do
    {
        peso = matrizPesos[(int)(seqMenor[tbLin - 1])][(int)(seqMaior[tbCol - 1])];
        diagScore = matrizScores[tbLin - 1][tbCol - 1] + peso;
        linScore = matrizScores[tbLin][tbCol - 1] - penalGap;
        colScore = matrizScores[tbLin - 1][tbCol] - penalGap;

        if ((diagScore >= linScore) && (diagScore >= colScore))
        {
            alinhaMenor[pos] = seqMenor[tbLin - 1];
            alinhaMaior[pos] = seqMaior[tbCol - 1];
            tbLin--;
            tbCol--;
            pos++;
        }
        else if (linScore > colScore)
        {
            alinhaMenor[pos] = (char)4;
            alinhaMaior[pos] = seqMaior[tbCol - 1];
            tbCol--;
            pos++;
        }
        else
        {
            alinhaMenor[pos] = seqMenor[tbLin - 1];
            alinhaMaior[pos] = (char)4;
            tbLin--;
            pos++;
        }

    } while ((tbLin != 0) && (tbCol != 0));

    /* descarrega o restante de gaps da linha 0, se for o caso */
    while (tbLin > 0)
    {
        alinhaMenor[pos] = seqMenor[tbLin - 1];
        alinhaMaior[pos] = (char)4;
        tbLin--;
        pos++;
    }

    /* descarrega o restante de gaps da coluna 0, se for o caso */
    while (tbCol > 0)
    {
        alinhaMenor[pos] = (char)4;
        alinhaMaior[pos] = seqMaior[tbCol - 1];
        tbCol--;
        pos++;
    }

    tamAlinha = pos;

    /* o alinhamento foi feito de tras para frente e deve ser
       invertido, conforme segue */
    for (i = 0; i < (tamAlinha / 2); i++)
    {
        aux = alinhaMenor[i];
        alinhaMenor[i] = alinhaMenor[tamAlinha - i - 1];
        alinhaMenor[tamAlinha - i - 1] = aux;

        aux = alinhaMaior[i];
        alinhaMaior[i] = alinhaMaior[tamAlinha - i - 1];
        alinhaMaior[tamAlinha - i - 1] = aux;
    }

    printf("\nAlinhamento Global Gerado.");
}

/**
 * Função executada por cada thread para preencher a matriz de scores.
 *
 * Esta função implementa o algoritmo de programação dinâmica para preencher
 * a matriz de scores. Cada thread é responsável por um subconjunto de linhas
 * da matriz, determinado pelo seu ID.
 *
 * @param arg Ponteiro para a estrutura ThreadArgs contendo o ID da thread
 * @return NULL
 */
void *preencheMatrizScoresThread(void *arg)
{
    ThreadArgs *args = (ThreadArgs *)arg;
    int thread_id = args->thread_id;

    for (int lin = thread_id + 1; lin <= tamSeqMenor; lin += num_threads)
    {
        for (int col = 1; col <= tamSeqMaior; col++)
        {
            int peso = matrizPesos[(int)(seqMenor[lin - 1])][(int)(seqMaior[col - 1])];
            int diagScore = matrizScores[lin - 1][col - 1] + peso;
            int linScore = matrizScores[lin][col - 1] - penalGap;
            int colScore = matrizScores[lin - 1][col] - penalGap;

            // Escolhe o maior score entre as três opções
            if (diagScore >= linScore && diagScore >= colScore)
            {
                matrizScores[lin][col] = diagScore;
            }
            else if (linScore > colScore)
            {
                matrizScores[lin][col] = linScore;
            }
            else
            {
                matrizScores[lin][col] = colScore;
            }
        }
    }

    return NULL;
}

/**
 * Gera a matriz de scores de forma paralela usando threads.
 *
 * Esta função inicializa a primeira linha e coluna da matriz com penalidades de gap,
 * cria as threads para preencher o resto da matriz e espera que todas terminem.
 *
 * @return void
 */
void geraMatrizScoresParalelo()
{
    pthread_t threads[MAX_THREADS];
    ThreadArgs thread_args[MAX_THREADS];

    // Inicializar a primeira linha e coluna com penalidades de gap
    for (int col = 0; col <= tamSeqMaior; col++)
    {
        matrizScores[0][col] = -1 * (col * penalGap);
    }
    for (int lin = 0; lin <= tamSeqMenor; lin++)
    {
        matrizScores[lin][0] = -1 * (lin * penalGap);
    }

    // Criar e iniciar as threads
    for (int i = 0; i < num_threads; i++)
    {
        thread_args[i].thread_id = i;
        if (pthread_create(&threads[i], NULL, preencheMatrizScoresThread, &thread_args[i]) != 0)
        {
            fprintf(stderr, "Erro ao criar thread %d\n", i);
            exit(1);
        }
    }

    // Esperar todas as threads terminarem
    for (int i = 0; i < num_threads; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            fprintf(stderr, "Erro ao juntar thread %d\n", i);
            exit(1);
        }
    }
}

// Função para exibir o menu de opções
void menu()
{
    // Exibir menu de opções
    printf("\n=======================================================");
    printf("\n|                   Menu de Opções                    |\n");
    printf("=======================================================\n");
    printf("|             1. Definir Sequências                   |\n");
    printf("|             2. Mostrar Sequências                   |\n");
    printf("|             3. Ler Matriz de Pesos                  |\n");
    printf("|             4. Mostrar Matriz de Pesos              |\n");
    printf("|             5. Ler Penalidade de Gap                |\n");
    printf("|             6. Gerar Matriz de Scores (Paralelo)    |\n");
    printf("|             7. Mostrar Matriz de Scores             |\n");
    printf("|             8. Gravar Matriz de Scores em Arquivo   |\n");
    printf("|             9. Gerar Alinhamento Global             |\n");
    printf("|             10. Mostrar Alinhamento Global          |\n");
    printf("|             11. Sair                                |\n");
    printf("=======================================================\n\n");
}

/**
 * Função para exibir o menu de opções e solicitar a escolha do usuário.
 * @return Opção escolhida pelo usuário
 */
int menuOpcao()
{
    // Variável para armazenar a opção escolhida pelo usuário
    int opcao;

    // Exibir menu de opções
    menu();

    // Solicitar opção ao usuário
    printf("Escolha uma opção: ");

    // Ler a opção fornecida pelo usuário
    if (scanf("%d", &opcao) != 1)
    {
        // Limpando o buffer de entrada para caso de erro
        while (getchar() != '\n')
            ;
        return -1;
    }

    return opcao;
}

/**
 *  Função para tratar a opção escolhida pelo usuário.
 * @param op Opção escolhida pelo usuário.
 * @return void
 *  */
void trataOpcao(int op)
{
    int resp;
    char enter;

    switch (op)
    {
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
        printf("\nDeseja Definicao: <1>MANUAL ou <2>ALEATORIA? = ");
        scanf("%d", &resp);
        scanf("%c", &enter); /* remove o enter */
        if (resp == 1)
        {
            leSequencias();
        }
        else
        {
            leTamMaior();
            leTamMenor();
            grauMuta = leGrauMutacao();
            geraSequencias();
        }
        break;
    case 6:
        mostraSequencias();
        break;
    case 7:
        geraMatrizScores();
        break;
    case 8:
        mostraMatrizScores();
        break;
    case 9:
        traceBack();
        break;
    case 10:
        mostraAlinhamentoGlobal();
        break;
    }
}

/**
 * Função para gravar a matriz de scores em um arquivo.
 * @return void
 */
void gravaMatrizScores()
{
    char nomeArquivo[100];

    // Solicitar nome do arquivo ao usuário
    printf("Digite o nome do arquivo para salvar a matriz de scores: ");
    scanf("%s", nomeArquivo);

    // Abrir arquivo para escrita
    FILE *arquivo = fopen(nomeArquivo, "w");

    // Verificar se o arquivo foi aberto corretamente
    if (arquivo == NULL)
    {
        printf("Erro ao criar o arquivo.\n");
        return; // Encerrar a função
    }

    // Escrever cabeçalho
    fprintf(arquivo, "   ");

    // Escrever bases da sequência menor
    for (int i = 0; i < tamSeqMaior; i++)
    {
        fprintf(arquivo, "%4c", baseMapa[seqMaior[i]]); // Escrever base da sequência menors
    }

    fprintf(arquivo, "\n"); // Quebra de linha

    // Escrever primeira linha da matriz de scores
    for (int i = 0; i <= tamSeqMenor; i++)
    {
        // Escrever base da sequência maior
        if (i > 0)
        {
            fprintf(arquivo, "%c ", baseMapa[seqMenor[i - 1]]); // Escrever base da sequência maior
        }
        else // Espaço para a linha de penalidades
        {
            fprintf(arquivo, "  "); // Espaço em branco
        }

        // Percorrer a linha da matriz de scores
        for (int j = 0; j <= tamSeqMaior; j++)
        {
            fprintf(arquivo, "%4d", matrizScores[i][j]); // Escrever score
        }

        fprintf(arquivo, "\n"); // Quebra de linha
    }

    fclose(arquivo);                                                              // Fechar arquivo
    printf("Matriz de scores gravada com sucesso no arquivo %s.\n", nomeArquivo); // Mensagem de sucesso
}

// Função para gerar sequências aleatórias com similaridade
void geraSequenciasAleatorias()
{
    srand(time(NULL));

    printf("Digite o tamanho da sequência maior: ");
    scanf("%d", &tamSeqMaior);
    if (tamSeqMaior > MAX_SEQ)
        tamSeqMaior = MAX_SEQ;

    printf("Digite o tamanho da sequência menor: ");
    scanf("%d", &tamSeqMenor);
    if (tamSeqMenor > tamSeqMaior)
        tamSeqMenor = tamSeqMaior;

    printf("Digite a porcentagem de similaridade (0-100): ");
    int similaridade;
    scanf("%d", &similaridade);
    if (similaridade < 0)
        similaridade = 0;
    if (similaridade > 100)
        similaridade = 100;

    // Gerar sequência maior
    for (int i = 0; i < tamSeqMaior; i++)
    {
        seqMaior[i] = rand() % 4; // 0=A, 1=T, 2=G, 3=C
    }

    // Gerar sequência menor baseada na maior
    int startPos = rand() % (tamSeqMaior - tamSeqMenor + 1);
    for (int i = 0; i < tamSeqMenor; i++)
    {
        if (rand() % 100 < similaridade)
        {
            seqMenor[i] = seqMaior[startPos + i];
        }
        else
        {
            seqMenor[i] = rand() % 4;
        }
    }

    printf("Sequências geradas com sucesso.\n");
}

/**
 * Lê sequências de um arquivo.
 *
 * Esta função solicita o nome do arquivo ao usuário e lê as sequências maior e menor
 * do arquivo especificado. As sequências devem estar em linhas separadas e conter
 * apenas os caracteres A, T, G e C.
 */
void leSequenciasArquivo()
{
    char nomeArquivo[100];
    printf("Digite o nome do arquivo: ");
    scanf("%s", nomeArquivo);

    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[MAX_SEQ + 1];

    // Ler sequência maior
    if (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        tamSeqMaior = strlen(linha) - 1; // -1 para remover o \n
        for (int i = 0; i < tamSeqMaior; i++)
        {
            switch (linha[i])
            {
            case 'A':
                seqMaior[i] = 0;
                break;
            case 'T':
                seqMaior[i] = 1;
                break;
            case 'G':
                seqMaior[i] = 2;
                break;
            case 'C':
                seqMaior[i] = 3;
                break;
            default:
                printf("Caractere inválido na sequência maior.\n");
                fclose(arquivo);
                return;
            }
        }
    }
    else
    {
        printf("Erro ao ler a sequência maior.\n");
        fclose(arquivo);
        return;
    }

    // Ler sequência menor
    if (fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        tamSeqMenor = strlen(linha) - 1; // -1 para remover o \n
        for (int i = 0; i < tamSeqMenor; i++)
        {
            switch (linha[i])
            {
            case 'A':
                seqMenor[i] = 0;
                break;
            case 'T':
                seqMenor[i] = 1;
                break;
            case 'G':
                seqMenor[i] = 2;
                break;
            case 'C':
                seqMenor[i] = 3;
                break;
            default:
                printf("Caractere inválido na sequência menor.\n");
                fclose(arquivo);
                return;
            }
        }
    }
    else
    {
        printf("Erro ao ler a sequência menor.\n");
        fclose(arquivo);
        return;
    }

    fclose(arquivo);
    printf("Sequências lidas com sucesso do arquivo.\n");
}

/** Função para ler as sequências de DNA do teclado.
 *
 * Esta função solicita ao usuário que digite as sequências de DNA
 * maior e menor no teclado. As sequências devem conter apenas os
 * caracteres A, T, G e C.
 *
 * A função converte os caracteres lidos para os valores numéricos
 * correspondentes (0=A, 1=T, 2=G, 3=C) e armazena nas variáveis
 * globais seqMaior e seqMenor.
 *
 * A função também verifica se as sequências lidas são válidas e
 * exibe uma mensagem de erro se um caractere inválido for digitado.
 *
 * @return void
 */
void leSequenciasTeclado()
{
    char linha[MAX_SEQ + 1];

    printf("Digite a sequência maior (apenas A, T, G, C): ");
    scanf("%s", linha);
    tamSeqMaior = strlen(linha);
    if (tamSeqMaior > MAX_SEQ)
        tamSeqMaior = MAX_SEQ;
    for (int i = 0; i < tamSeqMaior; i++)
    {
        switch (linha[i])
        {
        case 'A':
            seqMaior[i] = 0;
            break;
        case 'T':
            seqMaior[i] = 1;
            break;
        case 'G':
            seqMaior[i] = 2;
            break;
        case 'C':
            seqMaior[i] = 3;
            break;
        default:
            printf("Caractere inválido na sequência maior.\n");
            return;
        }
    }

    printf("Digite a sequência menor (apenas A, T, G, C): ");
    scanf("%s", linha);
    tamSeqMenor = strlen(linha);
    if (tamSeqMenor > tamSeqMaior)
        tamSeqMenor = tamSeqMaior;
    for (int i = 0; i < tamSeqMenor; i++)
    {
        switch (linha[i])
        {
        case 'A':
            seqMenor[i] = 0;
            break;
        case 'T':
            seqMenor[i] = 1;
            break;
        case 'G':
            seqMenor[i] = 2;
            break;
        case 'C':
            seqMenor[i] = 3;
            break;
        default:
            printf("Caractere inválido na sequência menor.\n");
            return;
        }
    }

    printf("Sequências lidas com sucesso do teclado.\n");
}

/**
 * Função para mostrar as sequências de DNA.
 * Esta função exibe as sequências de DNA lidas ou geradas.
 * @return void
 */
void definirSequencias()
{
    int metodo;
    printf("Escolha o método de entrada das sequências:\n");
    printf("1. Aleatório\n2. Arquivo\n3. Teclado\n");
    printf("Opção: ");
    scanf("%d", &metodo);

    switch (metodo)
    {
    case 1:
        geraSequenciasAleatorias();
        break;
    case 2:
        leSequenciasArquivo();
        break;
    case 3:
        leSequenciasTeclado();
        break;
    default:
        printf("Opção inválida.\n");
    }
}

/**
 * Função principal do programa.
 */
int main(void)
{
    int opcao;

    while (1)
    {
        opcao = menuOpcao();
        switch (opcao)
        {
        case 1:
            definirSequencias();
            break;
        case 2:
            mostraSequencias();
            break;
        case 3:
            leMatrizPesos();
            break;
        case 4:
            mostraMatrizPesos();
            break;
        case 5:
            penalGap = lePenalidade();
            break;
        case 6:
            printf("Digite o número de threads (1-%d): ", MAX_THREADS);
            if (scanf("%d", &num_threads) != 1 || num_threads < 1 || num_threads > MAX_THREADS)
            {
                printf("Número de threads inválido. Usando 1 thread.\n");
                num_threads = 1;
            }
            geraMatrizScoresParalelo();
            break;
        case 7:
            mostraMatrizScores();
            break;
        case 8:
            gravaMatrizScores();
            break;
        case 9:
            traceBack();
            break;
        case 10:
            mostraAlinhamentoGlobal();
            break;
        case 11:
            printf("Encerrando o programa.\n");
            return 0;
        default:
            printf("Opção inválida.\n");
        }
    }
}