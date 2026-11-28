#include<stdio.h>
int main(){
    FILE *fp;
    fp = fopen("file.txt" , "w+");
    char string[] = "Testing fputc";
    for(int i = 0 ; string[i] != '\0'; i++)
    fputc(string[i], fp);
    fclose(fp);
}