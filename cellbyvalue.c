#include<stdio.h>
int multiply(int, int);
int main(){
    int a , b , c ;
    a = 2;
    b = 5;
    c = multiply(a,b);
    a = multiply(b,c);
    return 0;
}
int multiply(int x , int y){
    x = x*y;
    return x;
}