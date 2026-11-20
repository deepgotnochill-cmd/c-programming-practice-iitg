#include <stdio.h>
#include <string.h>

struct Student {
    char name[30];
    int roll_number;
    float marks;
};

int main(void) {
    struct Student students[2];

    for (int i = 0; i < 2; i++) {
        printf("Enter details for student %d\n", i + 1);

        printf("Name: ");
        scanf(" %29[^\n]", students[i].name);

        printf("Roll number: ");
        scanf("%d", &students[i].roll_number);

        printf("Marks: ");
        scanf("%f", &students[i].marks);
    }

    printf("\nStudent details:\n");
    for (int i = 0; i < 2; i++) {
        printf("%s, Roll number: %d, Marks: %.2f\n",
               students[i].name,
               students[i].roll_number,
               students[i].marks);
    }

    return 0;
}