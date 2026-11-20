#include <stdio.h>

struct Student {
    char name[30];
    int roll_number;
    float marks;
};

void readStudent(struct Student *student) {
    printf("Name: ");
    scanf(" %29[^\n]", student->name);

    printf("Roll number: ");
    scanf("%d", &student->roll_number);

    printf("Marks: ");
    scanf("%f", &student->marks);
}

void displayStudent(struct Student student) {
    printf("Name: %s | Roll number: %d | Marks: %.2f\n",
           student.name, student.roll_number, student.marks);
}

int main(void) {
    struct Student students[2];

    for (int i = 0; i < 2; i++) {
        printf("\nEnter details for student %d:\n", i + 1);
        readStudent(&students[i]);
    }

    printf("\nStudent details:\n");
    for (int i = 0; i < 2; i++) {
        displayStudent(students[i]);
    }

    return 0;
}