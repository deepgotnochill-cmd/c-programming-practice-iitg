#include <stdio.h>

void add_one_copy(int number)
{
    number++;
    printf("Inside add_one_copy: %d\n", number);
}

void add_one_through_pointer(int *number_ptr)
{
    (*number_ptr)++;
}

void swap_ints(int *left_ptr, int *right_ptr)
{
    int temporary = *left_ptr;
    *left_ptr = *right_ptr;
    *right_ptr = temporary;
}

int sum_array(const int *values, int length)
{
    int total = 0;

    for (int i = 0; i < length; i++) {
        total += values[i];
    }

    return total;
}

int main(void)
{
    int score = 42;
    int *score_ptr = &score;

    printf("score = %d\n", score);
    printf("address of score = %p\n", (void *)&score);
    printf("value through pointer = %d\n", *score_ptr);

    *score_ptr = 50;
    printf("score after changing it through the pointer = %d\n\n", score);

    int count = 7;

    add_one_copy(count);
    printf("count after add_one_copy = %d\n", count);

    add_one_through_pointer(&count);
    printf("count after add_one_through_pointer = %d\n\n", count);

    int first = 3;
    int second = 9;

    printf("Before swap: first = %d, second = %d\n", first, second);
    swap_ints(&first, &second);
    printf("After swap:  first = %d, second = %d\n\n", first, second);

    int values[] = {18, 21, 19, 24};
    int length = (int)(sizeof values / sizeof values[0]);

    printf("Sum of array values = %d\n", sum_array(values, length));

    return 0;
}