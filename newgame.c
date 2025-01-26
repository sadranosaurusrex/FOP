#include <ncurses.h>
//#include <stdio.h>

int main() {
    NewGameChoices();
    endwin();
    return 0;
}

int NewGameChoices() {
    initscr();                
    keypad(stdscr, TRUE);     
    noecho();                 
    cbreak();                 

    int choice = -1;
    int highlight = 0;
    const char *options[] = { "Continue", "NewGame", "Back" };
    int n_options = sizeof(options) / sizeof(options[0]);

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
            case 10: // Enter key
                choice = highlight;
                break;
        }

        if (choice < n_options && choice > -1) { // Exit option
            return choice;
        }
    }

    endwin(); // Exit ncurses mode
    return 0;
}