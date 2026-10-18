#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "myheader.h"

int one = 0, two = 0, three = 0;
int four = 0, five = 0, six = 0;

int main(void) {
    srand((unsigned)time(NULL));

    for (int i = 0; i < 60; i++) {
        int roll = roll_die();

        switch (roll) {
            case 1: one++;   break;
            case 2: two++;   break;
            case 3: three++; break;
            case 4: four++;  break;
            case 5: five++;  break;
            case 6: six++;   break;
        }
    }

    printf("One = %d\n", one);
    printf("Two = %d\n", two);
    printf("Three = %d\n", three);
    printf("Four = %d\n", four);
    printf("Five = %d\n", five);
    printf("Six = %d\n", six);

    return 0;
}