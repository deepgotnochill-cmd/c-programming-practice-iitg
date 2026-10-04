#include <stdio.h>

int main(void) {
    int iVal, type;
    float fVal, predicted, actual;

    printf("Enter an integer: ");
    scanf("%d", &iVal);
    printf("Enter a float: ");
    scanf("%f", &fVal);

    actual = iVal + fVal;
    printf("\nPredict for: int + float\n");
    printf("Enter predicted type code (1=int, 2=float, 3=double): ");
    scanf("%d", &type);
    printf("Enter predicted value: ");
    scanf("%f", &predicted);
    printf("Predicted type: %d, value: %.4f\n", type, predicted);
    printf("Actual type: 2 (float), value: %.4f -> %s\n",
           actual, predicted == actual ? "MATCH" : "NO MATCH");

    actual = iVal / fVal;
    printf("\nPredict for: int / float\n");
    printf("Enter predicted type code (1=int, 2=float, 3=double): ");
    scanf("%d", &type);
    printf("Enter predicted value: ");
    scanf("%f", &predicted);
    printf("Predicted type: %d, value: %.4f\n", type, predicted);
    printf("Actual type: 2 (float), value: %.4f -> %s\n",
           actual, predicted == actual ? "MATCH" : "NO MATCH");

    return 0;
}