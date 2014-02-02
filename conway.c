#include "conway.h"

#define CONWAY_WIN_WIDTH 640
#define CONWAY_WIN_HEIGHT 480

#define CONWAY_GRID_WIDTH 80
#define CONWAY_GRID_HEIGHT 60

int window_id;
int cell_grid[CONWAY_GRID_WIDTH][CONWAY_GRID_HEIGHT] = {{0}, {0}};
int sim_speed = 0;
int num_cells_live = 0;
bool verbose_mode = false;
bool color_mode = false;

void reshape(int width, int height)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, CONWAY_WIN_WIDTH, CONWAY_WIN_HEIGHT, 0.0f, 0.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, CONWAY_WIN_WIDTH, CONWAY_WIN_HEIGHT);

	glutReshapeWindow(CONWAY_WIN_WIDTH, CONWAY_WIN_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT);
}

void randomize_grid()
{
	srand(time(NULL));
	for (int i = 0; i < CONWAY_GRID_WIDTH; ++i)
    {
		for (int j = 0; j < CONWAY_GRID_HEIGHT; ++j)
        {
			cell_grid[i][j] = rand() % 2;
			num_cells_live += cell_grid[i][j];
		}
	}
	glClear(GL_COLOR_BUFFER_BIT);
	glutPostRedisplay();
}

void clear_grid() {
	for (int i = 0; i < CONWAY_GRID_WIDTH; ++i)
    {
		for (int j = 0; j < CONWAY_GRID_HEIGHT; ++j)
        {
			cell_grid[i][j] = 0;
		}
	}
	num_cells_live = 0;
	glClear(GL_COLOR_BUFFER_BIT);
	glutPostRedisplay();
}

int get_neighbours(int i, int j)
{
	int MAX_NORTH = 0, MAX_SOUTH = CONWAY_GRID_HEIGHT - 1;
	int MAX_WEST = 0, MAX_EAST = CONWAY_GRID_WIDTH - 1;
	int sum = 0;

	if (i != MAX_EAST) sum += cell_grid[i + 1][j];
	if (i != MAX_WEST) sum += cell_grid[i - 1][j];
	if (j != MAX_SOUTH) sum += cell_grid[i][j + 1];
	if (j != MAX_NORTH) sum += cell_grid[i][j - 1];

	if (i != MAX_EAST && j != MAX_SOUTH) sum += cell_grid[i + 1][j + 1];
	if (i != MAX_EAST && j != MAX_NORTH) sum += cell_grid[i + 1][j - 1];
	if (i != MAX_WEST && j != MAX_SOUTH) sum += cell_grid[i - 1][j + 1];
	if (i != MAX_WEST && j != MAX_NORTH) sum += cell_grid[i - 1][j - 1];

	return sum;
}

void draw_cell(int x, int y, int neighbours)
{
	glPointSize(8);
	glBegin(GL_POINTS);
	if (color_mode)
    {
		switch(neighbours)
        {
			case 0: glColor4f(1.0f, 0.0f, 0.0f, 1.0f); break;
			case 2: case 3: glColor4f(0.0f, 1.0f, 0.0f, 1.0f); break;
			case 4: glColor4f(0.2f, 0.8f, 0.0f, 1.0f); break;
			case 5: glColor4f(0.4f, 0.6f, 0.0f, 1.0f); break;
			case 6: glColor4f(0.6f, 0.4f, 0.0f, 1.0f); break;
			case 7: glColor4f(0.8f, 0.2f, 0.0f, 1.0f); break;
			case 1: case 8: glColor4f(0.9f, 0.1f, 0.0f, 1.0f); break;
			default: glColor4f(0.0f, 0.0f, 0.0f, 1.0f); break;
		}
	}
    else
    {
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	}
		
	glVertex2i(8*x + 4, 8*y + 4);
	glEnd();
}

void draw_gridline(int x1, int y1, int x2, int y2)
{
	glBegin(GL_LINES);
		glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
	glEnd();
}

void draw_board()
{
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < CONWAY_GRID_WIDTH; ++i)
    {
		for (int j = 0; j < CONWAY_GRID_HEIGHT; ++j)
        {
			if (cell_grid[i][j])
            {
				draw_cell(i, j, get_neighbours(i, j));
			}
			draw_gridline(0, j*8, 8 * CONWAY_GRID_WIDTH, j*8);
		}
		draw_gridline(i*8, 0, i*8, 8 * CONWAY_GRID_HEIGHT);
	}
	glutSwapBuffers();
}

void simulate(int unused)
{
	int newCells[CONWAY_GRID_WIDTH][CONWAY_GRID_HEIGHT] = {{0}, {0}};
	for (int i = 0; i < CONWAY_GRID_WIDTH; ++i)
    {
		for (int j = 0; j < CONWAY_GRID_HEIGHT; ++j)
        {
			
			int sum = get_neighbours(i, j);

			if (cell_grid[i][j] == 1)
            { // Live cell
				if (sum == 2 || sum == 3)
                {
					newCells[i][j] = 1;
				}
                else
                { // sum < 2 || sum > 3
					newCells[i][j] = 0;
					num_cells_live -= 1;
				}
			}
            else
            { // Dead cell
				if (sum == 3)
                {
					newCells[i][j] = 1;
					num_cells_live += 1;
				}
                else
                {
					newCells[i][j] = 0;
				}
			}
		}
	}

	memmove(cell_grid, newCells, CONWAY_GRID_WIDTH*CONWAY_GRID_HEIGHT*sizeof(int));

	if (verbose_mode)
    {
		printf("Currently living cells: %d\n", num_cells_live);
	}

	if (num_cells_live == 0)
    {
		sim_speed = 0;
	}

	glutPostRedisplay();
}

void display()
{
	draw_board();
	int sleepTime = 0;
	switch (sim_speed)
    {
		case 1: sleepTime = 2000; break;
		case 2: sleepTime = 800; break;
		case 3: sleepTime = 300; break;
	}

	if (sim_speed > 0 && sleepTime > 0)
    {
		glutTimerFunc(sleepTime, simulate, 0);
	}
    else if (verbose_mode)
    {
		printf("Idling\n");
	}
}

void toggle_cell(int x, int y)
{
	int cx = (int)x/8;
	int cy = (int)y/8;

	if (cell_grid[cx][cy])
    {
		cell_grid[cx][cy] = 0;
		num_cells_live -= 1;
	}
    else
    {
		cell_grid[cx][cy] = 1;
		num_cells_live += 1;
	}

	display();
}

void kbd_func(unsigned char key, int x, int y)
{
	int oldSpeed = sim_speed;
	switch(key)
    {
		case 'Q': case 'q': glutDestroyWindow(window_id); break;
		case 'R': case 'r': randomize_grid(); draw_board(); sim_speed = 0; break;
		case 'C': case 'c': clear_grid(); draw_board(); sim_speed = 0; break;
		case 'P': case 'p': sim_speed = 0; break;
		case '1': sim_speed = 1; break;
		case '2': sim_speed = 2; break;
		case '3': sim_speed = 3; break;
		case 'S': case 's': sim_speed = 0; simulate(0); glutPostRedisplay(); break;
		case 'V': case 'v': verbose_mode = verbose_mode == true ? false : true; break;
		case 'A': case 'a': color_mode = color_mode == true ? false : true; draw_board(); break;
	}

	if (oldSpeed != sim_speed && oldSpeed == 0)
    {
		glutPostRedisplay();
	}
}

void mouse_func(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state)
    {
		toggle_cell(x, y);
	}
}

void print_help()
{
	printf("Instructions:\n");
	printf("Press 1, 2, or 3 to run the simulation at varying speeds (3 is fast)\n");
	printf("Press P to pause the simulation\n");
	printf("Press S to simulate a single generation\n");
	printf("Press R to randomize the board\n");
	printf("Press C to clear the board\n");
	printf("Click a cell on-screen to toggle it on and off individually\n");
	printf("Press A to toggle colour\n");
	printf("Press V to toggle verbose terminal output\n");
	printf("Press Q to quit\n");
}

int main(int argc, char** argv)
{
	print_help();

	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowSize(CONWAY_WIN_WIDTH, CONWAY_WIN_HEIGHT);
	glutInitWindowPosition(0, 0);	
	glutInit(&argc, argv);

	window_id = glutCreateWindow("Life");

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	reshape(CONWAY_WIN_WIDTH, CONWAY_WIN_HEIGHT);

	randomize_grid();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(kbd_func);
	glutMouseFunc(mouse_func);
	
	glutMainLoop();
	return(0);
}
