#include<stdio.h>
int main(void)
{
    int num;
    printf("Enter a number: ");
    scanf("%d", &num);
    if(num > 0)
    {
        prinf("Positive\n");
        if (num % 2 == 0)
        {
            printf("Even\n");
        }
        else
        {
            printf("Odd\n");
        }
    }
    else if(num < 0)
    {
        printf("Negative\n");
        if(num%2 == 0)
        {
            printf("Even\n");
        }
        else
        {
            printf("Odd\n");
        }
    }
    else
    {
        printf("Zero\n");
    }
    return 0;
}