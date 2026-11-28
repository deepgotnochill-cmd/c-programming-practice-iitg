#include<stdio.h>
int main(){
    char buf[20];
    fscanf(stdin, "%s", buf);
    fprintf(stdout, "%s" , buf);
    fprintf(stderr, "%s" , "Error");
}