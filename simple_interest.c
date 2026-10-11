#include <stdio.h>

// Function to calculate simple interest
float simpleInterest(float p, float r, float t) {
    return (p * r * t) / 100;
}

int main() {
    float principal, rate, time, interest;

    // Take input from user
    printf("Enter Principal amount: ");
    scanf("%f", &principal);

    printf("Enter Rate of interest: ");
    scanf("%f", &rate);

    printf("Enter Time (in years): ");
    scanf("%f", &time);

    // Call the function
    interest = simpleInterest(principal, rate, time);

    // Display result
    printf("Simple Interest = %.2f\n", interest);

    return 0;
}