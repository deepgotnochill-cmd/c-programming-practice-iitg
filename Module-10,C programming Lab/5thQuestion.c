/*
 * Question 5: Write a C program to compute the average of numbers given as
 * command line arguments.
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <num1> <num2> ... <numN>\n", argv[0]);
        return 1;
    }

    double sum = 0.0;
    int count = argc - 1;

    for (int i = 1; i < argc; i++) {
        sum += atof(argv[i]);
    }

    double average = sum / count;
    printf("Average of %d numbers = %.2f\n", count, average);

    return 0;
}
