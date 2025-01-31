#include <math.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "map_plot.h"  // Include the header file

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

void generateRooms() {
    srand(time(NULL));

    for (int f = 0; f < FLOORS; f++) {
        int placedRooms = 0;
        Room rooms[6];

        while (placedRooms < ROOMS) {
            Room room;
            room.width = 4 + rand() % 8;
            room.height = 4 + rand() % 8;

            int regionX = (placedRooms % 3) * (MAP_WIDTH / 3);
            int regionY = (placedRooms / 3) * (MAP_HEIGHT / 2);

            room.x = regionX + 1 + rand() % ((MAP_WIDTH / 3) - room.width - 2);
            room.y = regionY + 1 + rand() % ((MAP_HEIGHT / 2) - room.height - 2);

            if (room.x + room.width < MAP_WIDTH && room.y + room.height < MAP_HEIGHT) {
                for (int y = room.y; y < room.y + room.height; y++) {
                    for (int x = room.x; x < room.x + room.width; x++) {
                        dungeon[f][y][x] = '.';
                    }
                }
                rooms[placedRooms] = room;
                placedRooms++;
            }
        }

        for (int i = 0; i < 5; i++) {
            Room room1 = rooms[i];
            Room room2 = rooms[i + 1];

            int centerX1 = room1.x + room1.width / 2;
            int centerY1 = room1.y + room1.height / 2;
            int centerX2 = room2.x + room2.width / 2;
            int centerY2 = room2.y + room2.height / 2;

            if (centerX1 < centerX2) {
                for (int x = centerX1; x <= centerX2; x++) {
                    dungeon[f][centerY1][x] = '.';
                }
            } else {
                for (int x = centerX1; x >= centerX2; x--) {
                    dungeon[f][centerY1][x] = '.';
                }
            }

            if (centerY1 < centerY2) {
                for (int y = centerY1; y <= centerY2; y++) {
                    dungeon[f][y][centerX2] = '.';
                }
            } else {
                for (int y = centerY1; y >= centerY2; y--) {
                    dungeon[f][y][centerX2] = '.';
                }
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

