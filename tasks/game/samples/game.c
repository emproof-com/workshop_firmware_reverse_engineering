// gcc -O3 -o game game.c && strip game
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Hardcoded license key
static const char correct_license[] = "GAMEUNLOCK-2024";
int trial_uses = 3;

void play_game(int max_attempts) {
    int number, guess, attempts = 0;

    // Seed the random number generator
    srand(time(0));
    number = rand() % 100 + 1; // Random number between 1 and 100

    printf("Welcome to the number guessing game! Guess a number between 1 and 100.\n");

    while (attempts < max_attempts) {
        printf("Enter your guess: ");
        scanf("%d", &guess);
        attempts++;

        if (guess < number) {
            printf("Too low!\n");
        } else if (guess > number) {
            printf("Too high!\n");
        } else {
            printf("Congratulations! You've guessed the right number in %d attempts!\n", attempts);
            return;
        }

        if (attempts == max_attempts) {
            printf("Sorry, you've used all your attempts. The correct number was %d.\n", number);
        }
    }
}

__attribute__((noinline)) int check_license(char *input_license) {
    // Check if the input matches the correct license
    if (strcmp(input_license, correct_license) == 0) {
        return 1; // License is valid
    } else {
        return 0; // License is invalid
    }
}

int main() {
    char input_license[50];
    
    printf("Enter your license key (or type 'TRIAL' for trial mode with limited attempts): ");
    scanf("%49s", input_license);

    if (check_license(input_license)) {
        printf("Full access granted! You have unlimited attempts.\n\n");
        play_game(100); // Full version with 100 attempts
    } else if (strcmp(input_license, "TRIAL") == 0) {
        printf("Trial mode: You have only a limited number of attempts to guess the number.\n\n");
        play_game(trial_uses); // Trial version with 3 attempts
    } else {
        printf("Invalid license key. Access denied.\n");
    }

    return 0;
}
