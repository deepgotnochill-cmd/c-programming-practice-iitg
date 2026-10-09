#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main(){
    int rand_no;
    srand(time(0));
    rand_no = rand()%6+1;
    printf("Random no:%d\n",rand_no);
}