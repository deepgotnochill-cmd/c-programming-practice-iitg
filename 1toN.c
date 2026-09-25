#include<stdio.h>
int main(){
    int n,cnt=1;
    scanf("%d", &n);
    while(n>0){
        printf("%d\n",cnt);
        ++cnt;
        --n;
    }
    return 0;
}