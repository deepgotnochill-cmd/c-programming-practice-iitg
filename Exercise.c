#include <stdio.h>

int main() {
    // 1. Declare an integer, floating point and void pointer
    int *i;
    float *f;
    void *v;

    int num = 10;
    float fnum = 3.14;

    i = &num;       // integer pointer points to an int
    f = &fnum;      // float pointer points to a float

    // 2. Assign the integer pointer to the void pointer
    v = i;          // Legal (void* can hold any pointer)

    // 3. Dereference the void pointer
    // Must typecast before dereferencing
    printf("%d\n", *(int *)v);     // Output: 10

    // 4. Assign the floating point pointer to the void pointer
    v = f;          // Legal

    // 5. Assign the void pointer to the integer pointer
    i = (int *)v;   // Need explicit cast (void* → int*)

    // Note: Now i points to a float, which is dangerous
    // printf("%d\n", *i);  // Undefined behavior

    return 0;
}