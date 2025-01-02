/*
Authors: Emily Guidi 11202320595
         Paloma Lima 11202021803
2024-12-06 - Emily - Implementation sum function and create files 
2024-12-15 - Emily - Implementation subtract function and karatsuba Multiply algorithm
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bignumber.h"

// Creates a new BigNumber structure and initializes it
BigNumber *createBigNumber(const char *str) {
    BigNumber *bn = (BigNumber *)malloc(sizeof(BigNumber));
    bn->head = bn->tail = NULL;
    bn->isNegative = (*str == '-');
    if (*str == '+' || *str == '-') str++;

    // Iterate through the string, creating nodes for each digit
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

// Frees all dynamically allocated memory associated with a BigNumber
void freeBigNumber(BigNumber *bn) {
    Node *current = bn->head;
    while (current) {
        Node *next = current->next;
        free(current);
        current = next;
    }
    free(bn);
}

// Adds two positive BigNumber structures
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

// Adds two BigNumber structures, handling negative numbers
BigNumber *addBigNumbers(BigNumber *a, BigNumber *b) {
    if (a->isNegative == b->isNegative) {
        // Both positive or both negative
        BigNumber *result = addPositiveBigNumbers(a, b);
        result->isNegative = a->isNegative; 
        return result;
    }
    int comparison = compareBigNumbers(a, b);
    if (comparison == 0) {
        // Result is zero
        return createBigNumber("0");
    }
    BigNumber *result;
    if (comparison > 0) {
        // a is larger, subtract b from a
        result = subtractPositiveBigNumbers(a, b);
        result->isNegative = a->isNegative;
    } else {
        // b is larger, subtract a from b
        result = subtractPositiveBigNumbers(b, a);
        result->isNegative = b->isNegative;
    }
    return result;
}

// Subtracts one positive BigNumber structure from another
BigNumber *subtractPositiveBigNumbers(BigNumber *a, BigNumber *b) {
    // Check which number is larger
    if (compareBigNumbers(a, b) < 0) {
        // Swap a and b if necessary
        BigNumber *result = subtractPositiveBigNumbers(b, a);
        result->isNegative = true;
        return result;
    }
    BigNumber *result = (BigNumber *)malloc(sizeof(BigNumber));
    result->head = result->tail = NULL;
    result->isNegative = false;
    Node *nodeA = a->tail;
    Node *nodeB = b->tail;
    int borrow = 0;
    while (nodeA || nodeB || borrow) {
        int digitA = nodeA ? nodeA->digit : 0;
        int digitB = nodeB ? nodeB->digit : 0;
        int diff = digitA - digitB - borrow;
        if (diff < 0) {
            // Borrow needed
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->digit = diff;
        newNode->prev = NULL;
        newNode->next = result->head;
        if (result->head) {
            result->head->prev = newNode;
        } else {
            result->tail = newNode;
        }
        result->head = newNode;

        if (nodeA) nodeA = nodeA->prev;
        if (nodeB) nodeB = nodeB->prev;
    }
    // Remove leading zeros
    while (result->head && result->head->digit == 0 && result->head != result->tail) {
        Node *temp = result->head;
        result->head = result->head->next;
        result->head->prev = NULL;
        free(temp);
    }
    return result;
}

// Subtracts one BigNumber structure from another, handling negative numbers
BigNumber *subtractBigNumbers(BigNumber *a, BigNumber *b) {
    // When both are positive, perform normal subtraction
    if (!a->isNegative && !b->isNegative) {
        return subtractPositiveBigNumbers(a, b);
    }
    // When both are negative
    if (a->isNegative && b->isNegative) {
        // -a - (-b) = -a + b
        int comparison = compareBigNumbers(b, a); // compares |b| with |a|
        if (comparison > 0) {
            // If |b| > |a|, the result will be positive
            BigNumber *result = subtractPositiveBigNumbers(b, a);
            result->isNegative = false;
            return result;
        } else if (comparison == 0) {
            // If |b| == |a|, the result will be zero
            return createBigNumber("0");
        } else {
            // If |a| > |b|, the result will be negative
            BigNumber *result = subtractPositiveBigNumbers(a, b);
            result->isNegative = true;
            return result;
        }
    }
    // When a is negative and b is positive: -a - b = -(a + b)
    if (a->isNegative && !b->isNegative) {
        BigNumber *result = addPositiveBigNumbers(a, b); // a + b
        result->isNegative = true;
        return result;
    }
    // When a is positive and b is negative: a - (-b) = a + b
    return addPositiveBigNumbers(a, b);  // a + b
}

// Compares two BigNumber structures without considering the sign
int compareBigNumbers(BigNumber *a, BigNumber *b) {
    int lenA = getLength(a);
    int lenB = getLength(b);
    if (lenA != lenB) return lenA - lenB;
    Node *nodeA = a->head;
    Node *nodeB = b->head;
    while (nodeA && nodeB) {
        if (nodeA->digit != nodeB->digit) {
            return nodeA->digit - nodeB->digit;
        }
        nodeA = nodeA->next;
        nodeB = nodeB->next;
    }
    return 0;
}

BigNumber *shiftBigNumber(BigNumber *num, int m) {
    for (int i = 0; i < m; i++) {
        // Crie um novo nó com o dígito 0 diretamente dentro da função
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->digit = 0;
        newNode->next = NULL;
        newNode->prev = NULL;

        // Adicione o novo nó ao final do BigNumber
        if (num->tail == NULL) {
            // Se o número estiver vazio, inicialize a lista
            num->head = num->tail = newNode;
        } else {
            // Caso contrário, anexe ao final
            num->tail->next = newNode;
            newNode->prev = num->tail;
            num->tail = newNode;
        }
    }
    return num;
}

void appendNode(BigNumber *bigNumber, Node *newNode) {
    if (bigNumber->head == NULL) {
        // Se a lista está vazia, o novo nó é tanto a cabeça quanto a cauda.
        bigNumber->head = bigNumber->tail = newNode;
    } else {
        // Adicione o nó ao final da lista.
        bigNumber->tail->next = newNode;
        newNode->prev = bigNumber->tail;
        bigNumber->tail = newNode;
    }
    newNode->next = NULL; // Assegure-se de que o próximo nó é NULL.
}

void splitBigNumber(BigNumber *original, BigNumber **high, BigNumber **low, int m) {
    Node *current = original->head;
    *high = createBigNumber("0");
    *low = createBigNumber("0");

    int count = 0;
    while (current) {
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->digit = current->digit;
        newNode->prev = NULL;
        newNode->next = NULL;

        // Verifica se deve adicionar à parte "high" ou "low"
        if (count < m) {
            appendNode(*high, newNode);  // Adiciona à parte "high"
        } else {
            appendNode(*low, newNode);   // Adiciona à parte "low"
        }

        current = current->next;
        count++;
    }

    // Exibindo as partes para verificar
    printf("High part: ");
    printBigNumber(*high);
    printf("Low part: ");
    printBigNumber(*low);
}


//Multiply BigNumbers 
BigNumber *multiplyBigNumbers(BigNumber *a, BigNumber *b) {
    BigNumber *result = (BigNumber *)malloc(sizeof(BigNumber));
    result->head = result->tail = NULL;
    result->isNegative = a->isNegative != b->isNegative;  
    int len1 = 0, len2 = 0;
    Node *temp1 = a->head;
    while (temp1) {
        len1++;
        temp1 = temp1->next;
    }
    Node *temp2 = b->head;
    while (temp2) {
        len2++;
        temp2 = temp2->next;
    }
    // array to product
    int *product = (int *)calloc(len1 + len2, sizeof(int));
    // multiply bignumbers
    Node *node1 = a->tail;
    for (int i = 0; i < len1; i++) {
        Node *node2 = b->tail;
        for (int j = 0; j < len2; j++) {
            product[i + j] += (node1->digit) * (node2->digit);
            node2 = node2->prev;
        }
        node1 = node1->prev;
    }
    // init carry
    int carry = 0;
    for (int k = 0; k < len1 + len2; k++) {
        product[k] += carry;
        carry = product[k] / 10;
        product[k] %= 10;
    }
    // create result bignumber
    for (int i = len1 + len2 - 1; i >= 0; i--) {
        // verify if it has left zero
        if (i < len1 + len2 - 1 || product[i] != 0) {
            Node *newNode = (Node *)malloc(sizeof(Node));
            newNode->digit = product[i];
            newNode->prev = result->tail;
            newNode->next = NULL;
            
            if (result->tail) {
                result->tail->next = newNode;
            } else {
                result->head = newNode;
            }
            result->tail = newNode;
        }
    }
    free(product);  
    return result;
}

BigNumber *karatsuba(BigNumber *a, BigNumber *b) {
    int len1 = getLength(a);
    int len2 = getLength(b);
    // Caso base: se os números forem pequenos, use a multiplicação direta
    if (len1 == 1 || len2 == 1) {
        return multiplyBigNumbers(a, b);
    }

    // Determinar o tamanho da divisão (média entre os tamanhos)
    int m = len1  / 2;
    int n = len2  / 2;
    // Dividir os números em metades
    BigNumber *a0, *a1, *b0, *b1;
    splitBigNumber(a, &a1, &a0, m);
    splitBigNumber(b, &b1, &b0, n);

    // Inicializar variáveis intermediárias
    BigNumber *z0 = multiplyBigNumbers(a0, b0); // z0 = a0 * b0
    BigNumber *z2 = multiplyBigNumbers(a1, b1); // z2 = a1 * b1

    // Calcular as somas intermediárias
    BigNumber *sumA = addBigNumbers(a0, a1); // (a0 + a1)
    BigNumber *sumB = addBigNumbers(b0, b1); // (b0 + b1)
    BigNumber *z1 = multiplyBigNumbers(sumA, sumB); // z1 = (a0 + a1) * (b0 + b1)

    // Ajustar z1: z1 = z1 - z0 - z2
    BigNumber *temp = addBigNumbers(z0, z2);
    z1 = subtractBigNumbers(z1, temp);

    // Combinar os resultados finais
    BigNumber *z2Shifted = shiftBigNumber(z2, 2 * m); // z2 * 10^(2*m)
    BigNumber *z1Shifted = shiftBigNumber(z1, m);     // z1 * 10^m
    BigNumber *result = addBigNumbers(z2Shifted, z1Shifted);
    result = addBigNumbers(result, z0);

    // Liberar memória alocada para BigNumbers intermediários
    freeBigNumber(a0);
    freeBigNumber(a1);
    freeBigNumber(b0);
    freeBigNumber(b1);
    freeBigNumber(z0);
    freeBigNumber(z1);
    freeBigNumber(z2);
    freeBigNumber(temp);
    freeBigNumber(z2Shifted);
    freeBigNumber(z1Shifted);
    freeBigNumber(sumA);
    freeBigNumber(sumB);

    return result;
}


// Returns the length of a BigNumber
int getLength(BigNumber *bn) {
    int length = 0;
    for (Node *current = bn->head; current; current = current->next) 
        length++;
    return length;
}

// Prints a BigNumber to the console
void printBigNumber(BigNumber *bn) {
    if (bn->isNegative) {
        printf("-");
    }
    Node *current = bn->head;
    while (current) {
        printf("%d", current->digit);
        current = current->next;
    }
    printf("\n");
}

// dinamic inputs
char *readinput() {
    char *input = NULL;
    char c;
    int length = 0;

    while (1) {
        c = getchar();
        if (c == '\n') {
            break;
        }

        char *input_new = realloc(input, (length + 1) * sizeof(char));
        if (input_new == NULL) {
            printf("Erro ao alocar memória.\n");
            free(input);
            return NULL;
        }
        input = input_new;

        input[length] = c;
        length++;
    }

    char *input_final = realloc(input, (length + 1) * sizeof(char));
    input = input_final;
    input[length] = '\0';

    return input;
}

// Convert to Bignumber
BigNumber *stringToBigNumber(const char *str) {
    BigNumber *bn = createBigNumber(str); 
    if (bn == NULL) {
        fprintf(stderr, "Erro ao criar BigNumber.\n");
        return NULL;
    }
    return bn;
}

// Function to perform integer division of two BigNumbers
BigNumber *divideBigNumbers(BigNumber *a, BigNumber *b) {
    if (compareBigNumbers(a, b) < 0) {
        return createBigNumber("0");
    }

    BigNumber *result = createBigNumber("0");
    BigNumber *temp = createBigNumber("0");

    while (compareBigNumbers(a, b) >= 0) {
        a = subtractBigNumbers(a, b);
        result = addBigNumbers(result, createBigNumber("1"));
    }
    freeBigNumber(temp);
    return result;
}

// Recursive function to calculate exponentiation of a BigNumber
BigNumber *exponentiate(BigNumber *base, BigNumber *exponent) {
    if (compareBigNumbers(exponent, createBigNumber("0")) == 0) {
        return createBigNumber("1");
    }

    BigNumber *half = divideBigNumbers(exponent, createBigNumber("2"));
    BigNumber *temp = exponentiate(base, half);
    freeBigNumber(half);

    if (exponent->isNegative) {
        return divideBigNumbers(createBigNumber("1"), multiplyBigNumbers(temp, temp));
    } else if (exponent->head->digit % 2 == 0) {
        return multiplyBigNumbers(temp, temp);
    } else {
        return multiplyBigNumbers(base, multiplyBigNumbers(temp, temp));
    }
}

// Function to calculate the remainder of the division of two BigNumbers
BigNumber *modulo(BigNumber *a, BigNumber *b) {
    BigNumber *quotient = divideBigNumbers(a, b);

    BigNumber *product = multiplyBigNumbers(quotient, b);

    BigNumber *remainder = subtractBigNumbers(a, product);

    freeBigNumber(quotient);
    freeBigNumber(product);

    return remainder;
}
