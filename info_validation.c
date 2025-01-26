#include <ctype.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>

int has_uppercase(char *str) {
    while (*str) {
        if (isupper(*str)) {
            return 1;
        }
        str++;
    }
    return 0; // No uppercase character found
}

int valid_password(char* password) {
    if (strlen(password) >= 8 && has_uppercase(password)) {
        return 1;
    }
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

// Check if the email is valid
int valid_email(const char *str) {
    const char *at = strchr(str, '@');
    if (at == NULL) return 0;
    
    const char *dot = strchr(at, '.');
    if (dot == NULL) return 0;
    
    if (at - str < 1 || dot - at < 2 || strlen(dot) < 2) return 0;

    return 1;
}

void screen_setup() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

int main() {
    screen_setup();
    mvprintw(0, 0, "Press Enter to continue.");
    refresh();
    next_window();

    screen_setup();
    int x, y;
    getyx(stdscr, y, x);

    char email[100] = "";
    nocbreak();
    echo();
    mvprintw(0, 0, "Please enter your email address: ");
    scanw("%s", email);
    while(!valid_email(email)) {
        move(y, 0);
        clrtoeol();
        mvprintw(0, 0, "Enter a valid email address: ");
        scanw("%s", email);
    }

    char password[100] = "";
    move(++y, 0);
    clrtoeol();
    mvprintw(1, 0, "Please enter your password: ");
    scanw("%s", password);
    while(!valid_password(password)) {
        move(y, 0);
        clrtoeol();
        mvprintw(1, 0, "Enter a valid password: ");
        scanw("%s", password);
        
    }

    endwin();
    return 0;
}
