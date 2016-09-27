// All you need to the main body (3d graphics program)

// add to project properties->configuration ->linker->input
//->additional dependences-> opengl32.lib;glu32.lib;glut.lib;

#include "GLUT.H"
#include <math.h>
#include <time.h>

#define HEIGHT 600
#define WIDTH 600

struct rgb {
	double r;
	double g;
	double b;
};

const int GSIZE = 200;  			// size to paint the ground
const double PI = 4 * atan(1.0);  	// define PI
									// (1) PUT  CONST HERE

									// (2) PUT define HERE 

									// (3) PUT GLOBAL varibles HERE 
double eyex = 0, eyez = 85, eyey = 25; // camera - eye
double dirx, diry, dirz;			// camera - look at
double dx = 0, dy = 0, dz = 0; 				// change position 
double speed = 0;					// change position
double angular_speed = 0;			// change position
double sight = PI; 					// helps to calculate the change of position

double ground[GSIZE][GSIZE];  		// simple ground

rgb wallsColor = { 1,1,0.94 };

void init()
{
	int i, j;

	// optional set random values
	srand(time(0));

	// init ground - optional
	for (i = 0; i<GSIZE; i++)
		for (j = 0; j<GSIZE; j++)
			ground[i][j] = 0;

	// (4) PUT CONTENT HERE

	// set background color
	glClearColor(0, 1, 1, 0);
	glEnable(GL_DEPTH_TEST);
}


void DrawGround()
{

	// draw Y axis
	glColor3d(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 4, 0);
	glEnd();

	int i, j;

	glColor3d(0.5, 1, 0.5);


	for (i = 0; i<GSIZE - 1; i++)
		for (j = 0; j<GSIZE - 1; j++)
		{
			glBegin(GL_POLYGON);// GL_LINE_LOOP);
			glVertex3d(j - GSIZE / 2, 0, i - GSIZE / 2);
			glVertex3d(j - GSIZE / 2, 0, i + 1 - GSIZE / 2);
			glVertex3d(j + 1 - GSIZE / 2, 0, i + 1 - GSIZE / 2);
			glVertex3d(j + 1 - GSIZE / 2, 0, i - GSIZE / 2);
			glEnd();
		}
}

// (5) PUT METHODS HERE


void DrawCylinder(int n, double topr, double bottomr, int spaces, double startAngle, double endAngle)
// sefault : n = 80(shrap), spaces = 1 (full cylinder), startAngle = 0, endAngle = 2*PI
{
	double alpha;
	double teta = 2 * PI / n;

	for (alpha = startAngle;alpha<endAngle;alpha += spaces*teta)
	{
		glBegin(GL_POLYGON);
		glVertex3d(topr*sin(alpha), 1, topr*cos(alpha));
		glVertex3d(topr*sin(alpha + teta), 1, topr*cos(alpha + teta));
		glVertex3d(bottomr*sin(alpha + teta), 0, bottomr*cos(alpha + teta));
		glVertex3d(bottomr*sin(alpha), 0, bottomr*cos(alpha));
		glEnd();
	}
}

void DrawCube()
{
	// top
	glBegin(GL_POLYGON);
	glVertex3d(-1, 1, 1);
	glVertex3d(1, 1, 1);
	glVertex3d(1, 1, -1);
	glVertex3d(-1, 1, -1);
	glEnd();
	// top
	glBegin(GL_POLYGON);
	glVertex3d(-1, -1, 1);
	glVertex3d(1, -1, 1);
	glVertex3d(1, -1, -1);
	glVertex3d(-1, -1, -1);
	glEnd();
	// front
	glBegin(GL_POLYGON);
	glVertex3d(-1, 1, 1);
	glVertex3d(1, 1, 1);
	glVertex3d(1, -1, 1);
	glVertex3d(-1, -1, 1);
	glEnd();
	// back
	glBegin(GL_POLYGON);
	glVertex3d(1, 1, -1);
	glVertex3d(1, -1, -1);
	glVertex3d(-1, -1, -1);
	glVertex3d(-1, 1, -1);
	glEnd();
	// left
	glBegin(GL_POLYGON);
	glVertex3d(-1, 1, 1);
	glVertex3d(-1, 1, -1);
	glVertex3d(-1, -1, -1);
	glVertex3d(-1, -1, 1);
	glEnd();
	// right
	glBegin(GL_POLYGON);
	glVertex3d(1, 1, 1);
	glVertex3d(1, 1, -1);
	glVertex3d(1, -1, -1);
	glVertex3d(1, -1, 1);
	glEnd();
}
void drawTree()
{
	// draw trunk
	glColor3d(0.54, 0.27, 0.07);
	glPushMatrix();
	glRotated(0, 0, 1, 0);
	glTranslated(0, 0, 0);
	glScaled(1, 3, 1);
	DrawCylinder(80, 3, 3, 1, 0, 2 * PI);
	glPopMatrix();
	// draw leaves
	glColor3d(0, 1, 0);
	glPushMatrix();
	glRotated(0, 0, 1, 0);
	glTranslated(0, 2, 0);
	glScaled(1, 6, 1);
	DrawCylinder(80, 0, 5, 1, 0, 2 * PI);
	glPopMatrix();
}

void drawFence(int disatnce)
{
	// draw pillar 1
	glColor3d(0.54, 0.27, 0.07);
	glPushMatrix();
	glRotated(0, 0, 1, 0);
	glTranslated(disatnce, 0, 0);
	glScaled(0.5, 3,0.5);
	DrawCylinder(80, 2, 2, 1, 0, 2 * PI);
	glPopMatrix();

	// draw pillar 2
	glColor3d(0.54, 0.27, 0.07);
	glPushMatrix();
	glRotated(0, 0, 1, 0);
	glTranslated(0, 0, -disatnce);
	glScaled(0.5, 3, 0.5);
	DrawCylinder(80, 2, 2, 1, 0, 2 * PI);
	glPopMatrix();

	// draw wall betweem pillar 1 -2
	glColor3d(0.54, 0.27, 0.07);
	glPushMatrix();
	glRotated(0, 0, 1, 0);
	glTranslated(0, 0, -disatnce/2);
	glScaled(0.1, 3, -disatnce/2);
	DrawCube();
	glPopMatrix();



	// draw pillar 3
	glColor3d(0.54, 0.27, 0.07);
	glPushMatrix();
	glRotated(0, 0, 1, 0);
	glTranslated(disatnce, 0, -disatnce);
	glScaled(0.5, 3, 0.5);
	DrawCylinder(80, 2, 2, 1, 0, 2 * PI);
	glPopMatrix();


	// draw wall betweem pillar 2 -3
	glColor3d(0.54, 0.27, 0.07);
	glPushMatrix();
	glRotated(0, 0, 1, 0);
	glTranslated(disatnce/2, 0, -disatnce);
	glScaled(-disatnce / 2, 3, 0.1);
	DrawCube();
	glPopMatrix();


	// draw pillar 4
	glColor3d(0.54, 0.27, 0.07);
	glPushMatrix();
	glRotated(0, 0, 1, 0);
	glTranslated(0, 0, 0);
	glScaled(0.5, 3, 0.5);
	DrawCylinder(80, 2, 2, 1, 0, 2 * PI);
	glPopMatrix();

	// draw wall betweem pillar 3 -4
	glColor3d(0.54, 0.27, 0.07);
	glPushMatrix();
	glRotated(0, 0, 1, 0);
	glTranslated(disatnce , 0, -disatnce / 2);
	glScaled(0.1, 3, disatnce / 2);
	DrawCube();
	glPopMatrix();

	// draw wall betweem pillar 1 -4
	glColor3d(0.54, 0.27, 0.07);
	glPushMatrix();
	glRotated(0, 0, 1, 0);
	glTranslated(disatnce / 5, 0, 0);
	glScaled(disatnce /5, 3, 0.1);
	DrawCube();
	glPopMatrix();

	glColor3d(0.54, 0.27, 0.07);
	glPushMatrix();
	glRotated(0, 0, 1, 0);
	glTranslated(disatnce-disatnce / 5, 0, 0);
	glScaled(-disatnce / 5, 3, 0.1);
	DrawCube();
	glPopMatrix();

	//draw road
	glColor3d(0.54, 0.27, 0.07);
	glPushMatrix();
	glRotated(0, 0, 1, 0);
	glTranslated(disatnce/2, 0, 0 -10);
	glScaled(3, 0.1, disatnce / 4);
	DrawCube();
	glPopMatrix();


}
void DrawSquare()
{
	glBegin(GL_POLYGON);
	glVertex3d(-1, 1, 1);
	glVertex3d(1, 1, 1);
	glVertex3d(1, -1, 1);
	glVertex3d(-1, -1, 1);
	glEnd();
}

void DrawWindow()
{
	glColor3d(1, 0, 0);
	// TODO: put texture
	glPushMatrix();
	glScaled(3, 3, 1);
	DrawSquare();
	glPopMatrix();
}

void DrawDoor()
{
	glColor3d(1, 0, 0);
	// TODO: put texture
	glPushMatrix();
	glScaled(1, 6, 1);
	DrawCylinder(60, 5, 5, 1, -0.1*PI, 0.1*PI);
	glPopMatrix();
}

void DrawPillar()
{
	glColor3d(wallsColor.r - 0.2, wallsColor.g - 0.2, wallsColor.b - 0.2);

	// Bottom

	glPushMatrix();
	glScaled(1.75, 0.5, 1.75);
	DrawCube();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 1, 0);
	glScaled(1.5, 1, 1.5);
	DrawCylinder(60, 1, 1, 1, 0, 2 * PI);
	glPopMatrix();

	// Body
	glPushMatrix();
	glTranslated(0, 2, 0);
	glScaled(1, 16, 1);
	DrawCylinder(60, 1, 1, 1, 0, 2 * PI);
	glPopMatrix();

	// Top
	glPushMatrix();
	glTranslated(0, 18, 0);
	glScaled(1.5, 1, 1.5);
	DrawCylinder(60, 1, 1, 1, 0, 2 * PI);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 19, 0);
	glScaled(1.75, 0.5, 1.75);
	DrawCube();
	glPopMatrix();
}

void DrawWing()
{
	// Wall
	glColor3d(wallsColor.r, wallsColor.g, wallsColor.b);
	glPushMatrix();
	glTranslated(0, 10, 0);
	glScaled(20, 10, 1);
	DrawSquare();
	glPopMatrix();

	// Windows
	glPushMatrix();
	glTranslated(-7.5, 10, 0.01);
	DrawWindow();
	glPopMatrix();

	glPushMatrix();
	glTranslated(7.5, 10, 0.01);
	DrawWindow();
	glPopMatrix();

}

void DrawFront()
{
	glColor3d(wallsColor.r, wallsColor.g, wallsColor.b);
	// Center wing
	glPushMatrix();
	glScaled(1, 20, 1);
	DrawCylinder(60, 5, 5, 1, -0.5*PI, 0.5*PI);
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 0, 0.01);
		DrawDoor();
		glPopMatrix();
	glColor3d(wallsColor.r, wallsColor.g, wallsColor.b);

	// Left wing
	glPushMatrix();
		glTranslated(-25, 0, 0);
		DrawWing();
	glPopMatrix();

	// Right wing
	glPushMatrix();
		glTranslated(25, 0, 0);
		DrawWing();
	glPopMatrix();

	// Pillars
	glColor3d(wallsColor.r, wallsColor.g, wallsColor.b);

	glPushMatrix();
		glTranslated(-44, 1, 3);
		DrawPillar();
	glPopMatrix();

	glPushMatrix();
		glTranslated(-22, 1, 3);
		DrawPillar();
	glPopMatrix();

	glPushMatrix();
		glTranslated(-10, 1, 3);
		DrawPillar();
	glPopMatrix();

	glPushMatrix();
		glTranslated(10, 1, 3);
		DrawPillar();
	glPopMatrix();

	glPushMatrix();
	glTranslated(22, 1, 3);
	DrawPillar();
	glPopMatrix();

	glPushMatrix();
	glTranslated(44, 1, 3);
	DrawPillar();
	glPopMatrix();
}

void DrawFullWall()
{
	glColor3d(wallsColor.r, wallsColor.g, wallsColor.b);
	// Left wing
	glPushMatrix();
		glTranslated(-25, 0, 0);
		DrawWing();
	glPopMatrix();

	// Center filler wall
	glColor3d(wallsColor.r, wallsColor.g, wallsColor.b);
	glPushMatrix();
		glTranslated(0, 10, 0);
		glScaled(5, 10, 1);
		DrawSquare();
	glPopMatrix();

	// Right wing
	glPushMatrix();
		glTranslated(25, 0, 0);
		DrawWing();
	glPopMatrix();
}


void drawStairsLevel(int posX, int posY, int posZ)
{
	glPushMatrix();
	glRotated(0, 0, 1, 0);
	glTranslated(posX, posY, posZ);
	glScaled(4, 1, 1);
	glColor3d(wallsColor.r, wallsColor.g, wallsColor.b); // choose color
	DrawCube();
	glPopMatrix();;
}

void drawStairs(int levels)
{
	int i;
	for (i = 0; i<levels; i++)
		drawStairsLevel(0, i, i);
}

void drawRoof()
{
	glColor3d(1, 0, 0);
	DrawCylinder(4, 0, 1, 1, 0, 2 * PI);
}

void DrawExterior()
{
	glColor3d(wallsColor.r, wallsColor.g, wallsColor.b);
	DrawFront();

	// Right wall
	glPushMatrix();
		glTranslated(44, 0, -44);
		glRotated(90, 0, 1, 0);
		DrawFullWall();
	glPopMatrix();

	// Left wall
	glPushMatrix();
		glTranslated(-44, 0, -44);
		glRotated(270, 0, 1, 0);
		DrawFullWall();
	glPopMatrix();

	// Back wall
	glPushMatrix();
		glTranslated(0, 0, -88);
		glRotated(180, 0, 1, 0);
		DrawFullWall();
	glPopMatrix();

	//draw roof
	glPushMatrix();
	glTranslated(0, 20, -40);
	glRotated(45, 0, 1, 0);
	glScaled(65, 20, 65);
	drawRoof();
	glPopMatrix();


}
void DrawHouse()
{
	glPushMatrix();
		glTranslated(0, 0, 44);
		DrawExterior();
	glPopMatrix();

}

void drawTrees()
{
	int i;
	for (i = 0; i < 3; i++)
	{
		glPushMatrix();
		glRotated(0, 0, 1, 0);
		glTranslated(-12, 0, 55+5*i);
		glScaled(0.5, 2, 0.5);
		drawTree();
		glPopMatrix();
	}

	for (i = 0; i <3 ; i++)
	{
		glPushMatrix();
		glRotated(0, 0, 1, 0);
		glTranslated(12, 0, 55 + 5 * i);
		glScaled(0.5, 2, 0.5);
		drawTree();
		glPopMatrix();
	}
}
void DrawSphere(int cylinderDensity, int density, int spaces, double startTop, double endBottom)
// default: cylinderDensity = 80,density= 80,spaces = 1, startTop = -PI / 2,endBottom = PI / 2
{
	double beta;
	double delta = PI / density;
	int i;

	for (beta = startTop, i = 0; beta<endBottom; beta += spaces*delta, i++)
	{
		glPushMatrix();
		glRotated(0, 0, 1, 0);
		glTranslated(0, sin(beta), 0);
		glScaled(1, (sin(beta + delta) - sin(beta)), 1);
		DrawCylinder(cylinderDensity, cos(beta + delta), cos(beta), 1, 0, 2 * PI);
		glPopMatrix();
	}
}

// addone to display
void ShowAll()
{
	glEnable(GL_DEPTH_TEST);
	// start of the transformations
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	DrawGround();
	// (6) PUT CONTENT HERE

	DrawHouse();

	drawStairs(10);

	glPushMatrix();
	glRotated(0, 0, 1, 0);
	glTranslated(-20 * 3, 0, 20 * 3.5);
	glScaled(3, 1, 3);
	drawFence(40);
	glPopMatrix();

	drawTrees();
	

	
}

// refresh
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, WIDTH, HEIGHT); // splits window into subwindows
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glFrustum(-1, 1, -1, 1, 0.7, 300); // perspective projection(left,right,bottom,top,near,far)
	gluLookAt(eyex, eyey, eyez, eyex + dirx, eyey - 0.5, eyez + dirz, 0, 1, 0);// (eye,dir,up)

	ShowAll();

	// (7) PUT CONTENT HERE

	glEnable(GL_DEPTH_TEST); // Return back to 3d 
	glutSwapBuffers();
}

void idle() // WRITE OFFSETS IN THIS METHOD
{
	// change position with keyboard
	sight += angular_speed;
	dirx = sin(sight);
	dirz = cos(sight);
	eyex += dirx*speed;
	eyey += dy;
	eyez += dirz*speed;

	//stop moving - remove this if you want automat move
	angular_speed = 0;
	speed = 0;
	dy = 0;
	dx = 0;

	// (8) PUT CONTENT HERE

	glutPostRedisplay(); //-> display
}

// mouse control
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//(9) mouse left click
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		//(10) mouse right click
	}

}

// keyboard regular
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ' ': //stop moving
		angular_speed = 0;
		speed = 0;
		dy = 0;
		dx = 0;
		break;
		// (11) add here 
	default:
		// do nothing
		break;
	}
}


// keyboard special
void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		angular_speed += 0.05;
		break;
	case GLUT_KEY_RIGHT:
		angular_speed -= 0.05;
		break;
	case GLUT_KEY_UP:
		speed += 0.5;
		break;
	case GLUT_KEY_DOWN:
		speed -= 0.5;
		break;
	case GLUT_KEY_PAGE_UP:
		dy += 0.5;
		break;
	case GLUT_KEY_PAGE_DOWN:
		dy -= 0.5;
		break;
		// (12) add here
	}

}

void main(int argc, char* argv[])
{
	// windowing
	glutInit(&argc, argv);
	// GLUT_DOUBLE stands for double buffer
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(100, 100);

	glutCreateWindow("excercise name");
	// set refresh function
	glutDisplayFunc(display);
	glutIdleFunc(idle);

	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard); // for ascii keys
	glutSpecialFunc(special); // for special keys

							  // (13) CALL TO NEW METHOD HERE (like menu)

	init();

	glutMainLoop();
}