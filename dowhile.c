#include<stdio.h>
int main(){
    int number;
    do{
        printf("Enter a number: ");
        scanf("%d", &number);
    }while(number < 0);
    printf*"You entered: a positive number\n");
    return 0;
}