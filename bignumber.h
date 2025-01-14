#ifndef BIGNUMBER_H
#define BIGNUMBER_H

#include <stdbool.h> 

typedef struct Node {
    char digit;
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct {
    Node *head;
    Node *tail;
    int isNegative;
} BigNumber;

BigNumber *createBigNumber(const char *str);
void freeBigNumber(BigNumber *bn);
BigNumber *addPositiveBigNumbers(BigNumber *a, BigNumber *b);
BigNumber *addBigNumbers(BigNumber *a, BigNumber *b);
BigNumber *subtractPositiveBigNumbers(BigNumber *a, BigNumber *b) ;
BigNumber *subtractBigNumbers(BigNumber *a, BigNumber *b);
int compareBigNumbers(BigNumber *a, BigNumber *b);
void printBigNumber(BigNumber *bn);
BigNumber *divideBigNumbers(BigNumber *a, BigNumber *b);
int getLength(BigNumber *a);
char *readinput();
BigNumber *multiplyBigNumbers(BigNumber *a, BigNumber *b);
BigNumber *stringToBigNumber(const char *str);
BigNumber *exponenciacao(BigNumber *base, BigNumber *exponent); 
BigNumber *restoDivisao(BigNumber *a, BigNumber *b);

#endif
