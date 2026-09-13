#include <stdio.h>

int main(void)
{
    float physics, chemistry, maths, english, computer;
    float total, percentage;

    printf("Enter Marks For 5 Subjects:\n");

    printf("Physics: ");
    scanf("%f", &physics);

    printf("Chemistry: ");
    scanf("%f", &chemistry);

    printf("Mathematics: ");
    scanf("%f", &maths);

    printf("English: ");
    scanf("%f", &english);

    printf("Computer Science: ");
    scanf("%f", &computer);

    total = physics + chemistry + maths + english + computer;
    percentage = total / 5;

    printf("\n--------------- RESULT ---------------\n");
    printf("Total Marks: %.2f / 500\n", total);
    printf("Percentage: %.2f%%\n", percentage);

    if (percentage >= 90)
        printf("Grade: A+\n");
    else if (percentage >= 80)
        printf("Grade: A\n");
    else if (percentage >= 70)
        printf("Grade: B\n");
    else if (percentage >= 60)
        printf("Grade: C\n");
    else if (percentage >= 50)
        printf("Grade: D\n");
    else
        printf("Grade: F\n");

    return 0;
}