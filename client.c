/*
Authors: Emily Guidi 11202320595
         Paloma 
2024-12-06 - Emily - Implementation sum function and create files  
2024-12-15 - Emily - Implementation subtract function and karatsuba Multiply algorithm
*/
#include <stdio.h>
#include <stdlib.h>
#include "bignumber.h"
int main() {
    while (!feof(stdin)) {
        // read bignumber and operation
        BigNumber *a = parseBigNumberInput();
        BigNumber *b = parseBigNumberInput();
        char operation;
        if (scanf(" %c", &operation) != 1) break;

        BigNumber *result = NULL;
        switch (operation) {
            case '+':
                result = addBigNumbers(a, b);
                break;
            case '-':
                result = subtractBigNumbers(a, b);
                break;
            case '*':
                result = karatsubaMultiply(a, b);
                break;
            case '/':
                result = divideBigNumbers(a, b);
                break;
            default:
                fprintf(stderr, "Operação inválida!\n");
                break;
        }

        // print result
        if (result) {
            printBigNumber(result);
            freeBigNumber(result);
        }

        // free memory bignumbers
        freeBigNumber(a);
        freeBigNumber(b);
    }
    return 0;
}