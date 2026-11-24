#include <stdio.h>
int main(){
   int r;
   printf("Enter radius : ");
   scanf("%d" , &r);
   int h;
   printf("Enter height : ");
   scanf("%d" , &h);
   printf("The area of circle with radius %d is %f\n", r , 3.14 * r * r);
   printf("The volume of cylinder with radius %d and height %d is %f" , r , h , 3.14 * r * r * h);
   return 0;
}