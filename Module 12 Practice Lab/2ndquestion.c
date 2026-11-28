#include<stdio.h>
#include<ctype.h>
int main(){
    FILE *fp;
    char ch;
    int characters = 0, words = 0 , inWord = 0;
    fp = fopen("words.txt" , "r");
    if(fp == NULL){
        printf("Could not open words.txt\n");
        return 1;
    }
    while((ch = fgetc(fp)) != EOF){
        characters++;
        if(isspace(ch)){
            inWord = 0;
        } else if (inWord == 0){
            inWord = 1;
            words++;
        }
    }
    fclose(fp);
    printf("Total Characters: %d\n", characters);
    printf("Total Words: %d\n" , words);
    return 0;
}