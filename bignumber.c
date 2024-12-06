/*
Authors: Emily Guidi 11202320595
         Paloma 
2024-12-06 - Emily - Implementation sum function and create files 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bignumber.h"

BigNumber *createBigNumber(const char *str) {
    BigNumber *bn = (BigNumber *)malloc(sizeof(BigNumber));
    bn->head = bn->tail = NULL;
    bn->isNegative = (*str == '-');
    if (*str == '+' || *str == '-') str++;

    for (const char *p = str; *p != '\0'; ++p) {
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->digit = *p - '0';
        newNode->prev = bn->tail;
        newNode->next = NULL;
        if (bn->tail) {
            bn->tail->next = newNode;
        } else {
            bn->head = newNode;
        }
        bn->tail = newNode;
    }
    return bn;
}

void freeBigNumber(BigNumber *bn) {
    Node *current = bn->head;
    while (current) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    free(bn);
}

BigNumber *addPositiveBigNumbers(BigNumber *a, BigNumber *b) {
    BigNumber *result = (BigNumber *)malloc(sizeof(BigNumber));
    result->head = result->tail = NULL;
    result->isNegative = 0;

    Node *nodeA = a->tail;
    Node *nodeB = b->tail;
    int carry = 0;

    while (nodeA || nodeB || carry) {
        int digitA = nodeA ? nodeA->digit : 0;
        int digitB = nodeB ? nodeB->digit : 0;
        int sum = digitA + digitB + carry;

        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->digit = sum % 10;
        newNode->prev = NULL;
        newNode->next = result->head;

        if (result->head) {
            result->head->prev = newNode;
        } else {
            result->tail = newNode;
        }
        result->head = newNode;

        carry = sum / 10;

        if (nodeA) nodeA = nodeA->prev;
        if (nodeB) nodeB = nodeB->prev;
    }

    return result;
}

BigNumber *addBigNumbers(BigNumber *a, BigNumber *b) {
    if (a->isNegative == b->isNegative) {
        BigNumber *result = addPositiveBigNumbers(a, b);
        result->isNegative = a->isNegative;
        return result;
    }
    BigNumber *result = subtractBigNumbers(a, b);
    return result;
}

BigNumber *subtractBigNumbers(BigNumber *a, BigNumber *b) {
    // Aqui será implementada a subtração
    return createBigNumber("0"); // Placeholder
}

BigNumber *multiplyBigNumbers(BigNumber *a, BigNumber *b) {
    // Aqui será implementada a multiplicação
    return createBigNumber("0"); // Placeholder
}

BigNumber *divideBigNumbers(BigNumber *a, BigNumber *b) {
    // Aqui será implementada a divisão
    return createBigNumber("0"); // Placeholder
}

void printBigNumber(BigNumber *bn) {
    if (bn->isNegative) printf("-");
    for (Node *current = bn->head; current != NULL; current = current->next) {
        printf("%c", current->digit + '0');
    }
    printf("\n");
}

BigNumber *parseBigNumberInput() {
    char buffer[10000];
    if (scanf("%s", buffer) != 1) return NULL;
    return createBigNumber(buffer);
}
