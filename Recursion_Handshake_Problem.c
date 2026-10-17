#include<stdio.h>
int h(int);
int main(){
    int n;
    printf("Enter no. of persons: ");
    scanf("%d", &n);
    printf("Total handshakes: %d\n",h(n));
    return 0;
}
int h(int n){
    if(n==0 || n ==1)
    return 0;
else
return(n - 1) + h(n-1);
}