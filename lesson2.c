#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <unistd.h>     // Header File For sleeping.

/* ASCII code for the escape key. */
#define ESCAPE 27

/* The number of our GLUT window */
int window; 
GLfloat rtri;
GLfloat rtrq;
GLfloat rtrp;

void InitGL(int Width, int Height)
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	
  glClearDepth(1.0);				
  glDepthFunc(GL_LESS);			
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();			

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);

  glMatrixMode(GL_MODELVIEW);
}

void ReSizeGLScene(int Width, int Height)
{
  if (Height==0)				
    Height=1;

  glViewport(0, 0, Width, Height);		

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
}

void DrawGLScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
  glLoadIdentity();	


	rtri += 0.2;
	rtrq -=0.15;
	rtrp +=0.35;
	
  glLoadIdentity();	
  glTranslatef(0.0f,0.0f,-6.0f);		
	glRotatef(rtrp, 0.0f, 1.0f, 0.0f);
  glBegin(GL_POLYGON);			
	//front
	glColor3f(1.0f, 0.0f, 0.0f); 
  glVertex3f(1.0f, 1.5f, 0.0f);//1
	glColor3f(0.0f, 1.0f, 0.0f); 
  glVertex3f( 1.5f, 0.0f, 0.0f);//2
	glColor3f(0.0f, 0.0f, 1.0f); 
  glVertex3f( 1.0f,-1.5f, 0.0f);//3
	glColor3f(1.0f, 1.0f, 0.0f); 
  glVertex3f(-1.0f,-1.5f, 0.0f);//4
	glColor3f(0.0f, 1.0f, 1.0f); 
  glVertex3f(-1.5f,0.0f, 0.0f);//5	
	glColor3f(1.0f, 0.0f, 1.0f); 
  glVertex3f(-1.0f,1.5f, 0.0f);//6	
  glEnd();					
	//back
  glBegin(GL_POLYGON);			
	glColor3f(0.5f, 0.0f, 0.0f); 
  glVertex3f(1.0f, 1.5f, -1.5f);//1'
	glColor3f(0.0f, 0.5f, 0.0f); 
  glVertex3f( 1.5f, 0.0f, -1.5f);//2'
	glColor3f(0.0f, 0.0f, 0.5f); 
  glVertex3f( 1.0f,-1.5f, -1.5f);//3'
	glColor3f(0.5f, 0.5f, 0.0f); 
  glVertex3f(-1.0f,-1.5f, -1.5f);//4'
	glColor3f(0.0f, 0.5f, 0.5f); 
  glVertex3f(-1.5f,0.0f, -1.5f);//5'
	glColor3f(0.5f, 0.0f, 0.5f); 
  glVertex3f(-1.0f,1.5f, -1.5f);//6'	
  glEnd();					
	//middle1
  glBegin(GL_QUADS);			
	glColor3f(1.0f, 0.0f, 0.0f); 
  glVertex3f(1.0f, 1.5f, 0.0f);//1
	glColor3f(0.5f, 0.0f, 0.0f); 
  glVertex3f(1.0f, 1.5f, -1.5f);//1'
	glColor3f(0.0f, 0.5f, 0.0f); 
  glVertex3f( 1.5f, 0.0f, -1.5f);//2'
	glColor3f(0.0f, 1.0f, 0.0f); 
  glVertex3f( 1.5f, 0.0f, 0.0f);//2
	//middle2
	glColor3f(0.0f, 1.0f, 0.0f); 
  glVertex3f( 1.5f, 0.0f, 0.0f);//2
	glColor3f(0.0f, 0.5f, 0.0f); 
  glVertex3f( 1.5f, 0.0f, -1.5f);//2'
	glColor3f(0.0f, 0.0f, 0.5f); 
  glVertex3f( 1.0f,-1.5f, -1.5f);//3'
	glColor3f(0.0f, 0.0f, 1.0f); 
  glVertex3f( 1.0f,-1.5f, 0.0f);//3
	//middle3
	glColor3f(0.0f, 0.0f, 1.0f); 
  glVertex3f( 1.0f,-1.5f, 0.0f);//3
	glColor3f(0.0f, 0.0f, 0.5f); 
  glVertex3f( 1.0f,-1.5f, -1.5f);//3'
	glColor3f(0.5f, 0.5f, 0.0f); 
  glVertex3f(-1.0f,-1.5f, -1.5f);//4'
	glColor3f(1.0f, 1.0f, 0.0f); 
  glVertex3f(-1.0f,-1.5f, 0.0f);//4
	//middle4
	glColor3f(1.0f, 1.0f, 0.0f); 
  glVertex3f(-1.0f,-1.5f, 0.0f);//4
	glColor3f(0.5f, 0.5f, 0.0f); 
  glVertex3f(-1.0f,-1.5f, -1.5f);//4'
	glColor3f(0.0f, 0.5f, 0.5f); 
  glVertex3f(-1.5f,0.0f, -1.5f);//5'
	glColor3f(0.0f, 1.0f, 1.0f); 
  glVertex3f(-1.5f,0.0f, 0.0f);//5	
	//middle5
	glColor3f(0.0f, 1.0f, 1.0f); 
  glVertex3f(-1.5f,0.0f, 0.0f);//5	
	glColor3f(0.0f, 0.5f, 0.5f); 
  glVertex3f(-1.5f,0.0f, -1.5f);//5'
	glColor3f(0.5f, 0.0f, 0.5f); 
  glVertex3f(-1.0f,1.5f, -1.5f);//6'	
	glColor3f(1.0f, 0.0f, 1.0f); 
  glVertex3f(-1.0f,1.5f, 0.0f);//6	
	//middle6
	glColor3f(1.0f, 0.0f, 1.0f); 
  glVertex3f(-1.0f,1.5f, 0.0f);//6	
	glColor3f(0.5f, 0.0f, 0.5f); 
  glVertex3f(-1.0f,1.5f, -1.5f);//6'	
	glColor3f(0.5f, 0.0f, 0.0f); 
  glVertex3f(1.0f, 1.5f, -1.5f);//1'
	glColor3f(1.0f, 0.0f, 0.0f); 
  glVertex3f(1.0f, 1.5f, 0.0f);//1
	
	
  glEnd();					


  glutSwapBuffers();
}

void keyPressed(unsigned char key, int x, int y) 
{
    usleep(100);

    if (key == ESCAPE) 
    { 
	glutDestroyWindow(window); 
	
	exit(0);                   
    }
}

int main(int argc, char **argv) 
{  
  glutInit(&argc, argv);  

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  
  glutInitWindowSize(640, 480);  

  glutInitWindowPosition(0, 0);  

  window = glutCreateWindow("OpenGL");
  glutDisplayFunc(&DrawGLScene);  
  glutFullScreen();
  glutIdleFunc(&DrawGLScene);
  glutReshapeFunc(&ReSizeGLScene);
  glutKeyboardFunc(&keyPressed);
  InitGL(640, 480);
  glutMainLoop();  

  return 1;
}
