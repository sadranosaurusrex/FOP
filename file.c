#include <ctype.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>

// Function declarations
int valid_password(const char *password);
int valid_email(const char *str);
void next_window();
void screen_setup();

int main() {
    screen_setup();
    mvprintw(0, 0, "Press Enter to continue.");
    refresh(); // Refresh the window to show the message
    next_window();

    screen_setup();
    int x, y;
    getyx(stdscr, y, x);

    char username[100] = "";
    nocbreak();
    echo();
    mvprintw(y, 0, "Please enter a username: ");
    scanw("%s", username);
    while (strlen(username) < 2) {
        move(y, 0);
        clrtoeol();
        mvprintw(y, 0, "Enter a longer username: ");
        scanw("%s", username);
    }

    char email[100] = "";
    move(++y, 0);
    clrtoeol();
    mvprintw(y, 0, "Welcome %s! Please enter your email address: ", username);
    scanw("%s", email);
    while (!valid_email(email)) {
        move(y, 0);
        clrtoeol();
        mvprintw(y, 0, "Enter a valid email address: ");
        scanw("%s", email);
    }

    char password[100] = "";
    move(++y, 0);
    clrtoeol();
    mvprintw(y, 0, "Please enter your password: ");
    scanw("%s", password);
    while (!valid_password(password)) {
        clrtoeol();
        mvprintw(y, 0, "Enter a valid password (8+ chars, 1 upper, 1 lower, 1 digit): ");
        scanw("%s", password);
    }

    // Save user info to a file
    FILE *file = fopen("user_data.txt", "a"); // Open in append mode
    if (file != NULL) {
        fprintf(file, "Username: %s\n", username);
        fprintf(file, "Email: %s\n", email);
        fprintf(file, "Password: %s\n", password);
        fprintf(file, "-------------------------\n"); // Separator for multiple entries
        fclose(file);
    } else {
        mvprintw(++y, 0, "Error opening file!");
    }

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
