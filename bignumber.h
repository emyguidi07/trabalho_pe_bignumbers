/*
Authors: Emily Guidi 11202320595
         Paloma 
2024-12-06 - Emily - Implementation sum function and create files 
*/
#ifndef BIGNUMBER_H
#define BIGNUMBER_H

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
BigNumber *subtractBigNumbers(BigNumber *a, BigNumber *b);
BigNumber *multiplyBigNumbers(BigNumber *a, BigNumber *b);
void printBigNumber(BigNumber *bn);
BigNumber *parseBigNumberInput();
BigNumber *divideBigNumbers(BigNumber *a, BigNumber *b); 

#endif

