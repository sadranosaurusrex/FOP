#include <ctype.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "menu.h"

// Function declarations
int valid_password(const char *password);
int valid_email(const char *str);
int authenticate_user(const char *username, const char *password);
void create_account();
void guest_mode();
void login();
int menu();

char usernameext[100] = "";
char passwordext[100] = "";
char emailext[100] = "";

// Screen setup
void screen_setup() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

// int main() {
//     screen_setup();
//     menu();
//     endwin(); // Exit ncurses mode
//     return 0;
// }

int menu() {
    screen_setup();
    
    const char *options[] = {"Log in", "Create account", "Guest mode", "Exit"};
    int n_options = sizeof(options) / sizeof(options[0]);
    int choice = -1;
    int highlight = 0;

    while (1) {
        clear();
        for (int i = 0; i < n_options; i++) {
            if (i == highlight) {
                attron(A_REVERSE); // Highlight the current option
            }
            mvprintw(i, 0, options[i]); // Print option
            if (i == highlight) {
                attroff(A_REVERSE); // Turn off highlight
            }
        }

        int key = getch();
        switch (key) {
            case KEY_UP:
                highlight = (highlight - 1 + n_options) % n_options; // Move up
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % n_options; // Move down
                break;
            case 10: // Enter
                choice = highlight;
                break;
        }

        if (choice == 0) {
            login();
            return 0;
        } else if (choice == 1) {
            create_account();
            return 1;
        } else if (choice == 2) {
            return 2;
        } else if (choice == 3) {
            return 3; // Exit
        }

        choice = -1;
    }
}

void login() {
    clear();
    char username[100] = "";
    char password[100] = "";

    echo();
    mvprintw(0, 0, "Please enter your username: ");
    scanw("%s", username);

    mvprintw(1, 0, "Please enter your password: ");
    scanw("%s", password);

    if (authenticate_user(username, password)) {
        mvprintw(3, 0, "Login successful! Welcome, %s!", username);
    } else {
        mvprintw(3, 0, "Invalid username or password.");
    }

    strcpy(usernameext, username);
    strcpy(passwordext, password);

    mvprintw(5, 0, "Press any key to continue. ");
    getch();
}

void create_account() {
    clear();
    char username[100] = "";
    char email[100] = "";
    char password[100] = "";
    
    echo();
    mvprintw(0, 0, "Please enter a username: ");
    scanw(" %s", username);
    while (taken_username(username)) {
        mvprintw(0, 0, "We can't let you have this username. Please enter a new one: ");
        scanw("%s", username);
    }

    mvprintw(1, 0, "Please enter your email address: ");
    scanw("%s", email);
    while (!valid_email(email)) {
        mvprintw(2, 0, "Invalid email. Please re-enter: ");
        clrtoeol();
        scanw("%s", email);
    }

    mvprintw(3, 0, "Please enter your password: ");
    scanw("%s", password);
    while (!valid_password(password)) {
        mvprintw(4, 0, "Password must be 8+ chars, 1 upper, 1 lower, 1 digit. Re-enter: ");
        clrtoeol();
        scanw("%s", password);
    }

    FILE *file = fopen("user_data.txt", "a");
    if (file) {
        fprintf(file, "Username: %s\nEmail: %s\nPassword: %s\n", username, email, password);
        fclose(file);
        mvprintw(6, 0, "Account created successfully!");
    } else {
        mvprintw(6, 0, "Error: Could not save account.");
    }

    strcpy(usernameext, username);
    strcpy(passwordext, password);

    mvprintw(8, 0, "Press any key continue.");
    getch();
}

void guest_mode() {
    clear();
    mvprintw(0, 0, "Welcome to Guest Mode! Enjoy exploring the application.");
    mvprintw(2, 0, "Press any key to return to the menu.");
    getch();
}

int valid_password(const char *password) {
    int has_upper = 0, has_lower = 0, has_digit = 0;

    if (strlen(password) < 8) {
        return 0; // Password is too short
    }

    while (*password) {
        if (isupper(*password)) {
            has_upper = 1;
        } else if (islower(*password)) {
            has_lower = 1;
        } else if (isdigit(*password)) {
            has_digit = 1;
        }

        if (has_upper && has_lower && has_digit) {
            return 1;
        }
        password++;
    }
    return 0;
}

int valid_email(const char *str) {
    const char *at = strchr(str, '@');
    if (at == NULL) return 0;

    const char *dot = strchr(at, '.');
    if (dot == NULL) return 0;

    if (at - str < 1 || dot - at < 2 || strlen(dot) < 2) return 0;

    return 1;
}

int taken_username(char* username) {
    if (strlen(username) < 2) return 1;

    FILE *file = fopen("user_data.txt", "r");
    if (file == NULL) {
        return 0; // File not found
    }

    char file_username[100];
    char file_email[100];
    char file_password[100];

    while (fscanf(file, "Username: %99s\nEmail: %99s\nPassword: %99s\n", file_username, file_email, file_password) == 3) {
        if (strcmp(file_username, username) == 0) {
            fclose(file);
            return 1;
        }
    }
    
    return 0;
}

int authenticate_user(const char *username, const char *password) {
    FILE *file = fopen("user_data.txt", "r");
    if (file == NULL) {
        return 0; // File not found
    }

    char file_username[100];
    char file_email[100];
    char file_password[100];

    while (fscanf(file, "Username: %99s\nEmail: %99s\nPassword: %99s\n", file_username, file_email, file_password) == 3) {
        if (strcmp(file_username, username) == 0 && strcmp(file_password, password) == 0) {
            fclose(file);
            return 1; // Authentication successful
        }
    }

    fclose(file);
    return 0; // Authentication failed
}
