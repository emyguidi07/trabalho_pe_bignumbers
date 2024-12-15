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
BigNumber *parseBigNumberInput();
BigNumber *divideBigNumbers(BigNumber *a, BigNumber *b);
BigNumber *shiftBigNumber(BigNumber *a, int positions);
int getLength(BigNumber *a);
BigNumber *karatsuba(BigNumber *a, BigNumber *b);
BigNumber *karatsubaMultiply(BigNumber *a, BigNumber *b); 
bool isSingleDigit(BigNumber *bn);                        
void splitBigNumber(BigNumber *bn, int position, BigNumber **high, BigNumber **low); 
void appendNode(BigNumber *bn, Node *newNode);            

#endif
