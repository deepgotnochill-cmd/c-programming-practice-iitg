#include<stdio.h>
int f(void);
int main(void){
    printf("%d\n",f());
    printf("%d\n",f());
    printf("%d\n",f());
    return 0;
}
int f(void){
    static int cnt = 0;
    ++cnt;
    return(cnt);
}