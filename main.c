#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "map_plot.h"
#include "menu.h"

#define MAP_HEIGHT 40
#define MAP_WIDTH 80

typedef struct {
    char username[40];
    char password[40];
    int logStatus;
    int score;
    int gold;
    int count_games;
    int experience;
} user_data;

user_data newPlayerCreation (char username[], char password[]) {
    user_data newplayer;
    strcpy(newplayer.username, username);
    strcpy(newplayer.password, password);
    newplayer.logStatus = 1;
    newplayer.score = 0;
    newplayer.gold = 0;
    newplayer.count_games = 0;
    newplayer.experience = 0;
}

user_data setupLogin() {
    user_data newuser;
    int menu_value = menu();
    if (menu_value == 3) {
        newuser.logStatus = -1;
        return newuser;
    }
    else if (menu_value == 1 || menu_value == 0) {
        newuser = newPlayerCreation(usernameext, passwordext);
    }
    else if (menu_value == 2) {
        newuser = newPlayerCreation("Guest", "Guest");
    }
    return newuser;
}

void renderGame() {
    initscr();
    noecho();
    curs_set(FALSE);

    int currentFloor = 0;

    while (1) {
        displayFloor(currentFloor);
        int ch = getch();
        if (ch == 'q') break;
        if (ch == 'w' && currentFloor > 0) currentFloor--;
        if (ch == 's' && currentFloor < FLOORS - 1) currentFloor++;
    }

    endwin();
}


int main() {
    user_data player = setupLogin();
    if (player.logStatus == -1) return 0;
    map_generator();
    renderGame();
    return 0;
}
