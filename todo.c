#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 5
#define SUBJECTS 3

// Global variable: shared across functions
int studentsAdded = 0;

// Function declarations
void showMenu(void);
void addStudent(char names[][50], int marks[][SUBJECTS]);
void showAllStudents(char names[][50], int marks[][SUBJECTS]);
void showStudentAverage(char names[][50], int marks[][SUBJECTS]);
void showClassAverage(int marks[][SUBJECTS]);
int calculateTotal(int marks[]);
float calculateAverage(int marks[]);
char getGrade(float average);

int main(void)
{
    char names[MAX_STUDENTS][50];
    int marks[MAX_STUDENTS][SUBJECTS];
    int choice;

    // TODO: Initialize studentsAdded to 0 if needed.

    do
    {
        showMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                addStudent(names, marks);
                break;

            case 2:
                showAllStudents(names, marks);
                break;

            case 3:
                showStudentAverage(names, marks);
                break;

            case 4:
                showClassAverage(marks);
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

void showMenu(void)
{
    printf("\n===== Student Marks Menu =====\n");
    printf("1. Add a student's marks\n");
    printf("2. Show all students\n");
    printf("3. Show one student's average and grade\n");
    printf("4. Show class average\n");
    printf("5. Exit\n");
}

void addStudent(char names[][50], int marks[][SUBJECTS])
{
    if (studentsAdded >= MAX_STUDENTS)
    {
        printf("The student list is full.\n");
        return;
    }

    printf("Enter student name (one word): ");
    scanf("%49s", names[studentsAdded]);

    for (int subject = 0; subject < SUBJECTS; subject++)
    {
        printf("Enter marks for subject %d (0-100): ", subject + 1);
        scanf("%d", &marks[studentsAdded][subject]);

        // TODO: Keep asking until the mark is between 0 and 100.
    }

    studentsAdded++;
    printf("Student added.\n");
}

void showAllStudents(char names[][50], int marks[][SUBJECTS])
{
    if (studentsAdded == 0)
    {
        printf("No students have been added yet.\n");
        return;
    }

    printf("\n--- Student List ---\n");

    for (int i = 0; i < studentsAdded; i++)
    {
        int total = calculateTotal(marks[i]);
        float average = calculateAverage(marks[i]);

        printf("%d. %s | Total: %d | Average: %.2f\n",
               i + 1, names[i], total, average);
    }
}

void showStudentAverage(char names[][50], int marks[][SUBJECTS])
{
    if (studentsAdded == 0)
    {
        printf("Add a student first.\n");
        return;
    }

    int studentNumber;
    printf("Enter student number (1-%d): ", studentsAdded);
    scanf("%d", &studentNumber);

    // TODO: Check whether studentNumber is valid.
    // If it isn't, print a message and return.

    int index = studentNumber - 1;
    float average = calculateAverage(marks[index]);
    char grade = getGrade(average);

    printf("%s's average: %.2f\n", names[index], average);
    printf("Grade: %c\n", grade);
}

void showClassAverage(int marks[][SUBJECTS])
{
    if (studentsAdded == 0)
    {
        printf("Add a student first.\n");
        return;
    }

    float sumOfAverages = 0;

    for (int i = 0; i < studentsAdded; i++)
    {
        sumOfAverages += calculateAverage(marks[i]);
    }

    // TODO: Calculate and print the class average.
}

int calculateTotal(int marks[])
{
    int total = 0;

    for (int i = 0; i < SUBJECTS; i++)
    {
        total += marks[i];
    }

    return total;
}

float calculateAverage(int marks[])
{
    int total = calculateTotal(marks);

    // TODO: Return the average as a float.
}

char getGrade(float average)
{
    // TODO: Use if / else if / else to return a grade:
    // 90 or more: A
    // 80 or more: B
    // 70 or more: C
    // 60 or more: D
    // below 60: F
}