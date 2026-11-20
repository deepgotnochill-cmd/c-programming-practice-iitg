#include <stdio.h>

/* Define an enumeration */
enum TrafficLight {
    RED,
    YELLOW,
    GREEN
};

/* Function that returns an enum value */
enum TrafficLight getNextLight(enum TrafficLight current) {
    switch (current) {
        case RED:
            return GREEN;
        case GREEN:
            return YELLOW;
        case YELLOW:
            return RED;
        default:
            return RED;
    }
}

/* Function that displays an enum value */
void printLight(enum TrafficLight light) {
    switch (light) {
        case RED:
            printf("Red\n");
            break;
        case YELLOW:
            printf("Yellow\n");
            break;
        case GREEN:
            printf("Green\n");
            break;
        default:
            printf("Unknown light\n");
    }
}

int main(void) {
    enum TrafficLight current = RED;

    printf("Current light: ");
    printLight(current);

    current = getNextLight(current);
    printf("Next light: ");
    printLight(current);

    current = getNextLight(current);
    printf("Next light: ");
    printLight(current);

    return 0;
}