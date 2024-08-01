# 🧬 Needleman-Wunsch Threads

## 📜 Descrição
Este repositório contém a implementação do algoritmo de Needleman-Wunsch para alinhamento global de sequências, utilizando multithreading para melhorar a eficiência e o desempenho. Nosso objetivo é fornecer uma ferramenta educacional e prática para estudantes, pesquisadores e desenvolvedores interessados em bioinformática e programação concorrente.

## 📜 Especificação 

### Enunciado do Problema
Desenvolver uma versão multi-thread da aplicação de reconhecimento de genoma fornecida pelo professor.

### Diretrizes
- Permitir que o usuário defina os tamanhos das sequências a serem comparadas, que poderão ser da ordem de milhares de bases.
- Permitir que as sequências de bases possam ser geradas aleatoriamente, fornecidas em arquivos ou lidas a partir do teclado, todas as 3 opções. Na geração aleatória, gera-se a primeira sequência aleatória e com base nesta, cria a segunda sequência. A segunda sequência deve ser retirada da primeira e deve sofrer um percentual de alterações, para ficarem com certo grau de similaridade. O percentual de alterações deve ser fornecido pelo usuário. No fornecimento por arquivo ou na leitura pelo teclado, as duas sequências devem ser fornecidas pelo usuário.
- As sequências devem ser compostas por caracteres que representam as bases (A, T, G e C).
- A matriz de pesos deve ser fornecida pelo usuário via digitação.
- Deve-se usar a mesma Função de Pontuação do material passado em aula, exceto pelo valor da penalidade do gap (d), que deve ser fornecido pelo usuário.
- O programa deve mostrar o maior score para o alinhamento global. Além disso, o programa deverá mostrar até k possibilidades de alinhamento global e as sequências alinhadas pareadas uma a outra. k deverá ser fornecido pelo usuário.
- A matriz de scores deve ser gravada em arquivo texto, de forma tabulada e bem autoexplicativa. As duas sequências de entrada devem aparecer nas linhas e colunas da matriz de scores de forma a permitir analisar e conferir a corretude da matriz.
- Deve-se entregar a solução multi-threaded, além de um relatório técnico.
- O relatório técnico deve informar os dados dos autores e explicar o funcionamento dos códigos, as restrições e limitações, os erros, as partes que não funcionam e como os códigos devem ser usados, com exemplos de uso.
- Na solução multi-threaded, o usuário deve definir o número de threads que deverá ser usado.
- O paralelismo deve ser realizado em duas etapas da aplicação:
  1. A construção/preenchimento da matriz de scores deve ser paralelizada entre k threads, de forma mais igualitária possível. Tal distribuição deverá ser feita por linhas de forma que cada thread receba um conjunto de linhas para gerar. Cada thread deve trabalhar com um conjunto de linhas equidistantes, não consecutivas, para promover a paralelização. Se necessário, deve ser utilizado mecanismos de sincronização.
  2. A reconstrução das sequências alinhadas deve ser paralelizada em até k threads. Tal paralelização deve ser feita quando houver mais que uma possibilidade de traceback, ainda que com scores diferentes, com isso, o programa deverá mostrar até k pares de alinhamentos.

Atenção: a paralelização deve ocorrer no preenchimento da matriz de scores e na reconstrução dos até k pares de alinhamentos possíveis. A versão paralela deve possuir as mesmas funcionalidades e características da versão sequencial.
