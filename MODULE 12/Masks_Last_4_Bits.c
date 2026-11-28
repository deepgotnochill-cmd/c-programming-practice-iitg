#include<stdio.h>
int main(){
    int num = 53;
    int last4bits;
    int mask = 1;
    for(int i = 1; i <= 3 ; i++)
    mask = mask ^ 1 <<i;
last4bits = num & mask;
printf("%d\n", last4bits);
return 0;
}