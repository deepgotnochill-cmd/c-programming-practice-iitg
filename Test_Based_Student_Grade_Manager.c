#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LENGTH 50
#define SUBJECT_COUNT 3 
typedef struct {
    int id;
    char name[NAME_LENGTH];
    float grades[SUBJECT_COUNT];
} Student;

/* Function declarations */
void showMenu(void);
void addStudent(Student students[], int *count);
void listStudents(const Student students[], int count);
void searchStudent(const Student students[], int count);
float calculateAverage(const Student *student);
void showTopStudent(const Student students[], int count);
void printStudent(const Student *student);

int main(void) {
    Student students[MAX_STUDENTS];
    int count = 0;
    int choice;

    do {
        showMenu();
        printf("Choose an option: ");

        if (scanf("%d", &choice) != 1) {
            printf("Please enter a number.\n");
            return 1;
        }

        switch (choice) {
            case 1:
                addStudent(students, &count);
                break;
            case 2:
                listStudents(students, count);
                break;
            case 3:
                searchStudent(students, count);
                break;
            case 4:
                showTopStudent(students, count);
                break;
            case 5:
                printf("Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 5);

    return 0;
}

void showMenu(void) {
    printf("\n=== Student Grade Manager ===\n");
    printf("1. Add student\n");
    printf("2. List students\n");
    printf("3. Search by ID\n");
    printf("4. Show top student\n");
    printf("5. Exit\n");
}

void addStudent(Student students[], int *count) {
    if (*count >= MAX_STUDENTS) {
        printf("The student list is full.\n");
        return;
    }

    Student newStudent;

    printf("Enter student ID: ");
    if (scanf("%d", &newStudent.id) != 1) {
        printf("Invalid ID.\n");
        return;
    }

    /* TODO: Check whether this ID is already in use. */

    printf("Enter student name (one word): ");
    if (scanf("%49s", newStudent.name) != 1) {
        printf("Could not read the name.\n");
        return;
    }

    for (int i = 0; i < SUBJECT_COUNT; i++) {
        printf("Enter grade for subject %d (0-100): ", i + 1);

        if (scanf("%f", &newStudent.grades[i]) != 1) {
            printf("Invalid grade.\n");
            return;
        }

        /* TODO: Reject grades outside the 0-100 range. */
    }

    students[*count] = newStudent;
    (*count)++;

    printf("Student added.\n");
}

void listStudents(const Student students[], int count) {
    if (count == 0) {
        printf("No students have been added yet.\n");
        return;
    }

    printf("\n--- Student List ---\n");

    for (int i = 0; i < count; i++) {
        printStudent(&students[i]);
    }
}

void searchStudent(const Student students[], int count) {
    int id;

    printf("Enter the ID to search for: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID.\n");
        return;
    }

    /* TODO: Search the array for this ID. */
    /* If found, call printStudent and return. */

    printf("Student not found.\n");
}

float calculateAverage(const Student *student) {
    float total = 0.0f;

    for (int i = 0; i < SUBJECT_COUNT; i++) {
        total += student->grades[i];
    }

    return total / SUBJECT_COUNT;
}

void showTopStudent(const Student students[], int count) {
    if (count == 0) {
        printf("No students to compare.\n");
        return;
    }

    /* TODO: Find the student with the highest average. */
    /* Print that student using printStudent. */
}

void printStudent(const Student *student) {
    printf("ID: %d | Name: %s | Grades: ",
           student->id, student->name);

    for (int i = 0; i < SUBJECT_COUNT; i++) {
        printf("%.1f", student->grades[i]);

        if (i < SUBJECT_COUNT - 1) {
            printf(", ");
        }
    }

    printf(" | Average: %.2f\n", calculateAverage(student));
}