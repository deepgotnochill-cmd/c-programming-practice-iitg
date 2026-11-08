#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, new_n;
    printf("Enter initial number of floating point elements: ");
    scanf("%d", &n);

    float *arr = (float *)malloc(n * sizeof(float));
    if (arr == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("Enter %d numbers:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%f", &arr[i]);
    }

    printf("Enter the new expanded size of the array: ");
    scanf("%d", &new_n);

    // Reallocate memory to accommodate more elements
    arr = (float *)realloc(arr, new_n * sizeof(float));
    if (arr == NULL) {
        printf("Memory reallocation failed.\n");
        return 1;
    }

    printf("Enter the additional %d numbers:\n", new_n - n);
    for (int i = n; i < new_n; i++) {
        scanf("%f", &arr[i]);
    }

    printf("All elements in the expanded array:\n");
    for (int i = 0; i < new_n; i++) {
        printf("%.2f ", arr[i]);
    }
    printf("\n");

    free(arr);
    return 0;
}
  