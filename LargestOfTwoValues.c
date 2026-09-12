#include<stdio.h>
int main(void){
    int val1,val2,max;
    scanf("%d%d",&val1,&val2);
    if(val1>val2)
    max = val1;
else
    max = val2;
printf("The largest value is %d\n",max);
return 0;
}