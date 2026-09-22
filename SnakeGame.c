#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>

int main() {

    int x = 10, y = 10;
    int foodX = 5, foodY = 5;
    int score = 0;
    char key;

    while (1) {

        system("cls");

        for (int i = 0; i < 20; i++) {

            for (int j = 0; j < 30; j++) {

                if (i == 0 || i == 19 || j == 0 || j == 29)
                    printf("#");

                else if (i == y && j == x)
                    printf("O");

                else if (i == foodY && j == foodX)
                    printf("*");

                else
                    printf(" ");
            }

            printf("\n");
        }

        printf("Score: %d\n", score);
        printf("Use W A S D to move | X to exit\n");

        if (_kbhit()) {

            key = _getch();

            if (key == 'w')
                y--;

            if (key == 's')
                y++;

            if (key == 'a')
                x--;

            if (key == 'd')
                x++;

            if (key == 'x')
                break;
        }

        if (x == foodX && y == foodY) {

            score++;

            foodX = rand() % 28 + 1;
            foodY = rand() % 18 + 1;
        }

        if (x <= 0 || x >= 29 || y <= 0 || y >= 19) {

            printf("\nGame Over!\n");
            printf("Final Score: %d\n", score);

            break;
        }

        Sleep(150);
    }

    return 0;
}