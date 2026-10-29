#include <stdio.h>

int main(void) {
    char team1[50], team2[50];
    int score1, score2;

    printf("Enter the first team: ");
    scanf("%49s", team1);

    printf("Enter the second team: ");
    scanf("%49s", team2);

    printf("Enter %s's score: ", team1);
    scanf("%d", &score1);

    printf("Enter %s's score: ", team2);
    scanf("%d", &score2);

    printf("\nFinal score: %s %d - %d %s\n",
           team1, score1, score2, team2);

    if (score1 > score2) {
        printf("%s wins!\n", team1);
    } else if (score2 > score1) {
        printf("%s wins!\n", team2);
    } else {
        printf("It's a draw!\n");
    }

    return 0;
}