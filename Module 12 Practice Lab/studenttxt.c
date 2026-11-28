#include<stdio.h>
#include<stdlib.h>
int main(){
    FILE *fp;
    char firstName[50], lastName[50];
    int n , i;
    fp = fopen("students.txt", "w");
    if(fp == NULL){
        printf("Error opening file!\n");
        return 1;
    }
    printf("Enter the number of students: ");
    scanf("%d" , &n);
    for(i = 0 ; i < n ; i++){
        printf("Enter first name and last name for student: ", i + 1);
        scanf("%s %s" , firstName , lastName);
        fprintf(fp , " %s %s\n" , firstName , lastName);
    }
    fclose(fp);
    fp = fopen("students.txt" , "r");
    if(fp == NULL){
        printf("Error opening file!\n");
        return 1;
    }
    printf("\n--- Student Records ---\n");
    while(fscanf(fp , "%s %s" , firstName , lastName) != EOF){
        printf("Name: %s %s\n", firstName , lastName);
    }
    fclose(fp);
    return 0;
}