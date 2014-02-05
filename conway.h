#ifndef CONWAY_H_GUARD
#define CONWAY_H_GUARD

#include <GL/gl.h>
#include <GL/glut.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define DEFAULT_GRID_WIDTH 80
#define DEFAULT_GRID_HEIGHT 60

#define DEFAULT_PX_SIZE 8

void reshape(int, int);
void randomize_grid();
void clear_grid();
void simulate();
void display();
void kbd_func(unsigned char, int, int);
void mouse_func(int, int, int, int);
void print_help();
void shutdown();
unsigned char** make_2d_array(int, int);
void free_2d_array(unsigned char**, int);
int main(int, char**);

int get_neighbours(int, int);
void draw_cell(int, int, int);
void draw_gridline(int, int, int, int);
void draw_board();
void toggle_cell(int, int);

#endif

