#include <ctype.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>

// Function declarations
int valid_password(const char *password);
int valid_email(const char *str);
void next_window();
void screen_setup();
int authenticate_user(const char *username, const char *password);

int main() {
    screen_setup();
    mvprintw(0, 0, "Press Enter to continue.");
    refresh();
    next_window();

    screen_setup();
    int x, y;
    getyx(stdscr, y, x);

    char username[100] = "";
    nocbreak();
    echo();
    mvprintw(y, 0, "Please enter your username: ");
    scanw("%s", username);

    char password[100] = "";
    move(++y, 0);
    clrtoeol();
    mvprintw(y, 0, "Please enter your password: ");
    scanw("%s", password);

    if (authenticate_user(username, password)) {
        move(++y, 0);
        clrtoeol();
        mvprintw(y, 0, "Login successful! Welcome, %s!", username);
    } else {
        move(++y, 0);
        clrtoeol();
        mvprintw(y, 0, "Invalid username or password.");
    }

    move(++y, 0);
    mvprintw(y, 0, "Press Enter to exit.");
    refresh();
    getch();

    endwin();
    return 0;
}

void next_window() {
    int x, y;
    getyx(stdscr, y, x);
    move(++y, 0);
    short int ch;

    while (1) {
        clrtoeol();
        ch = getch();

        if (ch == 10) { // Enter key
            break;
        } else if (ch == ' ') {
            mvprintw(y, 0, "You pressed space.");
        } else {
            mvprintw(y, 0, "You pressed %c.", ch);
        }
        refresh();
    }

    clear();
}

void screen_setup() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
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
