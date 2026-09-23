#include <stdio.h>

int main() {

    int a, b, c, d, e;
    int total;
    float percentage;

    printf("Enter marks of 5 subjects:\n");

    scanf("%d", &a);
    scanf("%d", &b);
    scanf("%d", &c);
    scanf("%d", &d);
    scanf("%d", &e);

    total = a + b + c + d + e;
    percentage = total / 5.0;

    printf("\nTotal = %d\n", total);
    printf("Percentage = %.2f%%\n", percentage);

    if (percentage >= 90)
        printf("Grade = A\n");
    else if (percentage >= 80)
        printf("Grade = B\n");
    else if (percentage >= 70)
        printf("Grade = C\n");
    else if (percentage >= 60)
        printf("Grade = D\n");
    else
        printf("Grade = F\n");

    return 0;
}