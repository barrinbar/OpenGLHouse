// All you need to the main body (3d graphics program)

// add to project properties->configuration ->linker->input
//->additional dependences-> opengl32.lib;glu32.lib;glut.lib;

#include "GLUT.H"
#include <math.h>
#include <time.h>
#include <stdio.h>

#define HEIGHT 600
#define WIDTH 600

struct rgb {
	double r;
	double g;
	double b;
};

const int GSIZE = 200;  			// size to paint the ground
const int TSIZE = 256;				// size of texture must be power of two

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

unsigned char tx0[TSIZE][TSIZE][4]; // texture 0
unsigned char tx1[TSIZE][512][4];	// texture 1
unsigned char tx2[512][512][4]; // texture 2
unsigned char tx3[TSIZE][TSIZE][4]; // texture 3

unsigned char* bmp;

rgb wallsColor = { 1,1,0.94 };

void LoadBitmap(char *fname)
{
	FILE* pf;
	BITMAPFILEHEADER bf;
	BITMAPINFOHEADER bi;
	int sz;

	pf = fopen(fname, "rb");
	fread(&bf, sizeof(BITMAPFILEHEADER), 1, pf);
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, pf);

	sz = bi.biHeight*bi.biWidth * 3;
	bmp = (unsigned char*)malloc(sz);

	fread(bmp, 1, sz, pf);

	fclose(pf);
}

// setup matrix of texture
void SetTexture(int tx)
{
	int i, j;
	switch (tx)
	{
	case 0:
		for (i = 0;i<TSIZE;i++)
			for (j = 0;j<TSIZE;j++)
			{
				if (i % 64 >= 60 ||  // horizontal lines
					(((i / 64) % 2 == 0) && (j / (TSIZE / 4) == 0 &&
						j % (TSIZE / 4) >= TSIZE / 4 - 3 || j / (TSIZE / 4) == 2 &&
						j % (TSIZE / 4) >= TSIZE / 4 - 3) || // 3 bricks
						(((i / 64) % 2 == 1) && ((j % (TSIZE / 2) >= TSIZE / 2 - 3))))) // 2 bricks
				{
					tx0[i][j][0] = 50; // red
					tx0[i][j][1] = 50; // green
					tx0[i][j][2] = 50; // blue
					tx0[i][j][3] = 0;
				}
				else
				{
					tx0[i][j][0] = 200 + rand() % 55; // red
					tx0[i][j][1] = 120 + rand() % 50; // green
					tx0[i][j][2] = 0; // blue
					tx0[i][j][3] = 0;
				}
			}
		break;
	case 1:
	{
		int k = 0;
		for (i = 0;i < 256;i++)
			for (j = 0;j < 512;j++, k += 3)
			{
				tx1[i][j][0] = bmp[k + 2]; // red
				tx1[i][j][1] = bmp[k + 1]; // green
				tx1[i][j][2] = bmp[k]; // blue
				tx1[i][j][3] = 0;
			}
		break;
	}
	case 2:
	{
		int k = 0;
		for (i = 0;i < 512;i++)
			for (j = 0;j < 512;j++, k += 3)
			{
				tx2[i][j][0] = bmp[k + 2]; // red
				tx2[i][j][1] = bmp[k + 1]; // green
				tx2[i][j][2] = bmp[k]; // blue
				tx2[i][j][3] = 0;
			}
		break;
	}
	}
}


void TextureDefintions()
{
	SetTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE,
		0, GL_RGBA, GL_UNSIGNED_BYTE, tx0);

	LoadBitmap("window.bmp");
	SetTexture(1);
	glBindTexture(GL_TEXTURE_2D, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 256,
		0, GL_RGBA, GL_UNSIGNED_BYTE, tx1);

	LoadBitmap("door.bmp");
	SetTexture(2);
	glBindTexture(GL_TEXTURE_2D, 2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512,
		0, GL_RGBA, GL_UNSIGNED_BYTE, tx2);
}

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

	TextureDefintions();
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


// TODO: adjust
void DrawTexCylinder(int n, int tn, int r, double startAngle, double endAngle)
{
	/*double alpha;
	double teta = 2 * PI / n;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tn); // tn is texture number
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
		GL_MODULATE); // GL_MODULATE to get lighting

	for (alpha = startAngle;alpha<endAngle;alpha += teta)
	{
		glBegin(GL_POLYGON);
		glTexCoord2d(0, 0);
		glVertex3d(r*sin(alpha), 1, r*cos(alpha));
		glTexCoord2d(0, 2);
		glVertex3d(r*sin(alpha + teta), 1, r*cos(alpha + teta));
		glTexCoord2d(2, 2);
		glVertex3d(r*sin(alpha + teta), -1, r*cos(alpha + teta));
		glTexCoord2d(2, 0);
		glVertex3d(r*sin(alpha), -1, r*cos(alpha));
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);*/


	double alpha;
	double teta = 2 * PI / n;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tn); // tn is texture number
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
		GL_MODULATE); // GL_MODULATE to get lighting

	for (alpha = startAngle;alpha<endAngle;alpha += teta)
	{
		glBegin(GL_POLYGON);
		glTexCoord2d(0, teta);
		glVertex3d(r*sin(alpha), 1, r*cos(alpha));
		glTexCoord2d(teta, teta);
		glVertex3d(r*sin(alpha + teta), 1, r*cos(alpha + teta));
		glTexCoord2d(teta, 0);
		glVertex3d(r*sin(alpha + teta), -1, r*cos(alpha + teta));
		glTexCoord2d(0, 0);
		glVertex3d(r*sin(alpha), -1, r*cos(alpha));
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);

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
	glVertex3d(-1, 1, 0);
	glVertex3d(1, 1, 0);
	glVertex3d(1, -1, 0);
	glVertex3d(-1, -1, 0);
	glEnd();
}

void DrawCircle()
{
	//filled circle
	float x1, y1, x2, y2;
	float angle;
	double radius = 1;

	x1 = 0.0, y1 = 0.0;
	
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x1, y1);

	for (angle = 1.0f;angle<361.0f;angle += 0.2)
	{
		x2 = x1 + sin(angle)*radius;
		y2 = y1 + cos(angle)*radius;
		glVertex2f(x2, y2);
	}

	glEnd();
}
void DrawWindow()
{
	// using texture
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // GL_MODULATE to get lighting
	glBegin(GL_POLYGON);
	glTexCoord2d(0, 0);
	glVertex3d(-4, 0, 0);
	glTexCoord2d(0, 1);
	glVertex3d(-4, 4, 0);
	glTexCoord2d(1, 1);
	glVertex3d(4, 4, 0);
	glTexCoord2d(1, 0);
	glVertex3d(4, 0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void DrawDoor()
{
	glColor3d(1, 0, 0);
	// TODO: put texture
	glPushMatrix();
	glTranslated(0, 3.0, 0.1);
	glScaled(1, 3, 1);
	DrawTexCylinder(30, 2, 5, -0.1*PI, 0.1*PI);
	//DrawCylinder(60, 5, 5, 1, -0.2*PI, 0.2*PI);
	glPopMatrix();
}

void DrawPillar()
{
	glColor3d(wallsColor.r - 0.2, wallsColor.g - 0.2, wallsColor.b - 0.2);

	// Bottom
	glPushMatrix();
	glTranslated(0, 0, 0);
	glScaled(1.75, 0.5, 1.75);
	DrawCube();
	glPopMatrix();

	glPushMatrix();
	glScaled(1.5, 1, 1.5);
	
	glPushMatrix();
		glRotated(90, 1, 0, 0);
		DrawCircle();
	glPopMatrix();

	DrawCylinder(60, 1, 1, 1, 0, 2 * PI);
	glPopMatrix();

	// Body
	glPushMatrix();
	glTranslated(0, 1, 0);
	glScaled(1, 17, 1);
	DrawCylinder(60, 1, 1, 1, 0, 2 * PI);
	glPopMatrix();

	// Top
	glPushMatrix();
	glTranslated(0, 18, 0);
	glScaled(1.5, 1, 1.5);
	DrawCylinder(60, 1, 1, 1, 0, 2 * PI);
	glPushMatrix();
	glRotated(90, 1, 0, 0);
	DrawCircle();
	glPopMatrix();
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
	glTranslated(-7.5, 5, 0.01);
	DrawWindow();
	glPopMatrix();

	glPushMatrix();
	glTranslated(7.5, 5, 0.01);
	DrawWindow();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-7.5, 15, 0.01);
	DrawWindow();
	glPopMatrix();

	glPushMatrix();
	glTranslated(7.5, 15, 0.01);
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
		glTranslated(-38, 0.5, 3);
		DrawPillar();
	glPopMatrix();

	glPushMatrix();
		glTranslated(-23, 0.5, 3);
		DrawPillar();
	glPopMatrix();

	glPushMatrix();
		glTranslated(-8, 0.5, 3);
		DrawPillar();
	glPopMatrix();

	glPushMatrix();
		glTranslated(8, 0.5, 3);
		DrawPillar();
	glPopMatrix();

	glPushMatrix();
		glTranslated(23, 0.5, 3);
		DrawPillar();
	glPopMatrix();

	glPushMatrix();
	glTranslated(38, 0.5, 3);
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
	
	/*glPushMatrix();
	glTranslated(0, 10, 20);
	glScaled(5, 10, 5);
	DrawTexCylinder(8, 1);
	glPopMatrix();*/

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