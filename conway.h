#include <stdio.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <time.h>

#include "bool.h"

#ifndef CONWAY_H_GUARD
#define CONWAY_H_GUARD

void reshape(int, int);
void randomize_grid();
void clear_grid();
void simulate(int);
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

