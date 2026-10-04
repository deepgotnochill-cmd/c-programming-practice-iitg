#include <stdio.h>

int main(void) {
    int number, choice;

    printf("Enter a positive integer: ");
    scanf("%d", &number);

    do {
        printf("\nMenu\n");
        printf("1) Check if the number is prime\n");
        printf("2) Check if the number is an Armstrong number\n");
        printf("3) Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        if (choice == 1) {
            int is_prime = number > 1;

            for (int i = 2; i <= number / i; i++) {
                if (number % i == 0) {
                    is_prime = 0;
                    break;
                }
            }

            printf("%s\n", is_prime ? "Yes" : "No");
        } else if (choice == 2) {
            int original = number;
            int digits = 0;
            int sum = 0;

            for (int temp = number; temp > 0; temp /= 10) {
                digits++;
            }

            for (int temp = number; temp > 0; temp /= 10) {
                int digit = temp % 10;
                int power = 1;

                for (int i = 0; i < digits; i++) {
                    power *= digit;
                }

                sum += power;
            }

            printf("%s\n", sum == original ? "Yes" : "No");
        } else if (choice != 3) {
            printf("Invalid option. Please choose 1, 2, or 3.\n");
        }
    } while (choice != 3);

    printf("Goodbye!\n");
    return 0;
}