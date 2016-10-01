///////////////////////////////////////////////////////////////////////////
//								House						    	     //
///////////////////////////////////////////////////////////////////////////

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
double eyex = 0, eyez = 85, eyey = 5; // camera - eye
double dirx, diry, dirz;			// camera - look at
double dx = 0, dy = 0, dz = 0; 		// change position 
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
rgb floorColor = { 0.9,0.05,0.05 };

double sunPositionX = 1;
double sunPositionY = 1;
GLfloat mat_shininess[] = { 200 };
double sun_alpha = 0; // sun angle;

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

	// set background color
	glClearColor(0, 0.8, 1, 0);
	glEnable(GL_DEPTH_TEST);

	TextureDefintions();
}


void DrawGround()
{
	int i, j;

	glColor3d(0.5, 1, 0.5);

	for (i = 0; i<GSIZE - 1; i++)
		for (j = 0; j<GSIZE - 1; j++)
		{
			glBegin(GL_POLYGON);
			glVertex3d(j - GSIZE / 2, 0, i - GSIZE / 2);
			glVertex3d(j - GSIZE / 2, 0, i + 1 - GSIZE / 2);
			glVertex3d(j + 1 - GSIZE / 2, 0, i + 1 - GSIZE / 2);
			glVertex3d(j + 1 - GSIZE / 2, 0, i - GSIZE / 2);
			glEnd();
		}
}

void DrawCylinder(int n, double topr, double bottomr, int spaces, double startAngle, double endAngle)
// sefault : n = 80(shrap), spaces = 1 (full cylinder), startAngle = 0, endAngle = 2*PI
{
	double alpha;
	double teta = 2 * PI / n;

	for (alpha = startAngle;alpha<endAngle;alpha += spaces*teta)
	{
		glBegin(GL_POLYGON);
		glNormal3d(sin(alpha), 0, cos(alpha));
		glVertex3d(topr*sin(alpha), 1, topr*cos(alpha));
		glVertex3d(topr*sin(alpha + teta), 1, topr*cos(alpha + teta));
		glVertex3d(bottomr*sin(alpha + teta), 0, bottomr*cos(alpha + teta));
		glVertex3d(bottomr*sin(alpha), 0, bottomr*cos(alpha));
		glEnd();
	}
}

void DrawTexCylinder(int n, int tn, int r, double startAngle, double endAngle)
{
	double alpha;
	double teta = 2 * PI / n;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tn); // tn is texture number
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
		GL_MODULATE); // GL_MODULATE to get lighting

	for (alpha = startAngle;alpha<endAngle;alpha += teta)
	{
		glBegin(GL_POLYGON);
		glNormal3d(sin(alpha), 0, cos(alpha));
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

void DrawTexCylinder1(int n, int tn, int r, double startAngle, double endAngle)
{
	double alpha;
	double teta = 2 * PI / n;
	int c;
	double part = 1 / (double)n;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tn); // tn is texture number
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // GL_MODULATE to get lighting

	for (alpha = startAngle, c = 0;alpha<endAngle;alpha += teta, c++)
	{
		glBegin(GL_POLYGON);

		glNormal3d(sin(alpha), 0, cos(alpha));
		glTexCoord2d(c*part, 0);
		glVertex3d(r*sin(alpha), 1, r*cos(alpha)); // 1
		glTexCoord2d((c + 1)*part, 0);
		glVertex3d(r*sin(alpha + teta), 1, r*cos(alpha + teta)); // 2
	}

	glTexCoord2d((c + 1)*part, 2);
	glVertex3d(r*sin(alpha + teta), 0, r*cos(alpha + teta)); // 3

	glTexCoord2d(c*part, 2);
	glVertex3d(r*sin(alpha), 0, r*cos(alpha)); // 4
	glEnd();
	glDisable(GL_TEXTURE_2D);
}


void DrawCube()
{
	// top
	glBegin(GL_POLYGON);
	glNormal3d(0,1,0);
	glVertex3d(-1, 1, 1);
	glVertex3d(1, 1, 1);
	glVertex3d(1, 1, -1);
	glVertex3d(-1, 1, -1);
	glEnd();
	// top
	glBegin(GL_POLYGON);
	glNormal3d(0, -1, 0);
	glVertex3d(-1, -1, 1);
	glVertex3d(1, -1, 1);
	glVertex3d(1, -1, -1);
	glVertex3d(-1, -1, -1);
	glEnd();
	// front
	glBegin(GL_POLYGON);
	glNormal3d(0, 0, 1);
	glVertex3d(-1, 1, 1);
	glVertex3d(1, 1, 1);
	glVertex3d(1, -1, 1);
	glVertex3d(-1, -1, 1);
	glEnd();
	// back
	glBegin(GL_POLYGON);
	glNormal3d(0, 0, -1);
	glVertex3d(1, 1, -1);
	glVertex3d(1, -1, -1);
	glVertex3d(-1, -1, -1);
	glVertex3d(-1, 1, -1);
	glEnd();
	// left
	glBegin(GL_POLYGON);
	glNormal3d(-1, 0, 0);
	glVertex3d(-1, 1, 1);
	glVertex3d(-1, 1, -1);
	glVertex3d(-1, -1, -1);
	glVertex3d(-1, -1, 1);
	glEnd();
	// right
	glBegin(GL_POLYGON);
	glNormal3d(1, 0, 0);
	glVertex3d(1, 1, 1);
	glVertex3d(1, 1, -1);
	glVertex3d(1, -1, -1);
	glVertex3d(1, -1, 1);
	glEnd();
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

void addLight()
{
	GLfloat light_position[] = { 40,sunPositionX,sunPositionY, 0 };


	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	glEnable(GL_COLOR_MATERIAL); // to save the original color of the object
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT_AND_BACK | GL_FRONT_FACE | GL_LEFT | GL_RIGHT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

//draw sun
void drawSun()
{
	glDisable(GL_LIGHTING);
	glColor3d(1, 1, 0);
	glPushMatrix();
	glRotated(0, 0, 1, 0);
	glTranslated(sunPositionX, sunPositionY, 0);
	glScaled(10, 10, 10);
	DrawSphere(80, 80, 1, -PI / 2, PI / 2);
	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void DrawLightBulb(double x, double y, double z, GLenum light)
{
	// Create light components
	GLfloat ambientLight[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0f };
	GLfloat position[] = { x, y, z, 1.0 };

	// Assign created components to GL_LIGHT0
	glLightfv(light, GL_DIFFUSE, diffuseLight);
	glLightfv(light, GL_POSITION, position);

	glDisable(GL_LIGHTING);
	glColor3d(1, 1, 0);
	glPushMatrix();
		glTranslated(x, y, z);
		glScaled(0.25, 0.25, 0.25);
		DrawSphere(60, 60, 1, -PI / 2, PI / 2);
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glEnable(light);
	glEnable(GL_COLOR_MATERIAL);
	// set material properties which will be assigned by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

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
		glTranslated(disatnce, 0, 0);
		glPushMatrix();
			glTranslated(0,3,0);
			glScaled(1, 0, 1);
			glutSolidSphere(1, 60, 60);
		glPopMatrix();
		glScaled(0.5, 3,0.5);
		DrawCylinder(80, 2, 2, 1, 0, 2 * PI);
	glPopMatrix();

	// draw pillar 2
	glColor3d(0.54, 0.27, 0.07);
	glPushMatrix();
		glTranslated(0, 0, -disatnce);
		glPushMatrix();
			glTranslated(0, 3, 0);
			glScaled(1, 0, 1);
			glutSolidSphere(1, 60, 60);
		glPopMatrix();
		glScaled(0.5, 3, 0.5);
		DrawCylinder(80, 2, 2, 1, 0, 2 * PI);
	glPopMatrix();

	// draw wall betweem pillar 1 -2
	glColor3d(0.54, 0.27, 0.07);
	glPushMatrix();
		glTranslated(0, 0, -disatnce/2);
		glScaled(0.1, 3, -disatnce/2);
		DrawCube();
	glPopMatrix();

	// draw pillar 3
	glColor3d(0.54, 0.27, 0.07);
	glPushMatrix();
		glTranslated(disatnce, 0, -disatnce);
		glPushMatrix();
			glTranslated(0, 3, 0);
			glScaled(1, 0, 1);
			glutSolidSphere(1, 60, 60);
		glPopMatrix();
		glScaled(0.5, 3, 0.5);
		DrawCylinder(80, 2, 2, 1, 0, 2 * PI);
	glPopMatrix();

	// draw wall betweem pillar 2 -3
	glColor3d(0.54, 0.27, 0.07);
	glPushMatrix();
		glTranslated(disatnce/2, 0, -disatnce);
		glScaled(-disatnce / 2, 3, 0.1);
		DrawCube();
	glPopMatrix();

	// draw pillar 4
	glColor3d(0.54, 0.27, 0.07);
	glPushMatrix();
		glTranslated(0, 0, 0);
		glPushMatrix();
			glTranslated(0, 3, 0);
			glScaled(1, 0, 1);
			glutSolidSphere(1, 60, 60);
		glPopMatrix();
		glScaled(0.5, 3, 0.5);
		DrawCylinder(80, 2, 2, 1, 0, 2 * PI);
	glPopMatrix();

	// draw wall betweem pillar 3 -4
	glColor3d(0.54, 0.27, 0.07);
	glPushMatrix();
		glTranslated(disatnce , 0, -disatnce / 2);
		glScaled(0.1, 3, disatnce / 2);
		DrawCube();
	glPopMatrix();

	// draw wall betweem pillar 1 -4
	glColor3d(0.54, 0.27, 0.07);
	glPushMatrix();
		glTranslated(disatnce / 5, 0, 0);
		glScaled(disatnce /5, 3, 0.1);
		DrawCube();
	glPopMatrix();

	glColor3d(0.54, 0.27, 0.07);
	glPushMatrix();
		glTranslated(disatnce-disatnce / 5, 0, 0);
		glScaled(-disatnce / 5, 3, 0.1);
		DrawCube();
	glPopMatrix();

	//draw road
	glColor3d(0.54, 0.27, 0.07);
	glPushMatrix();
		glTranslated(disatnce/2, 0, 0 -10);
		glScaled(3, 0.1, disatnce / 4);
		DrawCube();
	glPopMatrix();
}
void DrawSquare()
{
	glBegin(GL_POLYGON);
		glNormal3d(0,0,0);
		glVertex3d(-1, 1, 0);
		glVertex3d(1, 1, 0);
		glVertex3d(1, -1, 0);
		glVertex3d(-1, -1, 0);
	glEnd();
}

void DrawWindow()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 1); // tn is texture number
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
		GL_MODULATE); // GL_MODULATE to get lighting

	glPushMatrix();
	glBegin(GL_POLYGON);
	glNormal3d(0, 0, -1);
	glTexCoord2d(0, 1);
	glVertex3d(-2, 2, 1);
	glTexCoord2d(1, 1);
	glVertex3d(2, 2, 1);
	glTexCoord2d(1, 0);
	glVertex3d(2, -2, 1);
	glTexCoord2d(0, 0);
	glVertex3d(-2, -2, 1);
	glEnd();
	glPopMatrix();
}

void DrawDoor()
{
	glColor3d(1, 0, 0);
	glPushMatrix();
	glTranslated(0, 3.0, 0.1);
	glScaled(1, 3, 1);
	DrawTexCylinder(30, 2, 5, -0.1*PI, 0.1*PI);
	glPopMatrix();
}

void DrawPillar()
{
	glColor3d(wallsColor.r - 0.2, wallsColor.g - 0.2, wallsColor.b - 0.2);
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat mat_amb_diff[] = { wallsColor.r - 0.1, wallsColor.g - 0.1, wallsColor.b - 0.1, 1.0 };
	
	//GLfloat mat_amb_diff[] = { 0.1, 0.5, 0.8, 1.0 };
	glMaterialfv(GL_FRONT| GL_FRONT_FACE | GL_LEFT | GL_RIGHT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT| GL_FRONT_FACE | GL_LEFT | GL_RIGHT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);


	// Bottom
	glPushMatrix();
	glScaled(1.75, 0.5, 1.75);
	DrawCube();
	glPopMatrix();

	glPushMatrix();
	glScaled(1.5, 1, 1.5);
	DrawCylinder(60, 1, 1, 1, 0, 2 * PI);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 1, 0);
	glScaled(1.5, 0, 1.5);
	glutSolidSphere(1, 60, 60);
	glPopMatrix();

	// Body
	glPushMatrix();
	glTranslated(0, 1, 0);
	glScaled(1, 17, 1);
	DrawCylinder(60, 1, 1, 1, 0, 2 * PI);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 18, 0);
	glScaled(1.5, 0, 1.5);
	glutSolidSphere(1, 60, 60);
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

void DrawWindowpane()
{
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
void DrawWing()
{
	// Wall
	glColor3d(wallsColor.r, wallsColor.g, wallsColor.b);
	glPushMatrix();
		glTranslated(0, 10, 0);
		glScaled(20, 10, 0.5);
		DrawCube();
	glPopMatrix();

	DrawWindowpane();
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

void BackWall()
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
		glScaled(5, 10, 0.5);
		DrawCube();
	glPopMatrix();

	DrawWindowpane();

	// Right wing
	glPushMatrix();
		glTranslated(25, 0, 0);
		DrawWing();
	glPopMatrix();
}

void SideWall()
{
	glColor3d(wallsColor.r, wallsColor.g, wallsColor.b);
	// Left wing
	glPushMatrix();
	glTranslated(-20, 0, 0);
	DrawWing();
	glPopMatrix();

	// Right wing
	glPushMatrix();
	glTranslated(20, 0, 0);
	DrawWing();
	glPopMatrix();
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
	glTranslated(45, 0, -40);
	glRotated(90, 0, 1, 0);
	//BackWall();
	SideWall();
	glPopMatrix();

	// Left wall
	glPushMatrix();
	glTranslated(-45, 0, -40);
	glRotated(270, 0, 1, 0);
	//BackWall();
	SideWall();
	glPopMatrix();

	// Back wall
	glPushMatrix();
	glTranslated(0, 0, -80);
	glRotated(180, 0, 1, 0);
	BackWall();
	glPopMatrix();

	//draw roof
	glPushMatrix();
	glTranslated(0, 20, -40);
	glRotated(45, 0, 1, 0);
	glScaled(70, 20, 70);
	drawRoof();
	glPopMatrix();
}

void RoomEntrance()
{
	// Wall
	glColor3d(wallsColor.r, wallsColor.g, wallsColor.b);
	glPushMatrix();
	glScaled(4, 5, 0.5);
	DrawCube();
	glPopMatrix();

	// Door
	glPushMatrix();
	glTranslated(6, 3, 0);
	glScaled(2, 2, 0.5);
	DrawCube();
	glPopMatrix();

	// Wall
	glPushMatrix();
	glTranslated(12, 0, 0);
	glScaled(4, 5, 0.5);
	DrawCube();
	glPopMatrix();
}

void RoomWall()
{
	// Wall
	glColor3d(wallsColor.r, wallsColor.g, wallsColor.b);
	glPushMatrix();
		glScaled(10, 5, 0.5);
		DrawCube();
	glPopMatrix();
}

void drawStairsLevel(int posX, int posY, int posZ)
{
	glPushMatrix();
	glTranslated(posX, posY, posZ);
	glScaled(5, 1, 1);
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

void SecondFloor()
{
	glPushMatrix();
		glTranslated(0, 10, 15);
		glPushMatrix();
			glTranslated(0, 0.05, 29);
			glScaled(1, 0.1, 1);
			glScaled(1, 0, 1);
			glutSolidSphere(5, 60, 60);
		glPopMatrix();
		glScaled(45, 0.1, 29);
		DrawCube();
	glPopMatrix();

	glPushMatrix();
		glTranslated(-25, 10, -21);
		glScaled(20, 0.1, 15);
		DrawCube();
	glPopMatrix();

	glPushMatrix();
		glTranslated(25, 10, -21);
		glScaled(20, 0.1, 15);
		DrawCube();
	glPopMatrix();

	glPushMatrix();
		glTranslated(-5, 10, -27);
		glScaled(20, 0.1, 9);
		DrawCube();
	glPopMatrix();
}


// Draw floors
void DrawFloors()
{
	// Ground Floor
	glColor3d(floorColor.r, floorColor.g, floorColor.b);
		glPushMatrix();
			glTranslated(0,0.1,4);
			glPushMatrix();
				glTranslated(0, 0.05, 40);
				glScaled(1, 0.1, 1);
				glScaled(1, 0, 1);
				glutSolidSphere(5, 60, 60);
			glPopMatrix();
			glScaled(45,0.1, 40);
			DrawCube();
	glPopMatrix();

	// Second Floor
	glColor3d(floorColor.r, floorColor.g, floorColor.b);
	SecondFloor();

	// Top Ceiling
	glColor3d(floorColor.r, floorColor.g, floorColor.b);
		glPushMatrix();
		glTranslated(0, 20, 4);
		glPushMatrix();
			glTranslated(0, 0.05, 40);
			glScaled(1, 0.1, 1);
			glScaled(1, 0, 1);
			glutSolidSphere(5, 60, 60);
		glPopMatrix();
		glScaled(44, 0.1, 40);
		DrawCube();
	glPopMatrix();

	glPushMatrix();
		glRotated(180, 0, 1, 0);
		glTranslated(0, 0, 10);
		drawStairs(10);
	glPopMatrix();
}

void DrawRooms()
{
	glColor3d(wallsColor.r, wallsColor.g, wallsColor.b);
	glPushMatrix();
	glTranslated(5, 5, -4);
	glRotated(90, 0, 1, 0);
	RoomEntrance();
	glPopMatrix();

	glColor3d(wallsColor.r, wallsColor.g, wallsColor.b);
	glPushMatrix();
	glTranslated(15, 5, -20);
	RoomWall();
	glPopMatrix();

	glColor3d(wallsColor.r, wallsColor.g, wallsColor.b);
	glPushMatrix();
	glTranslated(24, 5, -4);
	glRotated(90, 0, 1, 0);
	RoomEntrance();
	glPopMatrix();

	glColor3d(wallsColor.r, wallsColor.g, wallsColor.b);
	glPushMatrix();
	glTranslated(29, 5, -20);
		RoomEntrance();
	glPopMatrix();
}
void FirstLevel()
{
	DrawRooms();
	glPushMatrix();
	glScaled(-1, 1, 1);
		DrawRooms();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0,0,-20);
		DrawRooms();
		glPushMatrix();
			glScaled(-1, 1, 1);
			DrawRooms();
		glPopMatrix();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 0, -40);
		DrawRooms();
		glPushMatrix();
			glScaled(-1, 1, 1);
			DrawRooms();
		glPopMatrix();
	glPopMatrix();

	glPushMatrix();
		glTranslated(0, 0, -60);
		DrawRooms();
		glPushMatrix();
			glScaled(-1, 1, 1);
			DrawRooms();
		glPopMatrix();
	glPopMatrix();
}

void DrawInterior()
{
	FirstLevel();
	glPushMatrix();
	glTranslated(0, 10, 0);
	FirstLevel();
	glPopMatrix();
}

void DrawHouse()
{
	glPushMatrix();
		glTranslated(0, 0, 44);
		DrawExterior();
		DrawInterior();
	glPopMatrix();

	DrawFloors();
	
	DrawLightBulb(0, 9.6, 1.0, GL_LIGHT1);
}

void DrawGarden()
{
	glPushMatrix();
		glRotated(0, 0, 1, 0);
		glTranslated(-20 * 3, 0, 20 * 3.5);
		glScaled(3, 1, 3);
		drawFence(40);
	glPopMatrix();

	drawTrees();
}

// addone to display
void ShowAll()
{
	glEnable(GL_DEPTH_TEST);
	// start of the transformations
	glMatrixMode(GL_MODELVIEW);
	
	glLoadIdentity();

	DrawGround();
	
	DrawHouse();

	DrawGarden();
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
	
	drawSun();
	addLight();
	ShowAll();

	glEnable(GL_DEPTH_TEST); // Return back to 3d 
	glutSwapBuffers();
}

void idle()
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
	sun_alpha += 0.001;
	sunPositionY = cos(sun_alpha)*cos(sun_alpha)*100;
	sunPositionX = sin(sun_alpha)*sin(sun_alpha)*100;

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
	
	init();

	glutMainLoop();
}