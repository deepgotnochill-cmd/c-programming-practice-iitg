#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Student{
    char firstName[50];
    char lastName[50];
};
int main(){
    FILE *fp;
    struct Student s;
    fp = fopen("students.txt", "w+");
    if(fp == NULL){
        printf("Error opening file!\n") ;
        return 1;
    }
    printf("Enter student records (type 'end' as first name to stop):\n");
    while(1){
        printf("First name : ");
        scanf("%49s" , s.firstName);
        if(strcmp(s.firstName, "end") == 0){
            break;
        }
        printf("Last name: ");
        scanf("%49s", s.lastName);
        fprintf(fp , "%s %s\n", s.firstName , s.lastName);
    }
    rewind(fp);
    printf("\n--- Stored Student Records ---\n");
    while(fscanf(fp, "%49s %49s", s.firstName , s.lastName) == 2){
        printf("First Name: %s , Last Name: %s\n" , s.firstName , s.lastName);
    }
    fclose(fp);
    return 0;
}
