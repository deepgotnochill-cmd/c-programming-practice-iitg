#include<stdio.h>
int main(){
    FILE *fp = fopen("file.txt", "w");
    char str[] = {'D','A','1','0','4'};
    fwrite(str,sizeof(char), 2 , fp);
    fputs("\n", fp);
    fclose(fp);
    return(0);
}