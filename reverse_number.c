int reverseNumber(int n) {
    int reversed = 0;
    int sign = 1;

    // Handle negative numbers
    if (n < 0) {
        sign = -1;
        n = -n;
    }

    while (n > 0) {
        int digit = n % 10;
        reversed = reversed * 10 + digit;
        n = n / 10;
    }

    return reversed * sign;
}