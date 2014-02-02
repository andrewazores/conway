#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <time.h>

int kWinWidth = 640;
int kWinHeight = 480;
int mWindow;
int mCells[80][60] = {{0}, {0}};
int mRunSpeed = 0;
int mLiveCells;
int mVerbose = 0;
int mColorize = 0;

void reshape(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, kWinWidth, kWinHeight, 0.0f, 0.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, kWinWidth, kWinHeight);

	glutReshapeWindow(kWinWidth, kWinHeight);
	glClear(GL_COLOR_BUFFER_BIT);
}

void randomizeCells() {
	srand(time(NULL));
	for (int i = 0; i < 80; i+= 1) {
		for (int j = 0; j < 60; j += 1) {
			mCells[i][j] = rand() % 2;
			mLiveCells += mCells[i][j];
		}
	}
	glClear(GL_COLOR_BUFFER_BIT);
	glutPostRedisplay();
}

void clearCells() {
	for (int i = 0; i < 80; i+= 1) {
		for (int j = 0; j < 60; j += 1) {
			mCells[i][j] = 0;
		}
	}
	mLiveCells = 0;
	glClear(GL_COLOR_BUFFER_BIT);
	glutPostRedisplay();
}

int getNeighbourCount(int i, int j) {
	int MAX_NORTH = 0, MAX_SOUTH = 59;
	int MAX_WEST = 0, MAX_EAST = 79;
	int sum = 0;

	if (i != MAX_EAST) sum += mCells[i + 1][j];
	if (i != MAX_WEST) sum += mCells[i - 1][j];
	if (j != MAX_SOUTH) sum += mCells[i][j + 1];
	if (j != MAX_NORTH) sum += mCells[i][j - 1];

	if (i != MAX_EAST && j != MAX_SOUTH) sum += mCells[i + 1][j + 1];
	if (i != MAX_EAST && j != MAX_NORTH) sum += mCells[i + 1][j - 1];
	if (i != MAX_WEST && j != MAX_SOUTH) sum += mCells[i - 1][j + 1];
	if (i != MAX_WEST && j != MAX_NORTH) sum += mCells[i - 1][j - 1];

	return sum;
}

void drawCell(int x, int y, int neighbours) {
	glPointSize(8);
	glBegin(GL_POINTS);
	if (mColorize) {
		switch(neighbours) {
			case 0: glColor4f(1.0f, 0.0f, 0.0f, 1.0f); break;
			case 2: case 3: glColor4f(0.0f, 1.0f, 0.0f, 1.0f); break;
			case 4: glColor4f(0.2f, 0.8f, 0.0f, 1.0f); break;
			case 5: glColor4f(0.4f, 0.6f, 0.0f, 1.0f); break;
			case 6: glColor4f(0.6f, 0.4f, 0.0f, 1.0f); break;
			case 7: glColor4f(0.8f, 0.2f, 0.0f, 1.0f); break;
			case 1: case 8: glColor4f(0.9f, 0.1f, 0.0f, 1.0f); break;
			default: glColor4f(0.0f, 0.0f, 0.0f, 1.0f); break;
		}
	} else {
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	}
		
	glVertex2i(8*x + 4, 8*y + 4);
	glEnd();
}

void drawGridline(int x1, int y1, int x2, int y2) {
	glBegin(GL_LINES);
		glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
	glEnd();
}

void drawBoard() {
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < 80; i += 1) {
		for (int j = 0; j < 60; j += 1) {
			if (mCells[i][j]) {
				drawCell(i, j, getNeighbourCount(i, j));
			}
			drawGridline(0, j*8, 640, j*8);
		}
		drawGridline(i*8, 0, i*8, 480);
	}
	glutSwapBuffers();
}

void simulate(int unused) {
	int newCells[80][60] = {{0}, {0}};
	for (int i = 0; i < 80; i+= 1) {
		for (int j = 0; j < 60; j+= 1) {
			
			int sum = getNeighbourCount(i, j);

			if (mCells[i][j] == 1) { // Live cell
				if (sum == 2 || sum == 3) {
					newCells[i][j] = 1;
				} else { // sum < 2 || sum > 3
					newCells[i][j] = 0;
					mLiveCells -= 1;
				}
			} else { // Dead cell
				if (sum == 3) {
					newCells[i][j] = 1;
					mLiveCells += 1;
				} else {
					newCells[i][j] = 0;
				}
			}
		}
	}

	memmove(mCells, newCells, 80*60*sizeof(int));

	if (mVerbose) {
		printf("Currently living cells: %d\n", mLiveCells);
	}

	if (mLiveCells == 0) {
		mRunSpeed = 0;
	}

	glutPostRedisplay();
}

void display() {
	drawBoard();
	int sleepTime = 0;
	switch (mRunSpeed) {
		case 1: sleepTime = 2000; break;
		case 2: sleepTime = 800; break;
		case 3: sleepTime = 300; break;
	}

	if (mRunSpeed > 0 && sleepTime > 0) {
		glutTimerFunc(sleepTime, simulate, 0);
	} else if (mVerbose) {
		printf("Idling\n");
	}
}

void toggleCell(int x, int y) {
	int cx = (int)x/8;
	int cy = (int)y/8;

	if (mCells[cx][cy]) {
		mCells[cx][cy] = 0;
		mLiveCells -= 1;
	} else {
		mCells[cx][cy] = 1;
		mLiveCells += 1;
	}

	display();
}

void keyboardFunc(unsigned char key, int x, int y) {
	int oldSpeed = mRunSpeed;
	switch(key) {
		case 'Q': case 'q': glutDestroyWindow(mWindow); break;
		case 'R': case 'r': randomizeCells(); drawBoard(); mRunSpeed = 0; break;
		case 'C': case 'c': clearCells(); drawBoard(); mRunSpeed = 0; break;
		case 'P': case 'p': mRunSpeed = 0; break;
		case '1': mRunSpeed = 1; break;
		case '2': mRunSpeed = 2; break;
		case '3': mRunSpeed = 3; break;
		case 'S': case 's': mRunSpeed = 0; simulate(0); glutPostRedisplay(); break;
		case 'V': case 'v': mVerbose = mVerbose == 0 ? 1 : 0; break;
		case 'A': case 'a': mColorize = mColorize == 0? 1 : 0; drawBoard(); break;
	}

	if (oldSpeed != mRunSpeed && oldSpeed == 0) {
		glutPostRedisplay();
	}
}

void mouseClicked(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state) {
		toggleCell(x, y);
	}
}

void printHelp() {
	printf("Written by:\tAndrew Azores\n\t\tMcMaster University\n\t\t");
	printf("Computer Science 3GC3\n\t\tAssignment 1, Part B\n\n");

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

int main(int argc, char** argv) {
	printHelp();

	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowSize(kWinWidth, kWinHeight);
	glutInitWindowPosition(0, 0);	
	glutInit(&argc, argv);

	mWindow = glutCreateWindow("Life");

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	reshape(kWinWidth, kWinHeight);

	randomizeCells();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardFunc);
	glutMouseFunc(mouseClicked);
	
	glutMainLoop();
	return(0);
}
