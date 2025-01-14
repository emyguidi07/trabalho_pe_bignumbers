/*
Authors: Emily Guidi 11202320595
         Paloma Lima 11202021803
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

BigNumber *stringToBigNumber(const char *str) {
    BigNumber *bn = createBigNumber(str); 
    if (bn == NULL) {
        fprintf(stderr, "Erro ao criar BigNumber.\n");
        return NULL;
    }
    return bn;
}

// Divide BigNumbers
BigNumber *divideBigNumbers(BigNumber *a, BigNumber *b) {
    if (!b || (b->head == b->tail && b->head->digit == 0)) {
        fprintf(stderr, "Division by zero error.\n");
        return NULL;
    }
    // If a is zero, the result is zero
    if (a->head == a->tail && a->head->digit == 0) {
        return createBigNumber("0");
    }
    BigNumber *quotient = createBigNumber("0");
    BigNumber *current = createBigNumber("0");
    Node *currentNode = a->head;
    while (currentNode) {
        // Append the current digit of the a to the "current" number
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->digit = currentNode->digit;
        newNode->prev = current->tail;
        newNode->next = NULL;
        if (current->tail) {
            current->tail->next = newNode;
        } else {
            current->head = newNode;
        }
        current->tail = newNode;
        // Remove leading zeros in the current number
        while (current->head && current->head->digit == 0 && current->head != current->tail) {
            Node *temp = current->head;
            current->head = current->head->next;
            if (current->head) current->head->prev = NULL;
            free(temp);
        }
        // Find the largest multiple of b that fits into "current"
        int count = 0;
        while (compareBigNumbers(current, b) >= 0) {
            BigNumber *temp = subtractPositiveBigNumbers(current, b);
            freeBigNumber(current);
            current = temp;
            count++;
        }
        // Add the count to the quotient
        Node *quotientNode = (Node *)malloc(sizeof(Node));
        quotientNode->digit = count;
        quotientNode->prev = quotient->tail;
        quotientNode->next = NULL;
        if (quotient->tail) {
            quotient->tail->next = quotientNode;
        } else {
            quotient->head = quotientNode;
        }
        quotient->tail = quotientNode;
        // Move to the next digit
        currentNode = currentNode->next;
    }
    // Remove leading zeros in the quotient
    while (quotient->head && quotient->head->digit == 0 && quotient->head != quotient->tail) {
        Node *temp = quotient->head;
        quotient->head = quotient->head->next;
        if (quotient->head) quotient->head->prev = NULL;
        free(temp);
    }
    // Set the sign of the result
    quotient->isNegative = (a->isNegative != b->isNegative);
    if (quotient->head->digit == 0) {
        quotient->isNegative = false;
    }
    freeBigNumber(current);
    return quotient;
}

//copy a BigNumber
BigNumber *createBigNumberFromBigNumber(BigNumber *src) {
      BigNumber *copy = (BigNumber *)malloc(sizeof(BigNumber));
    copy->head = NULL;
    copy->tail = NULL;
    copy->isNegative = src->isNegative;
    Node *current = src->head;
    while (current && current->digit == 0) {
        current = current->next;
    }
    if (!current) {
        Node *zeroNode = (Node *)malloc(sizeof(Node));
        zeroNode->digit = 0;
        zeroNode->prev = NULL;
        zeroNode->next = NULL;
        copy->head = zeroNode;
        copy->tail = zeroNode;
        return copy;
    }
    while (current) {
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->digit = current->digit;
        newNode->prev = copy->tail;
        newNode->next = NULL;
        if (copy->tail) {
            copy->tail->next = newNode;
        } else {
            copy->head = newNode;
        }
        copy->tail = newNode;
        current = current->next;
    }
    return copy;
}


// Exponentiate
BigNumber *exponenciacao(BigNumber *base, BigNumber *expoente) {
    // Base case: return 1 when exponent is 0
     if (compareBigNumbers(expoente, createBigNumber("0")) == 0) {
        return createBigNumber("1");
    }
    // Base case: return base when exponent is 1
    if (compareBigNumbers(expoente, createBigNumber("1")) == 0) {
        return createBigNumberFromBigNumber(base);
    }
    // divide exponent / 2
    BigNumber *metade = divideBigNumbers(expoente, createBigNumber("2"));
    // recursive
    BigNumber *temp = exponenciacao(base, metade);
    // multiply result
    BigNumber *resultado = multiplyBigNumbers(temp, temp);
    // If the exponent is odd, multiply by the base value
    if (expoente->head->digit % 2 == 1) {
        BigNumber *tempResultado = multiplyBigNumbers(resultado, base);
        freeBigNumber(resultado);
        resultado = tempResultado;
    }
    if (base->isNegative && (expoente->head->digit % 2 == 1)) {
        resultado->isNegative = 1;
    }
    freeBigNumber(metade);
    freeBigNumber(temp);
    return resultado;
}

// Remainder of division
BigNumber *restoDivisao(BigNumber *a, BigNumber *b) {
    BigNumber *quociente = divideBigNumbers(a, b);
    BigNumber *produto = multiplyBigNumbers(quociente, b);
    BigNumber *resto = subtractBigNumbers(a, produto);
    freeBigNumber(quociente);
    freeBigNumber(produto);
    return resto;
}

