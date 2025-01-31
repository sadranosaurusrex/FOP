#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define FLOORS 4
#define MAP_HEIGHT 40
#define MAP_WIDTH 80
#define ROOMS 6

typedef struct {
    int x, y, width, height;
} Room;

char dungeon[FLOORS][MAP_HEIGHT][MAP_WIDTH];

void initializeDungeon() {
    for (int f = 0; f < FLOORS; f++) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                dungeon[f][y][x] = '#';
            }
        }
    }
}

int canPlaceRoom(int floor, Room room) {
    if (room.x + room.width >= MAP_WIDTH - 1 || room.y + room.height >= MAP_HEIGHT - 1)
        return 0;

    for (int y = room.y; y < room.y + room.height; y++) {
        for (int x = room.x; x < room.x + room.width; x++) {
            if (dungeon[floor][y][x] == '.') {
                return 0;
            }
        }
    }
    return 1;
}

void placeRoom(int floor, Room room) {
    for (int y = room.y; y < room.y + room.height; y++) {
        for (int x = room.x; x < room.x + room.width; x++) {
            dungeon[floor][y][x] = '.';
        }
    }
}

void generateRooms() {
    srand(time(NULL));

    for (int f = 0; f < FLOORS; f++) {
        int placedRooms = 0;

        while (placedRooms < ROOMS) {
            Room room;
            room.width = 4 + rand() % 6;
            room.height = 4 + rand() % 6;

            int regionX = (placedRooms % 3) * (MAP_WIDTH / 3);
            int regionY = (placedRooms / 3) * (MAP_HEIGHT / 2);

            room.x = regionX + 1 + rand() % ((MAP_WIDTH / 3) - room.width - 2);
            room.y = regionY + 1 + rand() % ((MAP_HEIGHT / 2) - room.height - 2);

            if (canPlaceRoom(f, room)) {
                placeRoom(f, room);
                placedRooms++;
            }
        }
    }
}

void displayFloor(int floor) {
    clear();
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            mvaddch(y, x, dungeon[floor][y][x]);
        }
    }
    refresh();
}

void map_generator() {
    initializeDungeon();
    generateRooms();
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
