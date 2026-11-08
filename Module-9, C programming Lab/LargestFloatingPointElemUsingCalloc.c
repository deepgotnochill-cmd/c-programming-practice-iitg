#include <stdio.h>
#include <stdlib.h>

int main() {
    int n;
    printf("Enter number of floating point elements: ");
    scanf("%d", &n);

    // Allocate memory using calloc
    float *arr = (float *)calloc(n, sizeof(float));
    if (arr == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("Enter %d numbers:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%f", &arr[i]);
    }

    float largest = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > largest) {
            largest = arr[i];
        }
    }

    printf("The largest element is: %.2f\n", largest);

    free(arr);
    return 0;
}
