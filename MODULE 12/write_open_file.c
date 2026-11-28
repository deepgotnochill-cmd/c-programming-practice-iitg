#include<stdio.h>
int main(){
    int a = 5 , b = 20;
    FILE *fp;
    // wfp = fopen("results", "w");
    // fprintf(wfp, "%d %d\n", a , b);
    fp = fopen("file.txt", "w+");
    fputs("This is MA511.", fp);
}