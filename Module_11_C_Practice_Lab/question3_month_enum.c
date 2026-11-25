#include <stdio.h>

enum Month {
    JANUARY = 1, FEBRUARY, MARCH, APRIL, MAY, JUNE,
    JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER
};

int main(void) {
    enum Month month;
    int monthNumber, year, days;

    printf("Enter month number (1-12): ");
    scanf("%d", &monthNumber);
    if (monthNumber < 1 || monthNumber > 12) {
        printf("Invalid month number!\n");
        return 1;
    }

    month = (enum Month)monthNumber;
    printf("Enter year: ");
    scanf("%d", &year);

    switch (month) {
        case APRIL:
        case JUNE:
        case SEPTEMBER:
        case NOVEMBER:
            days = 30;
            break;
        case FEBRUARY:
            days = (year % 400 == 0 ||
                    (year % 4 == 0 && year % 100 != 0)) ? 29 : 28;
            break;
        default:
            days = 31;
    }

    printf("Number of days = %d\n", days);
    return 0;
}
