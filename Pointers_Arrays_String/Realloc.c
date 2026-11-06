#include<stdio.h>
#include<stdlib.h>
int main(){
    int *ptr;
    int i;
    ptr = calloc(5,sizeof(int));
    if(ptr!=NULL){
        *ptr = 1;
        *(ptr+1) = 2;
        ptr[2] = 4;
        ptr[3] = 8;
        ptr[4] = 16;
        ptr = realloc(ptr , 7*sizeof(int));
        if(ptr != NULL){
            printf("Now allocating more memory... \n");
            ptr[5] = 32;
            ptr[6] = 64;
            for(i = 0 ; i < 7 ; i++){
                printf("ptr[%d] holds %d\n",i,ptr[i]);
            }
            realloc(ptr,0); 
            return 0;
        }
    }
}