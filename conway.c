#include "conway.h"

#define UNUSED(x) (void)(x)

int window_id;
bool** cell_grid;
bool color_mode = true;
bool draw_gridlines = true;
bool paused = true;
unsigned int sleep_time = 1000000;

int grid_width = 80,
    grid_height = 60,
    px_size = 8.0f;

void
reshape(int width, int height)
{
    UNUSED(width); UNUSED(height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, grid_width * px_size, grid_height * px_size, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, grid_width * px_size, grid_height * px_size);

    glutReshapeWindow(grid_width * px_size, grid_height * px_size);
    glClear(GL_COLOR_BUFFER_BIT);
}

void
randomize_grid()
{
    srand((unsigned int)time(NULL));
    for (int i = 0; i < grid_width; ++i)
    {
        for (int j = 0; j < grid_height; ++j)
        {
            cell_grid[i][j] = (bool)(rand() % 2);
        }
    }
    glClear(GL_COLOR_BUFFER_BIT);
}

void
clear_grid()
{
    bool** new_cells = make_2d_array(grid_width, grid_height);
    free_2d_array(cell_grid, grid_width);
    cell_grid = new_cells;

    glClear(GL_COLOR_BUFFER_BIT);
}

int
get_neighbours(int i, int j)
{
    int MAX_NORTH = 0, MAX_SOUTH = grid_height - 1;
    int MAX_WEST = 0, MAX_EAST = grid_width - 1;
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

void
draw_cell(int x, int y, int neighbours)
{
    glPointSize((GLfloat)px_size);
    glBegin(GL_POINTS);
    if (color_mode)
    {
        switch (neighbours)
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
    } else
    {
        glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    }
        
    glVertex2i(x * px_size + px_size / 2, y * px_size + px_size / 2);
    glEnd();
}

void
draw_gridline(int x1, int y1, int x2, int y2)
{
    if (!draw_gridlines) return;
    glBegin(GL_LINES);
        glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
        glVertex2i(x1, y1);
        glVertex2i(x2, y2);
    glEnd();
}

void
draw_board()
{
    glClear(GL_COLOR_BUFFER_BIT);
    for (int i = 0; i < grid_width; ++i)
    {
        for (int j = 0; j < grid_height; ++j)
        {
            if (cell_grid[i][j])
            {
                draw_cell(i, j, get_neighbours(i, j));
            }
            draw_gridline(0, j * px_size, px_size * grid_width, j * px_size);
        }
        draw_gridline(i * px_size, 0, i * px_size, px_size * grid_height);
    }
    glutSwapBuffers();
}

void
simulate()
{
    bool** new_cells = make_2d_array(grid_width, grid_height);
    for (int i = 0; i < grid_width; ++i)
    {
        for (int j = 0; j < grid_height; ++j)
        {
            
            int sum = get_neighbours(i, j);

            if (cell_grid[i][j])
            { // Live cell
                new_cells[i][j] = (sum == 2 || sum == 3);
            } else
            { // Dead cell
                new_cells[i][j] = (sum == 3);
            }
        }
    }

    free_2d_array(cell_grid, grid_width);
    cell_grid = new_cells;
}

void
display()
{
    draw_board();
    if (paused) return;
    usleep(sleep_time);
    simulate();
}

void
toggle_cell(int x, int y)
{
    int cx = (int)x/px_size;
    int cy = (int)y/px_size;

    cell_grid[cx][cy] = !cell_grid[cx][cy];
}

void
kbd_func(unsigned char key, int x, int y)
{
    UNUSED(x); UNUSED(y);
    switch(key)
    {
        case '1': sleep_time = 1000000; paused = false; break; // 1 frame per second
        case '2': sleep_time = 100000; paused = false; break; // 10 frames per second
        case '3': sleep_time = 40000; paused = false; break; // 25 frames per second
        case '4': sleep_time = 20000; paused = false; break; // 50 frames per second
        case '5': sleep_time = 0; paused = false; break; // as many frames per second as we can
        case 'Q': case 'q': shutdown(); break;
        case 'R': case 'r': paused = true; randomize_grid(); draw_board(); break;
        case 'C': case 'c': paused = true; clear_grid(); draw_board(); break;
        case 'P': case 'p': paused = !paused; break;
        case 'S': case 's': paused = true; simulate(); break;
        case 'A': case 'a': color_mode = !color_mode; draw_board(); break;
        case 'G': case 'g': draw_gridlines = !draw_gridlines; break;
    }
}

void
mouse_func(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state)
    {
        toggle_cell(x, y);
    }
}

bool**
make_2d_array(int width, int height)
{
    bool** arr = (bool**) calloc(width, sizeof(bool*));
    if (arr == NULL)
    {
        printf("Could not allocate memory!\n"); exit(1);
    }
    for (int i = 0; i < width; ++i)
    {
        arr[i] = (bool*) calloc(height, sizeof(bool));
        if (arr[i] == NULL)
        {
            printf("Could not allocate memory!\n"); exit(1);
        }
    }
    return arr;
}

void
free_2d_array(bool** arr, int width)
{
    for (int i = 0; i < width; ++i)
    {
        free(arr[i]);
    }
    free(arr);
}

void
print_help()
{
    printf("Flags:\n");
    printf("-h\tthis help message\n");
    printf("-l N\tset grid \"length\" (height) to N\n");
    printf("-w N\tset grid width to N\n");
    printf("-g\tdisable gridlines\n");
    printf("-r\tstart already running, not paused\n");
    printf("-s N\tset initial run speed to N (1-5)\n");
    printf("-p N\tset cell size to N pixels\n");
    printf("-c\tdisable colorized output\n");
    printf("In-window controls:\n");
    printf("\tPress 1 (slowest) - 5 (fastest) to select a simulation speed\n");
    printf("\tPress P to (un)pause the simulation\n");
    printf("\tPress S to simulate a single generation\n");
    printf("\tPress R to randomize the board\n");
    printf("\tPress C to clear the board\n");
    printf("\tClick a cell on-screen to toggle it on and off individually\n");
    printf("\tPress A to toggle colour\n");
    printf("\tPress G to toggle gridlines\n");
    printf("\tPress Q to quit\n");
}

void
shutdown()
{
    glutDestroyWindow(window_id);
    free_2d_array(cell_grid, grid_width);
    exit(0);
}

int
set_opts(int argc, char** argv)
{
    int c;
    while ((c = getopt(argc, argv, "hl:w:grs:p:c")) != -1)
    {
        switch (c)
        {
            case 'h': print_help(); exit(0);
            case 'l': grid_height = atoi(optarg); break;
            case 'w': grid_width = atoi(optarg); break;
            case 'g': draw_gridlines = false; break;
            case 'r': paused = false; break;
            case 's': kbd_func((unsigned char)*optarg, 0, 0); break;
            case 'p': px_size = atoi(optarg); break;
            case 'c': color_mode = false; break;
            case '?':
                if (optopt == 'w' || optopt == 'h' || optopt == 'p')
                {
                  fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                } else if (isprint (optopt))
                {
                  fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                } else
                {
                  fprintf (stderr,
                           "Unknown option character `\\x%x'.\n",
                           optopt);
                }
                return 1;
            default:
                return 1;
        }
    }
    return 0;
}

int
main(int argc, char** argv)
{
    int parse_status = set_opts(argc, argv);
    if (parse_status) exit(parse_status);
    set_opts(argc, argv);
    cell_grid = make_2d_array(grid_width, grid_height);
    randomize_grid();

    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
    glutInitWindowSize(grid_width * px_size, grid_height * px_size);
    glutInitWindowPosition(0, 0);   
    glutInit(&argc, argv);

    window_id = glutCreateWindow("Conway's Game of Life");

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    reshape(grid_width * px_size, grid_height * px_size);

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(kbd_func);
    glutMouseFunc(mouse_func);
    
    glutMainLoop();
    return(0);
}
