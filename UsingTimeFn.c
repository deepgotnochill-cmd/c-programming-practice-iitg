#include<stdio.h>
#include<time.h>
int main(){
    time_t seconds;
    printf("Seconds since January 1 , 1970 = %ld\n",time(0));
    return (0);
}