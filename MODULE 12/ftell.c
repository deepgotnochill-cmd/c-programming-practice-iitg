#include<stdio.h>
int main(){
    FILE* fp = fopen("test.txt", "r");
        char string[20];
        fscanf(fp, "%s", string);
        printf("%s\n",string);
        /* Printing position of file pointer */
        printf("%ld", ftell(fp));
        fclose(fp);
        return 0;
        
}