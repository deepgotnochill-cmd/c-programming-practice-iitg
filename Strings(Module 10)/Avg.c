#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    float sum = 0;
    int i, num;

    // The number of arguments passed (excluding the program name itself)
    num = argc - 1;

    // Check if any numbers were actually passed to avoid division by zero
    if (num == 0) {
        printf("No numbers provided to calculate an average.\n");
        return 1;
    }

    // Loop through each command line argument starting from index 1
    for (i = 1; i <= num; i++) {
        sum += atof(argv[i]); // Convert string to a float and add to sum
    }

    // Calculate and print the average
    float average = sum / num;
    printf("The average of the given numbers is: %.2f\n", average);

    return 0;
}
