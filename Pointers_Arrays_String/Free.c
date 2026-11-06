#include <stdio.h>

// Recursive function to read and print integers in reverse order
void reverse(int n) {
    // Base case: stop when no more integers need to be read
    if (n <= 0) {
        return;
    }

    int num;
    // Read the current integer from the sequence
    scanf("%d", &num);

    // Recursive call to handle the remaining n-1 integers
    reverse(n - 1);

    // Print the current integer on the way back up the call stack
    printf("%d ", num);
}

int main() {
    int total_elements;

    printf("Enter the number of integers in the sequence: ");
    if (scanf("%d", &total_elements) != 1 || total_elements <= 0) {
        printf("Invalid input.\n");
        return 1;
    }

    printf("Enter the sequence of %d integers:\n", total_elements);
    reverse(total_elements);
    printf("\n");

    return 0;
}
