#include <stdio.h>

int main() {
    int days, years, weeks, remaining_days;
    
    printf("Enter number of days: ");
    scanf("%d", &days);
    
    years = days / 365;
    weeks = (days % 365) / 7;
    remaining_days = days - (years * 365) - (weeks * 7);
    
    printf("Years: %dWeeks: %dDays: %d\n", years, weeks, remaining_days);
    
    return 0;
}