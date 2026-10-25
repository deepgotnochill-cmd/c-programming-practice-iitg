#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define NAME_LENGTH 50
#define SUBJECT_COUNT 3
#define INPUT_LENGTH 256

typedef struct { int id; char name[NAME_LENGTH]; float grades[SUBJECT_COUNT]; } Student;
typedef enum { INPUT_OK, INPUT_INVALID, INPUT_EOF } InputResult;

static InputResult readLine(const char *prompt, char *buffer, size_t size);
static InputResult readInt(const char *prompt, int *value);
static InputResult readName(char *name, size_t size);
static InputResult readGrade(int subject, float *grade);
static int findStudentById(const Student students[], int count, int id);
static float calculateAverage(const Student *student);
static void printStudent(const Student *student);
static void addStudent(Student students[], int *count);
static void listStudents(const Student students[], int count);
static void searchStudent(const Student students[], int count);
static void updateGrades(Student students[], int count);
static void removeStudent(Student students[], int *count);
static void showTopStudent(const Student students[], int count);
static void showClassSummary(const Student students[], int count);

int main(void) {
    Student students[MAX_STUDENTS];
    int count = 0;
    for (;;) {
        int choice;
        puts("\n=== Student Grade Manager ===");
        puts("1. Add student\n2. List students\n3. Search by ID\n4. Show top student");
        puts("5. Update student grades\n6. Remove student\n7. Show class summary\n8. Exit");
        InputResult result = readInt("Choose an option: ", &choice);
        if (result == INPUT_EOF) { puts("\nGoodbye!"); break; }
        if (result == INPUT_INVALID) { puts("Please enter a valid whole number."); continue; }
        switch (choice) {
            case 1: addStudent(students, &count); break;
            case 2: listStudents(students, count); break;
            case 3: searchStudent(students, count); break;
            case 4: showTopStudent(students, count); break;
            case 5: updateGrades(students, count); break;
            case 6: removeStudent(students, &count); break;
            case 7: showClassSummary(students, count); break;
            case 8: puts("Goodbye!"); return 0;
            default: puts("Choose an option from 1 to 8."); break;
        }
    }
    return 0;
}

/* Reads a complete line, discarding excess characters so they cannot affect a later prompt. */
static InputResult readLine(const char *prompt, char *buffer, size_t size) {
    size_t length;
    int ch;
    fputs(prompt, stdout);
    fflush(stdout);
    if (fgets(buffer, (int)size, stdin) == NULL) return INPUT_EOF;
    length = strcspn(buffer, "\r\n");
    if (buffer[length] == '\0') {
        ch = getchar();
        if (ch != '\n' && ch != EOF) {
            while ((ch = getchar()) != '\n' && ch != EOF) { }
            return INPUT_INVALID;
        }
    } else buffer[length] = '\0';
    return INPUT_OK;
}

static InputResult readInt(const char *prompt, int *value) {
    char line[INPUT_LENGTH], *end;
    long parsed;
    InputResult result = readLine(prompt, line, sizeof line);
    if (result != INPUT_OK) return result;
    errno = 0;
    parsed = strtol(line, &end, 10);
    while (isspace((unsigned char)*end)) end++;
    if (end == line || *end != '\0' || errno == ERANGE || parsed < INT_MIN || parsed > INT_MAX)
        return INPUT_INVALID;
    *value = (int)parsed;
    return INPUT_OK;
}

static InputResult readName(char *name, size_t size) {
    char line[INPUT_LENGTH], *start;
    size_t length;
    InputResult result = readLine("Enter student name: ", line, sizeof line);
    if (result != INPUT_OK) return result;
    start = line;
    while (isspace((unsigned char)*start)) start++;
    length = strlen(start);
    while (length > 0 && isspace((unsigned char)start[length - 1])) start[--length] = '\0';
    if (length == 0 || length >= size) return INPUT_INVALID;
    memcpy(name, start, length + 1);
    return INPUT_OK;
}

static InputResult readGrade(int subject, float *grade) {
    char prompt[80], line[INPUT_LENGTH], *end;
    float parsed;
    snprintf(prompt, sizeof prompt, "Grade for subject %d (0-100): ", subject + 1);
    InputResult result = readLine(prompt, line, sizeof line);
    if (result != INPUT_OK) return result;
    errno = 0;
    parsed = strtof(line, &end);
    while (isspace((unsigned char)*end)) end++;
    if (end == line || *end != '\0' || errno == ERANGE || !isfinite(parsed) || parsed < 0.0f || parsed > 100.0f)
        return INPUT_INVALID;
    *grade = parsed;
    return INPUT_OK;
}

static int findStudentById(const Student students[], int count, int id) {
    for (int i = 0; i < count; i++) if (students[i].id == id) return i;
    return -1;
}

static float calculateAverage(const Student *student) {
    float total = 0.0f;
    for (int i = 0; i < SUBJECT_COUNT; i++) total += student->grades[i];
    return total / SUBJECT_COUNT;
}

static void printStudent(const Student *student) {
    printf("ID: %d | Name: %s | Grades: ", student->id, student->name);
    for (int i = 0; i < SUBJECT_COUNT; i++) printf("%.1f%s", student->grades[i], i + 1 == SUBJECT_COUNT ? "" : ", ");
    printf(" | Average: %.2f\n", calculateAverage(student));
}

static void addStudent(Student students[], int *count) {
    Student candidate;
    if (*count >= MAX_STUDENTS) { puts("The student list is full."); return; }
    for (;;) {
        InputResult result = readInt("Enter a positive student ID: ", &candidate.id);
        if (result == INPUT_EOF) return;
        if (result == INPUT_INVALID) puts("Enter a valid whole number.");
        else if (candidate.id <= 0) puts("ID must be positive.");
        else if (findStudentById(students, *count, candidate.id) >= 0) puts("That ID is already in use.");
        else break;
    }
    for (;;) {
        InputResult result = readName(candidate.name, sizeof candidate.name);
        if (result == INPUT_EOF) return;
        if (result == INPUT_INVALID) printf("Enter a non-blank name up to %d characters.\n", NAME_LENGTH - 1);
        else break;
    }
    for (int i = 0; i < SUBJECT_COUNT; i++) {
        for (;;) {
            InputResult result = readGrade(i, &candidate.grades[i]);
            if (result == INPUT_EOF) return;
            if (result == INPUT_INVALID) puts("Enter a finite grade from 0 to 100.");
            else break;
        }
    }
    students[(*count)++] = candidate;
    puts("Student added.");
}

static void listStudents(const Student students[], int count) {
    if (count == 0) { puts("No students have been added yet."); return; }
    puts("\n--- Student List ---");
    for (int i = 0; i < count; i++) printStudent(&students[i]);
}

static void searchStudent(const Student students[], int count) {
    int id;
    InputResult result = readInt("Enter the ID to search for: ", &id);
    if (result == INPUT_EOF) return;
    if (result == INPUT_INVALID) { puts("Enter a valid whole number."); return; }
    int index = findStudentById(students, count, id);
    if (index < 0) puts("Student not found."); else printStudent(&students[index]);
}

static void updateGrades(Student students[], int count) {
    int id;
    InputResult result = readInt("Enter the ID to update: ", &id);
    if (result == INPUT_EOF) return;
    if (result == INPUT_INVALID) { puts("Enter a valid whole number."); return; }
    int index = findStudentById(students, count, id);
    if (index < 0) { puts("Student not found."); return; }
    Student updated = students[index]; /* Commit only after every new grade is valid. */
    printf("Updating grades for %s.\n", updated.name);
    for (int i = 0; i < SUBJECT_COUNT; i++) {
        for (;;) {
            result = readGrade(i, &updated.grades[i]);
            if (result == INPUT_EOF) return;
            if (result == INPUT_INVALID) puts("Enter a finite grade from 0 to 100.");
            else break;
        }
    }
    students[index] = updated;
    puts("Grades updated.");
}

static void removeStudent(Student students[], int *count) {
    int id;
    InputResult result = readInt("Enter the ID to remove: ", &id);
    if (result == INPUT_EOF) return;
    if (result == INPUT_INVALID) { puts("Enter a valid whole number."); return; }
    int index = findStudentById(students, *count, id);
    if (index < 0) { puts("Student not found."); return; }
    for (int i = index; i < *count - 1; i++) students[i] = students[i + 1];
    (*count)--;
    puts("Student removed.");
}

static void showTopStudent(const Student students[], int count) {
    if (count == 0) { puts("No students to compare."); return; }
    int topIndex = 0;
    for (int i = 1; i < count; i++)
        if (calculateAverage(&students[i]) > calculateAverage(&students[topIndex])) topIndex = i;
    puts("Top student:");
    printStudent(&students[topIndex]);
}

static void showClassSummary(const Student students[], int count) {
    if (count == 0) { puts("No students to summarize."); return; }
    float total = 0.0f;
    for (int i = 0; i < count; i++) total += calculateAverage(&students[i]);
    printf("Students: %d | Class average: %.2f\n", count, total / count);
}