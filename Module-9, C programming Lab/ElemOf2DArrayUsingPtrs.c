#include <stdio.h>

int main() {
    // Initialize a 2x3 array
    int arr[2][3] = {{1, 2, 3}, {4, 5, 6}};
    
    printf("Array elements using pointers:\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            // *(*(arr + i) + j) is equivalent to arr[i][j]
            printf("%d ", *(*(arr + i) + j));
        }
        printf("\n");
    }
    return 0;
}
