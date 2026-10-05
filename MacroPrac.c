#include<stdio.h>
#define SQUARE(x) ((x) * (x))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
inr main(void){
    int number = 17;
    int other = 8;
    printf("Square of %d is %d\n",number,SQUARE(number));
    printf("Larger number is %d\n", MAX(number, other));
    return 0;
}