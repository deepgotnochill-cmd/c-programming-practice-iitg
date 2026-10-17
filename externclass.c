#include<stdio.h>
int a = 1 , b = 2 , c = 3;
int f(void);
int main(void){
    printf("%d", f());
    printf("%d,%d,%d",a,b,c);
    return 0;
}
int f(void){
    int b , c;
    a = b = c = 4;
    return(a+b+c);
}