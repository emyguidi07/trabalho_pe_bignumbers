# trabalho_pe_bignumbers
### Trabalho de bignumbers em C
Alunas: Paloma Valéria Campos de Lima - RA 110202021803, Emily Guidi Cavalcante - RA 11202320595

---
## Estrutura e Organização
O código apresentado implementa uma classe para manipular números inteiros arbitrariamente grandes, denominados BigNumbers. A estrutura principal é uma lista duplamente encadeada, onde cada nó representa um dígito do número. A nossa estrutura de scripts foi a minima de bignumber.h , bignumber.c , makefile e cliente.c.

---
## Funcionalidades Implementadas

Criação e destruição: As funções createBigNumber e freeBigNumber permitem alocar e liberar memória para os objetos BigNumber.

Operações aritméticas básicas:

Adição: Implementada de forma eficiente, considerando o carry.

Subtração: Implementada de forma similar à adição, com o cuidado de lidar com empréstimos.

Multiplicação: Otimização da multiplicação de números grandes.

Divisão: Implementada usando um algoritmo de divisão longa.

Exponenciação: Utiliza o método de exponenciação por quadrados para otimizar o cálculo de potências.

Módulo: Calcula o resto da divisão.

Comparação: A função compareBigNumbers compara a magnitude de dois números.

Conversão: Converte entre strings e BigNumbers.

Cópia: Copia um número BigNumber integralmente.

Gerenciamento de sinal: A classe BigNumber suporta números negativos.

Leitura de entrada dinâmica: A função readinput permite ler números de tamanho arbitrário da entrada padrão.

Alocação dinâmica de memória: O código utiliza malloc e free para alocar e liberar memória conforme necessário.

Algoritmos eficientes: Exponenciação recursiva por quadrados otimiza o desempenho para números grandes.

---

## Interface Pública


A interface pública da classe BigNumber é composta pelas funções descritas anteriormente. Essas funções permitem criar, manipular e destruir objetos BigNumber e estão listadas no arquivo bignumber.h.
Lista de funções na interface pública:
BigNumber *createBigNumber(const char *str);
void freeBigNumber(BigNumber *bn);
BigNumber *addPositiveBigNumbers(BigNumber *a, BigNumber *b);
BigNumber *addBigNumbers(BigNumber *a, BigNumber *b);
BigNumber *subtractPositiveBigNumbers(BigNumber *a, BigNumber *b) ;
BigNumber *subtractBigNumbers(BigNumber *a, BigNumber *b);
BigNumber *createBigNumberFromBigNumber(BigNumber *src);
int compareBigNumbers(BigNumber *a, BigNumber *b);
void printBigNumber(BigNumber *bn);
BigNumber *divideBigNumbers(BigNumber *a, BigNumber *b);
int getLength(BigNumber *a);
char *readinput();
BigNumber *multiplyBigNumbers(BigNumber *a, BigNumber *b);
BigNumber *stringToBigNumber(const char *str);
BigNumber *exponenciacao(BigNumber *base, BigNumber *exponent); 
BigNumber *restoDivisao(BigNumber *a, BigNumber *b);

___

## Algoritmos e Estruturas de Dados Avançadas

Exponenciação por quadrados: Um algoritmo eficiente em recursão para calcular potências.

Lista duplamente encadeada: A estrutura de dados principal, escolhida por sua flexibilidade para inserir e remover elementos em qualquer posição.

---

## Divisão de tarefas do trabalho 

Emily:
- Criação da estrutura de arquivos;
- Função Soma;
- Função Subtração;
- Melhoria nos dados de entrada do programa (modelo passado em sala de aula para captar dígito por dígito do Bignumber);
- Bateria de testes das três funcionalidades;
- Ajustes em divisão de bignumbers e exponenciação;

Paloma: 
- Elaborar função Divisão (divisão inteira);
- Elaborar função de exponenciação;
- Elaborar função de resto de divisão;
- Testar essas três funções manualmente e com os arquivos dele de teste;
- Ajustes em divisão de bignumbers e exponenciação;
- Escrever o READ ME.
