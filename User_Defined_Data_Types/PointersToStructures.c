#include <stdio.h>

struct student {
    char *last_name;
    int student_id;
    char grade;
};

int main(void) {
    struct student tmp;
    struct student *p = &tmp;

    tmp.grade = 'A';
    tmp.last_name = "Casanov";
    tmp.student_id = 910017;

    printf("Last name: %s\n", p->last_name);
    printf("Student ID: %d\n", p->student_id);
    printf("Grade: %c\n", p->grade);

    return 0;
}