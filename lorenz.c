// file: lorenz.c
// author: Vince Coghlan
// date: 9/9/13
// you can find this as https://github.com/coghex

// Visualization of the Lorenz Attractor
// s will increase s, d will decrease
// b will increase b, n will decrease
// r will increase r, t will decrease
// arrows will move everything around
// - will zoom out, + will zoom in

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Graphing Globals
int th=115;    // azimuth view angle
int ph=-20;    // elevation view angle
double w=1;     // W variable

//  Lorenz Parameters
double s  = 10;
double b  = 2.6666;
double r  = 28;

// stuff to make sure we dont have to recalculate solutions
int N = 50000;
float la[50000][4];
float color[50000][3];

// Print was not written by me but provided by the professor
#define LEN 8192  // Maximum length of text string
void Print(const char* format , ...)
{
  char  buf[LEN];
  char*  ch=buf;
  va_list args;
  //  Turn the parameters into a character string
  va_start(args,format);
  vsnprintf(buf,LEN,format,args);
  va_end(args);
  //  Display the characters one at a time at the current raster position
  while (*ch)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

// We need to set a new 2D enviornment if we want the text on top
void begintext(int height, int width){
  glMatrixMode(GL_PROJECTION);
	// so we can restore the perspective view
	glPushMatrix();
	// reset the matrix
 	glLoadIdentity();
	glOrtho( 0, height, 0, width, 0, 1 );
  // go back to modelview
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// make text on top
  glDisable(GL_DEPTH_TEST);
}

void endtext(){
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	// pop the old matrix
 	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void display()
{
  int i;
  //  Clear the image
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //  Reset previous transforms
  glLoadIdentity();
  //  Set view angle
  glRotated(ph,1,0,0);
  glRotated(th,0,1,0);

  //  draw the points in a strip
  //  the color is based on their position
  glBegin(GL_LINE_STRIP);
  for (i=0;i<N;i++)
  {
    glColor3fv(color[i]);
    glVertex4fv(la[i]);
  }
  glEnd();

  //  Draw axes in white
  glColor3f(1,1,1);
  glBegin(GL_LINES);
  glVertex4d(0,0,0,w);
  glVertex4d(2,0,0,w);
  glVertex4d(0,0,0,w);
  glVertex4d(0,2,0,w);
  glVertex4d(0,0,0,w);
  glVertex4d(0,0,2,w);
  glEnd();
  //  Label axes
  glRasterPos4d(2,0,0,w);
  Print("X");
  glRasterPos4d(0,2,0,w);
  Print("Y");
  glRasterPos4d(0,0,2,w);
  Print("Z");
  //  Display parameters
  glWindowPos2i(5,5);

  //  We want text on the bottom right of the screen
  //  I dont think the width height matter because
  //  the words are at 0, 0
  begintext(500, 500);
  Print("s = %f, b =%f, r = %f", s, b, r);
  endtext();

  //  Done!
  glFlush();
  glutSwapBuffers();
}

// variation of professor's code that stores the values globally
static void lorenzGen(void) {
	int i;
	/*  Coordinates  */
	double x = 1;
	double y = 1;
	double z = 1;
  la[0][0] = x;
  la[0][1] = y;
  la[0][2] = z;
  la[0][3] = w;
	/*  Time step  */
	double dt = 0.001;

	for (i=0;i<N-1;i++)
	{	
		double dx = s*(y-x);
		double dy = x*(r-z)-y;
		double dz = x*y - b*z;
		x += dt*dx;
		y += dt*dy;
		z += dt*dz;

  // scale it down or it wont show
		la[i+1][0] = .05*x;
		la[i+1][1] = .05*y;
		la[i+1][2] = .05*z;
    la[i+1][3] = w;
    color[i+1][0] = .1*z;
    color[i+1][1] = x;
    color[i+1][2] = y;
  //printf("%5d %8.3f %8.3f %8.3f\n",i+1,x,y,z);
  }
}

// much of this was provided byt he professor
void key(unsigned char ch,int x,int y)
{
  //  Exit on ESC
  if (ch == 27)
    exit(0);
  //  Reset view angle
  else if (ch == '0')
    th = ph = 0;
  else if (ch == 's') {
    s += .25;
    lorenzGen();
  }
  else if (ch == 'd') {
    s -= .25;
    lorenzGen();
  }
  else if (ch == 'b') {
    b += .25;
    lorenzGen();
  }
  else if (ch == 'n') {
    b -= .25;
    lorenzGen();
  }
  else if (ch == 'r') {
    r += .25;
    lorenzGen();
  }
  else if (ch == 't') {
   r -= .25;
   lorenzGen();
  }
  //  zoom out
  else if (ch == '-')
  {
    w += 0.1;
    lorenzGen();
  }
  //  zoom in
  else if (ch == '+')
  {
    w -= 0.1;
    lorenzGen();
  }


  //  Tell GLUT it is necessary to redisplay the scene
  glutPostRedisplay();
}

// much of this was provided by the professor
void special(int key,int x,int y)
{
  //  Right arrow key - increase azimuth by 5 degrees
  if (key == GLUT_KEY_RIGHT)
    th += 5;
  //  Left arrow key - decrease azimuth by 5 degrees
  else if (key == GLUT_KEY_LEFT)
    th -= 5;
  //  Up arrow key - increase elevation by 5 degrees
  else if (key == GLUT_KEY_UP)
    ph += 5;
  //  Down arrow key - decrease elevation by 5 degrees
  else if (key == GLUT_KEY_DOWN)
    ph -= 5;
  //  Keep angles to +/-360 degrees
  th %= 360;
  ph %= 360;
  //  Tell GLUT it is necessary to redisplay the scene
  glutPostRedisplay();
}

// much of this was provided by the professor
void reshape(int width,int height)
{
  //  Ratio of the width to the height of the window
  double w2h = (height>0) ? (double)width/height : 1;
  int dim = 3;
  //  Set the viewport to the entire window
  glViewport(0,0, width,height);
  //  Tell OpenGL we want to manipulate the projection matrix
  glMatrixMode(GL_PROJECTION);
  //  Undo previous transformations
  glLoadIdentity();
  //  Orthogonal projection
  glOrtho(-dim*w2h,+dim*w2h, -dim,+dim, -dim,+dim);
  //  Switch to manipulating the model matrix
  glMatrixMode(GL_MODELVIEW);
  //  Undo previous transformations
  glLoadIdentity();
}

// This code was provided by the professor
int main(int argc, char *argv[])
{

  lorenzGen();
  //  Initialize GLUT and process user parameters
  glutInit(&argc,argv);
  //  Request double buffered, true color window 
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  //  Request 500 x 500 pixel window
  glutInitWindowSize(500,500);
  //  Create the window
  glutCreateWindow("Lorenz Attractor");
  //  Tell GLUT to call "display" when the scene should be drawn
  glutDisplayFunc(display);
  //  Tell GLUT to call "reshape" when the window is resized
  glutReshapeFunc(reshape);
  //  Tell GLUT to call "special" when an arrow key is pressed
  glutSpecialFunc(special);
  //  Tell GLUT to call "key" when a key is pressed
  glutKeyboardFunc(key);
  //  Pass control to GLUT so it can interact with the user
  glutMainLoop();
  //  Return code
  return 0;

}
