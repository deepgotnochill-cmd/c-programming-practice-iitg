#include <stdio.h>
#include <stdlib.h>

void reverse(int n) {
    // Allocate memory for character array
    char *arr = (char *)malloc(n * sizeof(char));
    if (arr == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("Enter %d characters: ", n);
    getchar(); // Clear the buffer newline character left by scanf
    
    for (int i = 0; i < n; i++) {
        scanf("%c", &arr[i]);
    }

    printf("Characters in reverse order: ");
    for (int i = n - 1; i >= 0; i--) {
        printf("%c", arr[i]);
    }
    printf("\n");

    free(arr);
}

int main() {
    int n;
    printf("Enter an integer n: ");
    scanf("%d", &n);

    reverse(n);

    return 0;
}
