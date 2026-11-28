#include<stdio.h>
int main(){
    FILE *fp = fopen("file.txt", "w+");
    char str[] = "DA 104:C programming";
    char buffer[30];
    fwrite(str , sizeof(str), 1 , fp);
    fputs("\n" , fp);
    fseek(fp , 0 , SEEK_SET);
    fread(buffer,sizeof(str),1,fp);
    printf("%s", buffer);
    fclose(fp);
    return(0); 
}