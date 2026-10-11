#include <stdio.h>

int isPalindrome(int n) {
    int original = n;
    int rev = 0;

    while (n > 0) {
        rev = rev * 10 + n % 10;
        n = n / 10;
    }

    return (original == rev);
}

int main() {
    int num;
    printf("Enter a number: ");
    scanf("%d", &num);

    if (isPalindrome(num))
        printf("1\n");
    else
        printf("0\n");

    return 0;
}