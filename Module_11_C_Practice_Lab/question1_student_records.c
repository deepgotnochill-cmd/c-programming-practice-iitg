#include <stdio.h>

#define STUDENTS 3
#define COURSES 4

struct Course {
    char code[20];
    char name[50];
    float marks;
};

struct Student {
    char name[50];
    int rollNo;
    struct Course courses[COURSES];
};

int main(void) {
    struct Student students[STUDENTS];
    float sum, average;

    for (int i = 0; i < STUDENTS; i++) {
        printf("\nEnter details of Student %d:\n", i + 1);
        printf("Enter name: ");
        scanf(" %49[^\n]", students[i].name);
        printf("Enter roll number: ");
        scanf("%d", &students[i].rollNo);

        for (int j = 0; j < COURSES; j++) {
            printf("\nCourse %d:\n", j + 1);
            printf("Enter course code: ");
            scanf("%19s", students[i].courses[j].code);
            printf("Enter course name: ");
            scanf(" %49[^\n]", students[i].courses[j].name);
            printf("Enter marks: ");
            scanf("%f", &students[i].courses[j].marks);
        }
    }

    printf("\n----- STUDENT RESULTS -----\n");
    for (int i = 0; i < STUDENTS; i++) {
        sum = 0;
        for (int j = 0; j < COURSES; j++) {
            sum += students[i].courses[j].marks;
        }
        average = sum / COURSES;
        printf("\nName: %s", students[i].name);
        printf("\nRoll Number: %d", students[i].rollNo);
        printf("\nAverage Marks: %.2f\n", average);
    }
    return 0;
}
