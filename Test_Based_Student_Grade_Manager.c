#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

#define MAX_STUDENTS 100
#define NAME_LENGTH 50
#define SUBJECT_COUNT 3

typedef struct {
    int id;
    char name[NAME_LENGTH];
    float grades[SUBJECT_COUNT];
} Student;

void showMenu(void);
int readInt(const char *prompt, int *value);
int readFloat(const char *prompt, float *value);
int readName(const char *prompt, char name[], size_t size);
int findStudentById(const Student students[], int count, int id);
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

    for (;;) {
        showMenu();

        if (!readInt("Choose an option: ", &choice)) {
            printf("\nGoodbye!\n");
            break;
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
                return 0;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

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

/* Returns 1 on success, 0 on end-of-file or invalid input. */
int readInt(const char *prompt, int *value) {
    char line[128];
    char *end;
    long parsed;

    printf("%s", prompt);
    if (fgets(line, sizeof line, stdin) == NULL) {
        return 0;
    }

    errno = 0;
    parsed = strtol(line, &end, 10);

    while (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r') {
        end++;
    }

    if (end == line || *end != '\0' || errno == ERANGE ||
        parsed < INT_MIN || parsed > INT_MAX) {
        printf("Please enter a valid whole number.\n");
        return 0;
    }

    *value = (int)parsed;
    return 1;
}

/* Returns 1 on success, 0 on end-of-file or invalid input. */
int readFloat(const char *prompt, float *value) {
    char line[128];
    char *end;
    float parsed;

    printf("%s", prompt);
    if (fgets(line, sizeof line, stdin) == NULL) {
        return 0;
    }

    errno = 0;
    parsed = strtof(line, &end);

    while (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r') {
        end++;
    }

    if (end == line || *end != '\0' || errno == ERANGE) {
        printf("Please enter a valid number.\n");
        return 0;
    }

    *value = parsed;
    return 1;
}

/* Reads a name that may contain spaces. */
int readName(const char *prompt, char name[], size_t size) {
    char line[256];
    size_t length;

    printf("%s", prompt);
    if (fgets(line, sizeof line, stdin) == NULL) {
        return 0;
    }

    length = strcspn(line, "\r\n");
    line[length] = '\0';

    if (length == 0 || length >= size) {
        printf("Name must be between 1 and %zu characters.\n", size - 1);
        return 0;
    }

    memcpy(name, line, length + 1);
    return 1;
}

int findStudentById(const Student students[], int count, int id) {
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            return i;
        }
    }

    return -1;
}

void addStudent(Student students[], int *count) {
    Student newStudent;

    if (*count >= MAX_STUDENTS) {
        printf("The student list is full.\n");
        return;
    }

    for (;;) {
        if (!readInt("Enter a positive student ID: ", &newStudent.id)) {
            return;
        }

        if (newStudent.id <= 0) {
            printf("ID must be positive.\n");
        } else if (findStudentById(students, *count, newStudent.id) >= 0) {
            printf("That ID is already in use.\n");
        } else {
            break;
        }
    }

    if (!readName("Enter student name: ", newStudent.name,
                  sizeof newStudent.name)) {
        return;
    }

    for (int i = 0; i < SUBJECT_COUNT; i++) {
        for (;;) {
            char prompt[80];
            float grade;

            snprintf(prompt, sizeof prompt,
                     "Enter grade for subject %d (0-100): ", i + 1);

            if (!readFloat(prompt, &grade)) {
                return;
            }

            if (grade < 0.0f || grade > 100.0f) {
                printf("Grade must be between 0 and 100.\n");
                continue;
            }

            newStudent.grades[i] = grade;
            break;
        }
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

    if (!readInt("Enter the ID to search for: ", &id)) {
        return;
    }

    int index = findStudentById(students, count, id);
    if (index >= 0) {
        printStudent(&students[index]);
    } else {
        printf("Student not found.\n");
    }
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

    int topIndex = 0;
    for (int i = 1; i < count; i++) {
        if (calculateAverage(&students[i]) >
            calculateAverage(&students[topIndex])) {
            topIndex = i;
        }
    }

    printf("Top student:\n");
    printStudent(&students[topIndex]);
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