#include <stdio.h>
#include <stdlib.h>

struct student {
    char name[30];
    int roll_no;
    float marks;
};

int main(void) {
    int count;

    /* 1. Declare the structure type: done above */

    /* 2–3. Define a pointer and allocate memory using malloc */
    struct student *one_student = malloc(sizeof(struct student));

    if (one_student == NULL) {
        printf("Memory allocation with malloc failed.\n");
        return 1;
    }

    /* 4. Populate the structure members */
    printf("Enter one student's name, roll number, and marks:\n");
    scanf(" %29[^\n]", one_student->name);
    scanf("%d", &one_student->roll_no);
    scanf("%f", &one_student->marks);

    /* 5. Access and use the structure */
    printf("\nStudent allocated with malloc:\n");
    printf("Name: %s\n", one_student->name);
    printf("Roll number: %d\n", one_student->roll_no);
    printf("Marks: %.2f\n", one_student->marks);

    /* 2–3. Define a pointer and allocate memory for an array using calloc */
    printf("\nHow many students should be stored in the calloc array? ");
    scanf("%d", &count);

    if (count <= 0) {
        printf("The number of students must be positive.\n");
        free(one_student);
        return 1;
    }

    struct student *students = calloc((size_t)count, sizeof(struct student));

    if (students == NULL) {
        printf("Memory allocation with calloc failed.\n");
        free(one_student);
        return 1;
    }

    /* 4. Populate the array's structure members */
    for (int i = 0; i < count; i++) {
        printf("\nEnter details for student %d:\n", i + 1);
        scanf(" %29[^\n]", students[i].name);
        scanf("%d", &students[i].roll_no);
        scanf("%f", &students[i].marks);
    }

    /* 5. Access and use the structures */
    printf("\nStudents allocated with calloc:\n");
    for (int i = 0; i < count; i++) {
        printf("\nStudent %d\n", i + 1);
        printf("Name: %s\n", students[i].name);
        printf("Roll number: %d\n", students[i].roll_no);
        printf("Marks: %.2f\n", students[i].marks);
    }

    /* 6. Free the allocated structures */
    free(one_student);
    free(students);

    return 0;
}