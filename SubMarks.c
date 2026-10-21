#include <stdio.h>

int main(void) {
    double marks[5][4] = {
        {10, 23, 31.1, 11},
        {20, 43.5, 21, 21},
        {12, 22.6, 30, 13},
        {30.3, 31, 26, 41.4},
        {13, 3, 41, 15}
    };

    for (int student = 0; student < 5; student++) {
        double total = 0;

        for (int subject = 0; subject < 4; subject++) {
            total += marks[student][subject];
        }

        printf("Student %d average: %.3f\n", student + 1, total / 4);
    }

    return 0;
}