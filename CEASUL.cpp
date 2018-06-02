#include "glos.h"

#include <gl.h>
#include <glu.h>
#include <glaux.h>
#include <math.h>

#define PI 3.1415

void myinit(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);
void CALLBACK display(void);
void CALLBACK IdleFunction(void);
void CALLBACK rotateLeft(void);
void CALLBACK rotateRight(void);
void CALLBACK rotateUP(void);
void CALLBACK rotateDown(void);
void CALLBACK rotateYplus(AUX_EVENTREC *event);
void CALLBACK rotateYminus(AUX_EVENTREC *event);
void ceas();
void corp();
void pendul();
void capac();
void cerc();
void ora();
void ace();
void acoperis();
void bec();
void podea();
void perete();
void incarca_textura(const char* s);

void MatriceUmbra(GLfloat puncte[3][3], GLfloat pozSursa[4], GLfloat mat[4][4]);
void calcCoeficientiPlan(float P[3][3], float coef[4]);

GLdouble culoareUmbra = 0.1;

bool esteUmbra = FALSE;

static GLfloat light_position[] = { 10.0, 10.0, 30.0, 1.0 };

GLUquadricObj *quadObject;

static GLfloat alfa = 0;
static GLfloat x = -40;
static GLfloat y = 0;
static GLfloat s = 0;
static GLfloat m = 0;
static int unghiY = 0;
bool i = FALSE;
static GLfloat angle = 0;   //rotire stanga, dreapta
static GLfloat theta = 0;
int windowWidth = 850;      //latinea ferestrei
int windowHeight = 650;     //inaltimea ferestri
int windowX = 150;          //pozitia pe x a ferestrei
int windowY = 50;           //pozitia pe y a ferestrei

GLuint IDtextura;
GLuint ID1;

void myinit(void)
{
	glClearColor(0.0, 0.3, 0.3, 1.0);

	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 120.0 };
	GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT);
}

void CALLBACK display(void)
{
	GLfloat matUmbra[4][4];
	GLfloat puncte[3][3] = { { -6.0, -9.75, -6.0 },
							{ -6.0, -9.75, 6.0 },
							{ 6.0, -9.75,  6.0 } };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
		MatriceUmbra(puncte, light_position, matUmbra);
		glRotatef(unghiY, 0, 1, 0);
		glPushMatrix();
			glLightfv(GL_LIGHT0, GL_POSITION, light_position);
			esteUmbra = FALSE;
			glRotatef(angle * 180 / 5, 0, 1, 0);
			glRotatef(theta * 180 / 5, 1, 0, 0);
			ceas();
		glPopMatrix();
		//umbra
		glPushAttrib(GL_LIGHTING_BIT);
			glDisable(GL_LIGHTING);
			glPushMatrix();
				glMultMatrixf((GLfloat*)matUmbra);
				esteUmbra = TRUE;
				glRotatef(angle * 180 / 5, 0, 1, 0);
				glRotatef(theta * 180 / 5, 1, 0, 0);
				ceas();
			glPopMatrix();
			bec();
			podea();
			perete();
		glPopAttrib();
	glPopMatrix();
	gluDeleteQuadric(quadObject);
	glFlush();
}
void CALLBACK myReshape(GLsizei w, GLsizei h)
{
	if (!h) return;
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);

	gluPerspective(40.0, (GLfloat)w / h, 10.0, 350.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 60.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

int main(int argc, char** argv)
{
	auxInitDisplayMode(AUX_SINGLE | AUX_RGB | AUX_DEPTH16);
	auxInitPosition(windowX, windowY, windowWidth, windowHeight);
	auxInitWindow("Ceas cu pendul");
	myinit();
	auxKeyFunc(AUX_LEFT, rotateLeft);
	auxKeyFunc(AUX_a, rotateLeft);
	auxKeyFunc(AUX_A, rotateLeft);

	auxKeyFunc(AUX_RIGHT, rotateRight);
	auxKeyFunc(AUX_d, rotateRight);
	auxKeyFunc(AUX_D, rotateRight);

	auxKeyFunc(AUX_UP, rotateUP);
	auxKeyFunc(AUX_w, rotateUP);
	auxKeyFunc(AUX_W, rotateUP);

	auxKeyFunc(AUX_DOWN, rotateDown);
	auxKeyFunc(AUX_s, rotateDown);
	auxKeyFunc(AUX_S, rotateDown);

	auxMouseFunc(AUX_LEFTBUTTON, AUX_MOUSEDOWN, rotateYplus);
	auxMouseFunc(AUX_RIGHTBUTTON, AUX_MOUSEDOWN, rotateYminus);

	auxReshapeFunc(myReshape);
	auxIdleFunc(IdleFunction);
	auxMainLoop(display);
	return(0);
}

//   DESENARE OBIECT

void corp() {
	
	glPushMatrix();
	if (esteUmbra == FALSE)
		glColor3f(1.0, 0.5, 0.2);
	
	else
		glColor3f(culoareUmbra, culoareUmbra, culoareUmbra);
	glTranslatef(6.5, 0, 0);
	glRotatef(90, 0, 1, 0);
	auxSolidBox(2, 7, 5);
	glPopMatrix();
}

void cerc() {
	
	glPushMatrix();
	if (esteUmbra == FALSE)
		glColor3f(0.0, 0.0, 0.0);
	else
		glColor3f(culoareUmbra, culoareUmbra, culoareUmbra);
	glTranslatef(6.5, 0, 0.3);
	glRotatef(90, 1, 0, 0);
	auxSolidCylinder(2, 0.5);
	glPopMatrix();
}

void capac() {
	quadObject = gluNewQuadric();
	glPushMatrix();
	if (esteUmbra == FALSE)
		glColor3f(0, 0, 0);
	else
		glColor3f(culoareUmbra, culoareUmbra, culoareUmbra);
	glTranslatef(6.5, 0, 1.3);
	glRotatef(180, 1, 0, 0);
	gluCylinder(quadObject, 0, 2, 0, 60, 70);
	glPopMatrix();
}

void pendul() {
	glPushMatrix();
	if (esteUmbra == FALSE)
		glColor3f(0, 0, 0);
	else
		glColor3f(culoareUmbra, culoareUmbra, culoareUmbra);
	glTranslatef(6.5, -3.4, 0);
	glRotatef(x, 0, 0, 1);
	glTranslatef(0, -2.5, 0);
	auxSolidBox(0.3, 5, 0.3);
	glTranslatef(0, -2.5, 0);
	auxSolidSphere(0.8);
	glPopMatrix();
}

void ora() {
	glPushMatrix();
	if (esteUmbra == FALSE)
		glColor3f(1, 1, 1);
	else
		glColor3f(culoareUmbra, culoareUmbra, culoareUmbra);
	glTranslatef(6.5, 1.4, 1.3);
	auxSolidBox(0.1, 0.6, 0.1);

	glTranslatef(0, -2.8, 0);
	auxSolidBox(0.1, 0.6, 0.1);

	glRotatef(90, 0, 0, 1);
	glTranslatef(1.4, 1.4, 0);
	auxSolidBox(0.1, 0.6, 0.1);

	glTranslatef(0, -2.8, 0);
	auxSolidBox(0.1, 0.6, 0.1);
	glPopMatrix();
}

void ace() {
	glPushMatrix();
	if (esteUmbra == FALSE)
		glColor3f(0.8, 0, 0);
	else
		glColor3f(culoareUmbra, culoareUmbra, culoareUmbra);
	glTranslatef(6.5, 0, 1.3);
	glRotatef(s, 0, 0, 1);
	glTranslatef(0, 0.75, 0);
	auxSolidBox(0.2, 1.5, 0.2);
	glPopMatrix();

	glPushMatrix();
	if (esteUmbra == FALSE)
		glColor3f(1, 1.0, 0);
	else
		glColor3f(culoareUmbra, culoareUmbra, culoareUmbra);
	glTranslatef(6.5, 0, 1.3);
	glRotatef(m, 0, 0, 1);
	glTranslatef(0, 0.55, 0);
	auxSolidBox(0.2, 1.1, 0.2);
	glPopMatrix();
}

void acoperis() {
	
	if (esteUmbra == FALSE)
		glColor3f(0.0, 0.0, 0.0);
		
	else
		glColor3f(culoareUmbra, culoareUmbra, culoareUmbra);

	//partea de jos
	
	glBegin(GL_POLYGON);
		glVertex3f(3.0, 3.4, -2.0);
		glVertex3f(10.0, 3.4, -2.0);
		glVertex3f(10.0, 3.4, 2.0);
		glVertex3f(3.0, 3.4, 2.0);
	glEnd();

	//cele 4 fete
	//1
	glBegin(GL_POLYGON);
	glVertex3f(6.5, 7.0, 0.0);
	glVertex3f(3.0, 3.4, -2.0);
	glVertex3f(10.0, 3.4, -2.0);

	//2
	glVertex3f(10, 3.4, -2.0);
	glVertex3f(10.0, 3.4, 2.0);
	glVertex3f(6.5, 7.0, 0.0);

	//3

	glVertex3f(3.0, 3.4, 2.0);
	glVertex3f(6.5, 7.0, 0.0);
	glVertex3f(10.0, 3.4, 2.0);

	//4

	glVertex3f(3.0, 3.4, 2.0);
	glVertex3f(3.0, 3.4, -2.0);
	glVertex3f(6.5, 7.0, 0.0);
	glEnd();
}


void ceas() {

	glPushMatrix();
	if (esteUmbra == FALSE)glEnable(GL_LIGHTING);
	else glDisable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	corp();
	pendul();
	capac();
	cerc();
	ora();
	ace();
	acoperis();
	glDisable(GL_LIGHTING);
	glPopMatrix();
}

//   BEC

void bec() {
	glPushMatrix();
	glTranslatef(10, 10, 30);
	glColor3f(0.5, 0.5, 0.5);
	auxSolidCylinder(0.25, 0.5);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(10, 10.25, 30);
	glColor3f(1, 1, 0);
	auxSolidCone(0.25, 0.5);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(10, 9.7, 30);
	glColor3f(1, 1, 0);
	auxSolidSphere(0.5);
	glPopMatrix();
}

//   PODEA

void podea(void)
{
	glPushMatrix();
	glColor3f(0.6, 0.4, 0.3);
	glBegin(GL_QUADS);
		 glVertex3f(150.0, -10, -220.0);
		 glVertex3f(-150.0, -10, -220.0);
		 glVertex3f(-150.0, -10, 220.0);
		 glVertex3f(150.0, -10, 220.0);
	glEnd();

	glPopMatrix();
}

//   PERETE

void perete(void)
{
	const char* sir;
	sir = "D:\\Ione\\Facultate\\An 2\\Sem 2\\Grafica\\Ceas cu pendul\\Ceas cu pendul\\covor.bmp";
	incarca_textura(sir);
	ID1 = IDtextura;
	glPushMatrix();
	glColor3f(0, 0.3, 0.3);
	glBindTexture(GL_TEXTURE_2D, ID1);
	glTranslatef(0, 0, -250);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(windowWidth, windowHeight, 0.0);
		glTexCoord2f(0.0, 16.0); glVertex3f(windowWidth, -windowHeight, 0.0);
		glTexCoord2f(16.0, 16.0); glVertex3f(-windowWidth, -windowHeight, 0.0);
		glTexCoord2f(16.0, 0.0); glVertex3f(-windowWidth, windowHeight, 0.0);
	glEnd();
	glPopMatrix();
}

//  TEXTURA

void incarca_textura(const char* s)
{
	AUX_RGBImageRec *pImagineTextura = auxDIBImageLoad(s);

	if (pImagineTextura != NULL)
	{
		glGenTextures(1, &IDtextura);

		glBindTexture(GL_TEXTURE_2D, IDtextura);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, pImagineTextura->sizeX, pImagineTextura->sizeY,
			0, GL_RGB, GL_UNSIGNED_BYTE, pImagineTextura->data);
	}

	if (pImagineTextura)
	{
		if (pImagineTextura->data)
			free(pImagineTextura->data);

		free(pImagineTextura);
	}

}

// ROTATII

void CALLBACK IdleFunction(void)
{
	Sleep(0.5);
	if (x <= -40)
	{
		i = FALSE;
	}
	if (x >= 40)
	{
		i = TRUE;
	}
	if (!i)
	{
		x = x + 0.053;
	}
	if (i)
	{
		x = x - 0.053;
	}
	y = y - 0.24;
	if (y <= -360)
	{
		y = 0;
		s = s - 6;
	}
	if (s <= -360)
	{
		s = 0;
		m = m - 6;
	}

	display();

}

void CALLBACK rotateLeft(void)
{
	angle = angle - PI / 12;
}

void CALLBACK rotateRight(void)
{
	angle = angle + PI / 12;
}

void CALLBACK rotateUP(void)
{
	theta = theta + PI / 12;
}

void CALLBACK rotateDown(void)
{
	theta = theta - PI / 12;
}

void CALLBACK rotateYplus(AUX_EVENTREC *event)
{
	unghiY = (unghiY + 10) % 360;
}
void CALLBACK rotateYminus(AUX_EVENTREC *event)
{
	unghiY = (unghiY - 10) % 360;
}

//   UMBRA
void calcCoeficientiPlan(float P[3][3], float coef[4])
{
	float v1[3], v2[3];
	float length;
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;
	//calculeazã doi vectori din trei puncte
	v1[x] = P[0][x] - P[1][x];
	v1[y] = P[0][y] - P[1][y];
	v1[z] = P[0][z] - P[1][z];

	v2[x] = P[1][x] - P[2][x];
	v2[y] = P[1][y] - P[2][y];
	v2[z] = P[1][z] - P[2][z];

	//se cacluleazã produsul vectorial al celor doi vectori
	// care reprezintã un al treilea vector perpendicular pe plan 
	// ale cãrui componente sunt chiar coeficienþii A, B, C din ecuaþia planului
	coef[x] = v1[y] * v2[z] - v1[z] * v2[y];
	coef[y] = v1[z] * v2[x] - v1[x] * v2[z];
	coef[z] = v1[x] * v2[y] - v1[y] * v2[x];
	//normalizeazã vectorul
	length = (float)sqrt((coef[x] * coef[x]) + (coef[y] * coef[y]) + (coef[z] * coef[z]));
	coef[x] /= length;
	coef[y] /= length;
	coef[z] /= length;
}
void MatriceUmbra(GLfloat puncte[3][3], GLfloat pozSursa[4], GLfloat mat[4][4])
{
	GLfloat coefPlan[4];
	GLfloat temp;
	//determinã coeficienþii planului
	calcCoeficientiPlan(puncte, coefPlan);
	//determinã ºi pe D
	coefPlan[3] = -(
		(coefPlan[0] * puncte[2][0]) +
		(coefPlan[1] * puncte[2][1]) +
		(coefPlan[2] * puncte[2][2]));
	//temp=A*xL+B*yL+C*zL+D*w
	temp = coefPlan[0] * pozSursa[0] +
		coefPlan[1] * pozSursa[1] +
		coefPlan[2] * pozSursa[2] +
		coefPlan[3] * pozSursa[3];
	//prima coloanã
	mat[0][0] = temp - pozSursa[0] * coefPlan[0];
	mat[1][0] = 0.0f - pozSursa[0] * coefPlan[1];
	mat[2][0] = 0.0f - pozSursa[0] * coefPlan[2];
	mat[3][0] = 0.0f - pozSursa[0] * coefPlan[3];
	//a doua coloana
	mat[0][1] = 0.0f - pozSursa[1] * coefPlan[0];
	mat[1][1] = temp - pozSursa[1] * coefPlan[1];
	mat[2][1] = 0.0f - pozSursa[1] * coefPlan[2];
	mat[3][1] = 0.0f - pozSursa[1] * coefPlan[3];
	//a treia coloana
	mat[0][2] = 0.0f - pozSursa[2] * coefPlan[0];
	mat[1][2] = 0.0f - pozSursa[2] * coefPlan[1];
	mat[2][2] = temp - pozSursa[2] * coefPlan[2];
	mat[3][2] = 0.0f - pozSursa[2] * coefPlan[3];
	//a patra coloana
	mat[0][3] = 0.0f - pozSursa[3] * coefPlan[0];
	mat[1][3] = 0.0f - pozSursa[3] * coefPlan[1];
	mat[2][3] = 0.0f - pozSursa[3] * coefPlan[2];
	mat[3][3] = temp - pozSursa[3] * coefPlan[3];
}
