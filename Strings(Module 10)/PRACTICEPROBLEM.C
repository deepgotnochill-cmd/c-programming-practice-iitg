#include <stdio.h>
#include <stdlib.h>

int main() {
    const char *s = "3.14159";
    char *endptr;
    double value = strtod(s, &endptr);
    
    if (endptr == s) {
        printf("No conversion performed.\n");
    } else {
        printf("Converted value: %lf\n", value);
        printf("Characters after conversion: %s\n", endptr);
    }
    
    return 0;
}
