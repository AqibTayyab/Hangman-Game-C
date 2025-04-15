#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define MAX_ATTEMPTS 6
#define DICTIONARY_FILE "words.txt" // Make sure this file exists with one word per line

// Function to count total words in dictionary
int countWordsInFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening dictionary file");
        exit(EXIT_FAILURE);
    }

    int count = 0;
    char word[MAX_WORD_LEN];
    while (fgets(word, sizeof(word), file)) {
        if (strlen(word) > 1) count++;
    }

    fclose(file);
    return count;
}

// Function to select a random word from the dictionary
void getRandomWord(char *selectedWord, const char *filename) {
    int totalWords = countWordsInFile(filename);

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening dictionary file");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));
    int targetLine = rand() % totalWords;

    for (int i = 0; i <= targetLine; i++) {
        fgets(selectedWord, MAX_WORD_LEN, file);
    }

    // Remove newline character
    selectedWord[strcspn(selectedWord, "\n")] = '\0';
    fclose(file);
}

// Function to display the current state of guessed letters
void displayWord(const char *word, const int *guessed) {
    printf("Word: ");
    for (int i = 0; i < strlen(word); i++) {
        if (guessed[i]) {
            printf("%c ", word[i]);
        } else {
            printf("_ ");
        }
    }
    printf("\n");
}

// Function to check if the word is fully guessed
int isWordGuessed(const int *guessed, int len) {
    for (int i = 0; i < len; i++) {
        if (!guessed[i]) return 0;
    }
    return 1;
}

int main() {
    char word[MAX_WORD_LEN];
    getRandomWord(word, DICTIONARY_FILE);

    int wordLen = strlen(word);
    int guessed[wordLen];
    memset(guessed, 0, sizeof(guessed));

    int attemptsLeft = MAX_ATTEMPTS;
    char guess;
    int found;

    printf("🎮 Welcome to Hangman Game! Guess the word before you run out of attempts!\n");

    while (attemptsLeft > 0) {
        displayWord(word, guessed);
        printf("Attempts left: %d\n", attemptsLeft);
        printf("Enter a letter: ");
        scanf(" %c", &guess);
        guess = tolower(guess);

        found = 0;
        for (int i = 0; i < wordLen; i++) {
            if (tolower(word[i]) == guess && !guessed[i]) {
                guessed[i] = 1;
                found = 1;
            }
        }

        if (!found) {
            printf("❌ Wrong guess!\n");
            attemptsLeft--;
        } else {
            printf("✅ Good guess!\n");
        }

        if (isWordGuessed(guessed, wordLen)) {
            printf("\n🎉 Congratulations! You guessed the word: %s\n", word);
            break;
        }
    }

    if (attemptsLeft == 0) {
        printf("\n💀 Game Over! The word was: %s\n", word);
    }

    return 0;
}
