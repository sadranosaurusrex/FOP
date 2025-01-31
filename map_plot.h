#ifndef MAP_GENERATOR_H
#define MAP_GENERATOR_H

#define FLOORS 4
#define MAP_HEIGHT 40
#define MAP_WIDTH 80

extern char dungeon[FLOORS][MAP_HEIGHT][MAP_WIDTH];

void map_generator();
void displayFloor(int floor);
void renderGame();

#endif
