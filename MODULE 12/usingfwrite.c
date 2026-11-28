#include<stdio.h>
int main(){
    FILE *fp = fopen("file.txt", "w");
    char str[] = "DA 104:C programming";
    fwrite(str,sizeof(str),1,fp);
    fputs("\n", fp);
    fclose(fp);
    return(0);
}