#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, i;
    int *arr;

    // 1. Allocate an integer array of size 3 initialized to zero
    arr = (int *)calloc(3, sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // 2. Take n (>3) as user input
    printf("Enter an integer greater than 3: ");
    scanf("%d", &n);

    // Basic validation to ensure n > 3
    if (n <= 3) {
        printf("Invalid input. Please enter a number greater than 3.\n");
        free(arr);
        return 1;
    }

    // 3. Expand the array to size n using realloc()
    int *temp = (int *)realloc(arr, n * sizeof(int));
    if (temp == NULL) {
        printf("Memory reallocation failed!\n");
        free(arr);
        return 1;
    }
    arr = temp;

    // 4. Populate the array starting from the 4th (index 3) to the nth element (index n-1)
    for (i = 3; i < n; i++) {
        printf("Enter the %d element: ", i + 1);
        scanf("%d", &arr[i]);
    }

    // 5. Print the entire array
    printf("The array elements are: ");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // 6. Free the dynamically allocated memory
    free(arr);

    return 0;
}
