#include <stdio.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <time.h>
#include <stdbool.h>

#ifndef CONWAY_H_GUARD
#define CONWAY_H_GUARD

#define CONWAY_GRID_WIDTH 80
#define CONWAY_GRID_HEIGHT 60

#define PX_SIZE 8

#define CONWAY_WIN_WIDTH CONWAY_GRID_WIDTH*PX_SIZE
#define CONWAY_WIN_HEIGHT CONWAY_GRID_HEIGHT*PX_SIZE

int window_id;
unsigned char** cell_grid;
int num_cells_live = 0;
bool verbose_mode = false;
bool color_mode = true;
bool paused = true;

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

