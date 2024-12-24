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

// Multiplies two BigNumber structures using the Karatsuba algorithm
BigNumber *karatsubaMultiply(BigNumber *a, BigNumber *b) {
    // Base case: single-digit numbers
    if (isSingleDigit(a) && isSingleDigit(b)) {
        int product = (a->head->digit) * (b->head->digit);
        char buffer[10];
        snprintf(buffer, sizeof(buffer), "%d", product);

        BigNumber *result = createBigNumber(buffer);
        // Adjust the sign for the base case
        result->isNegative = (a->isNegative != b->isNegative);
        return result;
    }
    // Determine the length of the numbers
    int lenA = getLength(a);
    int lenB = getLength(b);
    int maxLen = lenA > lenB ? lenA : lenB;
    int half = maxLen / 2;
    // Split the numbers into high and low parts
    BigNumber *highA, *lowA, *highB, *lowB;
    splitBigNumber(a, half, &highA, &lowA);
    splitBigNumber(b, half, &highB, &lowB);
    // Calculate the products recursively
    BigNumber *z0 = karatsubaMultiply(lowA, lowB);                // Product of lower parts
    BigNumber *z1 = karatsubaMultiply(addBigNumbers(lowA, highA), addBigNumbers(lowB, highB)); // Cross product
    BigNumber *z2 = karatsubaMultiply(highA, highB);              // Product of higher parts
    // Adjust z1 to subtract z0 and z2
    BigNumber *z1Adjusted = subtractBigNumbers(z1, addBigNumbers(z0, z2));
    // Combine the results
    BigNumber *result = addBigNumbers(
        addBigNumbers(shiftBigNumber(z2, 2 * half), shiftBigNumber(z1Adjusted, half)),
        z0
    );
    // Adjust the final sign of the result
    result->isNegative = (a->isNegative != b->isNegative);
    // Free temporary memory
    freeBigNumber(z0);
    freeBigNumber(z1);
    freeBigNumber(z2);
    freeBigNumber(z1Adjusted);
    freeBigNumber(highA);
    freeBigNumber(lowA);
    freeBigNumber(highB);
    freeBigNumber(lowB);
    return result;
}

// Checks if a BigNumber has only one digit
bool isSingleDigit(BigNumber *bn) {
    return (bn->head == bn->tail);
}

// Returns the length of a BigNumber
int getLength(BigNumber *bn) {
    int length = 0;
    for (Node *current = bn->head; current; current = current->next) 
        length++;
    return length;
}

// Splits a BigNumber into high and low parts
void splitBigNumber(BigNumber *bn, int position, BigNumber **high, BigNumber **low) {
    *high = createBigNumber("0");
    *low = createBigNumber("0");
    Node *current = bn->head;
    int index = 0;
    while (current) {
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->digit = current->digit;
        newNode->prev = newNode->next = NULL;
        if (index < position) 
            appendNode(*low, newNode);
        else
            appendNode(*high, newNode);
        current = current->next;
        index++;
    }
}

// Appends a new node to an existing BigNumber
void appendNode(BigNumber *bn, Node *newNode) {
    if (!bn->head) {
        bn->head = bn->tail = newNode;
    } else {
        bn->tail->next = newNode;
        newNode->prev = bn->tail;
        bn->tail = newNode;
    }
}

// Shifts a BigNumber left by a specified number of positions
BigNumber *shiftBigNumber(BigNumber *bn, int positions) {
    if (isSingleDigit(bn) && bn->head->digit == 0) {
        return createBigNumber("0");
    }
    // Create a new BigNumber to store the shifted result
    BigNumber *result = createBigNumber("0");
    // Copy digits from original BigNumber to result
    for (Node *current = bn->head; current; current = current->next) {
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->digit = current->digit;
        newNode->prev = newNode->next = NULL;
        appendNode(result, newNode);
    }
    // Append leading zeros based on the shift amount
    for (int i = 0; i < positions; i++) {
        Node *newNode = (Node *)malloc(sizeof(Node));
        newNode->digit = 0;
        newNode->prev = NULL;
        newNode->next = result->head;
        result->head->prev = newNode;
        result->head = newNode;
    }
    return result;
}

// Placeholder function for dividing two BigNumbers
BigNumber *divideBigNumbers(BigNumber *a, BigNumber *b) {
    // Aqui será implementada a divisão
    return createBigNumber("0"); // Placeholder
}

// Prints a BigNumber to the console
void printBigNumber(BigNumber *bn) {
    if (bn->isNegative) printf("-");
    for (Node *current = bn->head; current != NULL; current = current->next) {
        printf("%c", current->digit + '0');
    }
    printf("\n");
}

// Parses input string to create a BigNumber structure
BigNumber *parseBigNumberInput() {
    char buffer[10000];
    if (scanf("%s", buffer) != 1) return NULL;
    return createBigNumber(buffer);
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
