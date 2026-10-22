#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int read_number(int *number)
{
    char line[64];
    char extra;

    if (fgets(line, sizeof line, stdin) == NULL)
        return 0;

    return sscanf(line, " %d %c", number, &extra) == 1;
}

int main(void)
{
    int play_again = 1;

    srand((unsigned int)time(NULL));
    puts("=== Guess the Number ===");
    puts("I'm thinking of a number from 1 to 100. Can you guess it?");

    while (play_again) {
        const int secret = rand() % 100 + 1;
        int guesses = 0;
        int guess;

        for (;;) {
            printf("Your guess: ");
            if (!read_number(&guess)) {
                puts("Please enter a whole number.");
                if (feof(stdin))
                    return 0;
                continue;
            }

            if (guess < 1 || guess > 100) {
                puts("Pick a number between 1 and 100.");
                continue;
            }

            ++guesses;
            if (guess < secret)
                puts("Too low!");
            else if (guess > secret)
                puts("Too high!");
            else {
                printf("You got it in %d guess%s!\n", guesses,
                       guesses == 1 ? "" : "es");
                break;
            }
        }

        printf("Play again? (1 = yes, 0 = no): ");
        if (!read_number(&play_again))
            break;
        if (play_again != 0)
            play_again = 1;
    }

    puts("Thanks for playing!");
    return 0;
}
