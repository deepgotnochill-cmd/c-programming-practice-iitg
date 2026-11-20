#include <stdio.h>
#include <string.h>

/* Define a union: all members share the same memory */
union Data {
    int integer_value;
    float float_value;
    char text[20];
};

/* User-defined function: receives a union and returns it */
union Data makeInteger(int value) {
    union Data data;
    data.integer_value = value;
    return data;
}

/* User-defined function: receives a pointer to a union and changes it */
void setFloat(union Data *data, float value) {
    data->float_value = value;
}

/* User-defined function: accepts a union and displays its integer member */
void printInteger(union Data data) {
    printf("Integer value: %d\n", data.integer_value);
}

/* User-defined function: accepts a pointer and displays its float member */
void printFloat(const union Data *data) {
    printf("Float value: %.2f\n", data->float_value);
}

/* User-defined function: stores text in the union */
void setText(union Data *data, const char *text) {
    snprintf(data->text, sizeof(data->text), "%s", text);
}

/* User-defined function: displays the text member */
void printText(const union Data *data) {
    printf("Text value: %s\n", data->text);
}

int main(void) {
    union Data data;

    /* Function returns a union */
    data = makeInteger(42);
    printInteger(data);

    /* Function changes the union through a pointer */
    setFloat(&data, 3.14f);
    printFloat(&data);

    /* Writing text replaces the float stored in the shared memory */
    setText(&data, "Hello, union");
    printText(&data);

    return 0;
}