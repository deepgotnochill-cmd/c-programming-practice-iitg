#include <stdio.h>

struct student {
    char name[30];
    int roll_number;
};

int main(void) {
    struct student stud1={"Alice",100};
    struct student stud2={"Deep",17};
    printf("Name: %s\n",stud1.name);
    printf("Roll no: %s\n",stud1.roll_number);

    return 0;
}