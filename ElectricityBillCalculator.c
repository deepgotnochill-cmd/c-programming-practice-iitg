#include <stdio.h>
int main(void){
    float units,bill;
    printf("Enter electricity units consumed: ");
    scanf("%f", &units);
    if(units <= 100)
    {
        bill = units * 2;
    }
    else if (units <= 200)
    {
        bill = (100 * 2)+((units - 100)*3);
    }
    else if(units <= 300)
    {
        bill = (100 * 2) + (100 * 3) + ((units - 200) * 5);
    }
    else
    {
        bill = (100 * 2) + (100 * 3) + (100 * 5) + ((units - 300) * 7);
    }
    printf("Electricity Bill = Rs. %.2f\n", bill);
    return 0;
}