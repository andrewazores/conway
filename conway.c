#include "conway.h"

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
    srand((unsigned int)time(NULL));
    for (int i = 0; i < CONWAY_GRID_WIDTH; ++i)
    {
        for (int j = 0; j < CONWAY_GRID_HEIGHT; ++j)
        {
            cell_grid[i][j] = (unsigned char)rand() % 2;
            num_cells_live += (int)cell_grid[i][j];
        }
    }
    glClear(GL_COLOR_BUFFER_BIT);
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
    glPointSize(PX_SIZE);
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
        
    glVertex2i(PX_SIZE*x + PX_SIZE/2, PX_SIZE*y + PX_SIZE/2);
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
            draw_gridline(0, j*PX_SIZE, PX_SIZE * CONWAY_GRID_WIDTH, j*PX_SIZE);
        }
        draw_gridline(i*PX_SIZE, 0, i*PX_SIZE, PX_SIZE * CONWAY_GRID_HEIGHT);
    }
    glutSwapBuffers();
}

void simulate()
{
    unsigned char** new_cells = make_2d_array(CONWAY_GRID_WIDTH, CONWAY_GRID_HEIGHT);
    for (int i = 0; i < CONWAY_GRID_WIDTH; ++i)
    {
        for (int j = 0; j < CONWAY_GRID_HEIGHT; ++j)
        {
            
            int sum = get_neighbours(i, j);

            if (cell_grid[i][j] == 1)
            { // Live cell
                if (sum == 2 || sum == 3)
                {
                    new_cells[i][j] = 1;
                }
                else
                { // sum < 2 || sum > 3
                    new_cells[i][j] = 0;
                    num_cells_live -= 1;
                }
            }
            else
            { // Dead cell
                if (sum == 3)
                {
                    new_cells[i][j] = 1;
                    num_cells_live += 1;
                }
                else
                {
                    new_cells[i][j] = 0;
                }
            }
        }
    }

    free_2d_array(cell_grid, CONWAY_GRID_WIDTH);
    cell_grid = new_cells;

    if (verbose_mode)
    {
        printf("Currently living cells: %d\n", num_cells_live);
    }

    if (num_cells_live == 0)
    {
        paused = true;
    }

}

void display()
{
    draw_board();
    if (paused) return;
    usleep(sleep_time);
    simulate();
}

void toggle_cell(int x, int y)
{
    int cx = (int)x/PX_SIZE;
    int cy = (int)y/PX_SIZE;

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
}

void kbd_func(unsigned char key, int x, int y)
{
    switch(key)
    {
        case '1': sleep_time = 1000000; break; // 1 frame per second
        case '2': sleep_time = 100000; break; // 10 frames per second
        case '3': sleep_time = 40000; break; // 25 frames per second
        case '4': sleep_time = 20000; break; // 50 frames per second
        case '5': sleep_time = 0; break; // as many frames per second as we can
        case 'Q': case 'q': shutdown(); break;
        case 'R': case 'r': randomize_grid(); draw_board(); glutPostRedisplay(); break;
        case 'C': case 'c': clear_grid(); draw_board(); glutPostRedisplay(); break;
        case 'P': case 'p': paused = (paused + 1) % 2; break;
        case 'S': case 's': simulate(); glutPostRedisplay(); break;
        case 'V': case 'v': verbose_mode = verbose_mode == true ? false : true; break;
        case 'A': case 'a': color_mode = color_mode == true ? false : true; draw_board(); break;
    }
}

void mouse_func(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state)
    {
        toggle_cell(x, y);
    }
}

unsigned char** make_2d_array(int width, int height)
{
    unsigned char** arr = (unsigned char**) malloc(width * sizeof(unsigned char*));
    for (int i = 0; i < width; ++i)
    {
        arr[i] = (unsigned char*) malloc(height * sizeof(unsigned char));
    }
    return arr;
}

void free_2d_array(unsigned char** arr, int width)
{
    for (int i = 0; i < width; ++i)
    {
        free(arr[i]);
    }
    free(arr);
}

void print_help()
{
    printf("Instructions:\n");
    printf("Press 1 (slowest) - 5 (fastest) to select a simulation speed\n");
    printf("Press P to (un)pause the simulation\n");
    printf("Press S to simulate a single generation\n");
    printf("Press R to randomize the board\n");
    printf("Press C to clear the board\n");
    printf("Click a cell on-screen to toggle it on and off individually\n");
    printf("Press A to toggle colour\n");
    printf("Press V to toggle verbose terminal output\n");
    printf("Press Q to quit\n");
}

void shutdown()
{
    glutDestroyWindow(window_id);
    free_2d_array(cell_grid, CONWAY_GRID_WIDTH);
    exit(0);
}

int main(int argc, char** argv)
{
    cell_grid = make_2d_array(CONWAY_GRID_WIDTH, CONWAY_GRID_HEIGHT);
    randomize_grid();
    print_help();

    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    glutInitWindowSize(CONWAY_WIN_WIDTH, CONWAY_WIN_HEIGHT);
    glutInitWindowPosition(0, 0);   
    glutInit(&argc, argv);

    window_id = glutCreateWindow("Conway's Game of Life");

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    reshape(CONWAY_WIN_WIDTH, CONWAY_WIN_HEIGHT);

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(kbd_func);
    glutMouseFunc(mouse_func);
    
    glutMainLoop();
    return(0);
}
