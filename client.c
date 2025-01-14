/*
Authors: Emily Guidi 11202320595
         Paloma Lima 11202021803
*/
#include <stdio.h>
#include <stdlib.h>
#include "bignumber.h"
#include <string.h>

int main() {
    char *input1 = NULL;
    char *input2 = NULL;
    char *operador = NULL;

    while (1) {
        input1 = readinput();
        if (input1 == NULL || strlen(input1) == 0) {
            free(input1);
            break;
        }
        input2 = readinput();
        if (input2 == NULL || strlen(input2) == 0) {
            free(input1);
            free(input2);
            break;
        }
        operador = readinput();
        if (operador == NULL || strlen(operador) != 1) {
            free(input1);
            free(input2);
            free(operador);
            break;
        }
        BigNumber *bignumber1 = stringToBigNumber(input1);
        BigNumber *bignumber2 = stringToBigNumber(input2);

        if (bignumber1 == NULL || bignumber2 == NULL) {
            free(input1);
            free(input2);
            free(operador);
            freeBigNumber(bignumber1);
            freeBigNumber(bignumber2);
            break;
        }
        BigNumber *result = NULL;
        switch (operador[0]) {
            case '+':
                result = addBigNumbers(bignumber1, bignumber2);
                break;
            case '-':
                result = subtractBigNumbers(bignumber1, bignumber2);
                break;
            case '*':
                result = multiplyBigNumbers(bignumber1, bignumber2);
                break;
            case '/':
                result = divideBigNumbers(bignumber1, bignumber2);
                break;
            case '%':
                result = restoDivisao(bignumber1, bignumber2);
                break;
            case '^':
                result = exponenciacao(bignumber1, bignumber2);
                break;
            default:
                fprintf(stderr, "Operação inválida!\n");
                break;
        }

        if (result) {
            printBigNumber(result);
            freeBigNumber(result);
        }

        free(input1);
        free(input2);
        free(operador);
        freeBigNumber(bignumber1);
        freeBigNumber(bignumber2);

    }
    return 0;
}
