#include<stdio.h>
#include<ctype.h>
int main(){
    FILE *fp;
    char buffer[10000];
    int i = 0;
    char ch;
    fp = fopen("UpperLower.txt", "r");
    if(fp == NULL){
        printf("Could not open UpperLower.txt\n");
        return 1;
    }
    while((ch = fgetc(fp)) != EOF && i < 9999){
        if(islower(ch)){
            buffer[i++] = toupper(ch);
        } else if (isupper(ch)){
            buffer[i++] = tolower(ch);
        }else {
            buffer[i++] = ch;
        }
    }
    buffer[i] = '\0';
    fclose(fp);
    fp = fopen("UpperLower.txt", "w");
    if(fp == NULL){
        printf("Error writing to UppwerLower.txt\n");
        return 1;
    }
    fputs(buffer , fp);
    fclose(fp);
    printf("Case conversion complete.\n");
    return 0;
}