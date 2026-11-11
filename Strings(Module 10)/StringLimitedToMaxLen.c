#include<stdio.h>
#include<string.h>
int main(){
    char str1[20] = "Programming";
    printf("Length of stringh str1 : %lu",strnlen(str1,30));
    printf("Length of stringh str1 : %lu",strnlen(str1,7));
    return 0;  
} 