#include "khoom.hpp"
#include "world.hpp"

#define ESCAPE 27
#define PAGE_UP 73
#define PAGE_DOWN 81
#define UP_ARROW 72
#define DOWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77


polygon front(6);
polygon back(6);
quad m1, m2, m3, m4, m5, m6;

GLfloat rtri;
GLfloat rtrq;
GLfloat rtrp;
GLuint loop;             // general loop variable
GLuint texture[3];       // storage for 3 textures;
float something = 0.5f;

int light = 0;           // lighting on/off
int blend = 0;        // blending on/off

GLfloat xrot;            // x rotation
GLfloat yrot;            // y rotation
GLfloat xspeed;          // x rotation speed
GLfloat yspeed;          // y rotation speed

GLfloat walkbias = 0;
GLfloat walkbiasangle = 0;

GLfloat lookupdown = 0.0;
const float piover180 = 0.0174532925f;

float heading, xpos, zpos;

GLfloat camx = 0, camy = 0, camz = 0; // camera location.
GLfloat therotate;

GLfloat z=0.0f;                       // depth into the screen.

GLfloat LightAmbient[]  = {0.5f, 0.5f, 0.5f, 1.0f}; 
GLfloat LightDiffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f}; 
GLfloat LightPosition[] = {0.0f, 0.0f, 2.0f, 1.0f};

GLuint filter = 0;       // texture filtering method to use (nearest, linear, linear + mipmaps)

void init(int Width, int Height)
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
		

	

  front.addVertex(0, vertex(1.0f, 1.5f, 0.0f, 0.0f, 0.0f));//1
  front.addVertex(1, vertex( 1.5f, 0.0f, 0.0f, 0.0f, 0.0f));//2
  front.addVertex(2, vertex( 1.0f,-1.5f, 0.0f, 0.0f, 0.0f));//3
  front.addVertex(3, vertex(-1.0f,-1.5f, 0.0f, 0.0f, 0.0f));//4
  front.addVertex(4, vertex(-1.5f,0.0f, 0.0f, 0.0f, 0.0f));//5	
  front.addVertex(5, vertex(-1.0f,1.5f, 0.0f, 0.0f, 0.0f));//6	
	//back
  back.addVertex(0, vertex(1.0f, 1.5f, -1.5f, 0.0f, 0.0f));//1'
  back.addVertex(1, vertex( 1.5f, 0.0f, -1.5f, 0.0f, 0.0f));//2'
  back.addVertex(2, vertex( 1.0f,-1.5f, -1.5f, 0.0f, 0.0f));//3'
  back.addVertex(3, vertex(-1.0f,-1.5f, -1.5f, 0.0f, 0.0f));//4'
  back.addVertex(4, vertex(-1.5f,0.0f, -1.5f, 0.0f, 0.0f));//5'
  back.addVertex(5, vertex(-1.0f,1.5f, -1.5f, 0.0f, 0.0f));//6'	
	//middle1
  m1.addVertex(0, vertex(1.0f, 1.5f, 0.0f, 0.0f, 0.0f));//1
  m1.addVertex(1, vertex(1.0f, 1.5f, -1.5f, 0.0f, 0.0f));//1'
  m1.addVertex(2, vertex( 1.5f, 0.0f, -1.5f, 0.0f, 0.0f));//2'
  m1.addVertex(3, vertex( 1.5f, 0.0f, 0.0f, 0.0f, 0.0f));//2
	//middle2
  m2.addVertex(0, vertex( 1.5f, 0.0f, 0.0f, 0.0f, 0.0f));//2
  m2.addVertex(1, vertex( 1.5f, 0.0f, -1.5f, 0.0f, 0.0f));//2'
  m2.addVertex(2, vertex( 1.0f,-1.5f, -1.5f, 0.0f, 0.0f));//3'
  m2.addVertex(3, vertex( 1.0f,-1.5f, 0.0f, 0.0f, 0.0f));//3
	//middle3
  m3.addVertex(0, vertex( 1.0f,-1.5f, 0.0f, 0.0f, 0.0f));//3
  m3.addVertex(1, vertex( 1.0f,-1.5f, -1.5f, 0.0f, 0.0f));//3'
  m3.addVertex(2, vertex(-1.0f,-1.5f, -1.5f, 0.0f, 0.0f));//4'
  m3.addVertex(3, vertex(-1.0f,-1.5f, 0.0f, 0.0f, 0.0f));//4
	//middle4
  m4.addVertex(0, vertex(-1.0f,-1.5f, 0.0f, 0.0f, 0.0f));//4
  m4.addVertex(1, vertex(-1.0f,-1.5f, -1.5f, 0.0f, 0.0f));//4'
  m4.addVertex(2, vertex(-1.5f,0.0f, -1.5f, 0.0f, 0.0f));//5'
  m4.addVertex(3, vertex(-1.5f,0.0f, 0.0f, 0.0f, 0.0f));//5	
	//middle5
  m5.addVertex(0, vertex(-1.5f,0.0f, 0.0f, 0.0f, 0.0f));//5	
  m5.addVertex(1, vertex(-1.5f,0.0f, -1.5f, 0.0f, 0.0f));//5'
  m5.addVertex(2, vertex(-1.0f,1.5f, -1.5f, 0.0f, 0.0f));//6'	
  m5.addVertex(3, vertex(-1.0f,1.5f, 0.0f, 0.0f, 0.0f));//6	
	//middle6
  m6.addVertex(0, vertex(-1.0f,1.5f, 0.0f, 0.0f, 0.0f));//6	
  m6.addVertex(1, vertex(-1.0f,1.5f, -1.5f, 0.0f, 0.0f));//6'	
  m6.addVertex(2, vertex(1.0f, 1.5f, -1.5f, 0.0f, 0.0f));//1'
  m6.addVertex(3, vertex(1.0f, 1.5f, 0.0f, 0.0f, 0.0f));//1
}

void display()
{
    //GLfloat x_m, y_m, z_m, u_m, v_m;
    GLfloat xtrans, ztrans, ytrans;
    GLfloat sceneroty;
    //int triangle_count;

    xtrans = -xpos;
    ztrans = -zpos;
    ytrans = -walkbias-0.25f;
    sceneroty = 360.0f - yrot;
    	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
    glLoadIdentity();

    glRotatef(lookupdown, 1.0f, 0, 0);
    glRotatef(sceneroty, 0, 1.0f, 0);

    glTranslatef(xtrans, ytrans, ztrans);    

    //glBindTexture(GL_TEXTURE_2D, texture[filter]);   

    //triangle_count = sector1.triangle_count;

    //for (loop=0; loop<triangle_count; loop++) 
		//{     
	
    //}
		
		//glColor3f(something, 1.0f - something, something * 0.2); 
		//test.glPolygon();
		usleep(1000);
    
	rtri += 0.2;
	rtrq -=0.15;
	rtrp +=0.35;
	
	glTranslatef(0.0f,0.0f,-6.0f);		
	glRotatef(rtrp, 0.0f, 1.0f, 0.0f);

	glBegin(GL_POLYGON);
	front.glVertex(0,1.0f, 0.0f, 0.0f); 
	front.glVertex(1,0.0f, 1.0f, 0.0f); 
	front.glVertex(2,0.0f, 0.0f, 1.0f); 
	front.glVertex(3,1.0f, 1.0f, 0.0f); 
	front.glVertex(4,0.0f, 1.0f, 1.0f); 
	front.glVertex(5,1.0f, 0.0f, 1.0f);
  glEnd();	

	glBegin(GL_POLYGON);
	back.glVertex(0,0.5f, 0.0f, 0.0f); 
	back.glVertex(1,0.0f, 0.5f, 0.0f); 
	back.glVertex(2,0.0f, 0.0f, 0.5f); 
	back.glVertex(3,0.5f, 0.5f, 0.0f); 
	back.glVertex(4,0.0f, 0.5f, 0.5f); 
	back.glVertex(5,0.5f, 0.0f, 0.5f); 
  glEnd();	
	//middle1
	glBegin(GL_QUADS);
	m1.glVertex(0,1.0f, 0.0f, 0.0f); 
	m1.glVertex(1,0.5f, 0.0f, 0.0f); 
	m1.glVertex(2,0.0f, 0.5f, 0.0f); 
	m1.glVertex(3,0.0f, 1.0f, 0.0f); 
	//middle2
	m2.glVertex(0,0.0f, 1.0f, 0.0f); 
	m2.glVertex(1,0.0f, 0.5f, 0.0f); 
	m2.glVertex(2,0.0f, 0.0f, 0.5f); 
	m2.glVertex(3,0.0f, 0.0f, 1.0f); 
	//middle3
	m3.glVertex(0,0.0f, 0.0f, 1.0f); 
	m3.glVertex(1,0.0f, 0.0f, 0.5f); 
	m3.glVertex(2,0.5f, 0.5f, 0.0f); 
	m3.glVertex(3,1.0f, 1.0f, 0.0f); 
	//middle4
	m4.glVertex(0,1.0f, 1.0f, 0.0f); 
	m4.glVertex(1,0.5f, 0.5f, 0.0f); 
	m4.glVertex(2,0.0f, 0.5f, 0.5f); 
	m4.glVertex(3,0.0f, 1.0f, 1.0f); 
	//middle5
	m5.glVertex(0,0.0f, 1.0f, 1.0f); 
	m5.glVertex(1,0.0f, 0.5f, 0.5f); 
	m5.glVertex(2,0.5f, 0.0f, 0.5f); 
	m5.glVertex(3,1.0f, 0.0f, 1.0f); 
	//middle6
	m6.glVertex(0,1.0f, 0.0f, 1.0f); 
	m6.glVertex(1,0.5f, 0.0f, 0.5f); 
	m6.glVertex(2,0.5f, 0.0f, 0.0f); 
	m6.glVertex(3,1.0f, 0.0f, 0.0f); 
  glEnd();	
		glFlush();
    glutSwapBuffers();
}
void reshape(int width, int height)
{
  if (height==0)				
    height=1;

  glViewport(0, 0, width, height);		

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
}

void keyPressed(unsigned char key, int x, int y) 
{
    usleep(100);

    switch (key)
	 	{    
    case ESCAPE: 
			exit(1);                   	
		break; 
		}

}

void specialKeyPressed(int key, int x, int y) 
{
    usleep(100);

    switch (key) 
		{    
    case GLUT_KEY_PAGE_UP: // tilt up
			z -= 0.2f;
			lookupdown -= 0.2f;
		break;
    
    case GLUT_KEY_PAGE_DOWN: // tilt down
			z += 0.2f;
			lookupdown += 1.0f;
		break;

    case GLUT_KEY_UP: // walk forward (bob head)
			xpos -= (float)sin(yrot*piover180) * 0.05f;
			zpos -= (float)cos(yrot*piover180) * 0.05f;	
			if (walkbiasangle >= 359.0f)
	    		walkbiasangle = 0.0f;	
		else 
	  	  walkbiasangle+= 10;
			walkbias = (float)sin(walkbiasangle * piover180)/20.0f;
		break;

    case GLUT_KEY_DOWN: // walk back (bob head)
			xpos += (float)sin(yrot*piover180) * 0.05f;
			zpos += (float)cos(yrot*piover180) * 0.05f;	
			if (walkbiasangle <= 1.0f)
	    	walkbiasangle = 359.0f;	
		else 
	  	  walkbiasangle-= 10;
			walkbias = (float)sin(walkbiasangle * piover180)/20.0f;
		break;

    case GLUT_KEY_LEFT: // look left
			yrot += 1.5f;
		break;
    
    case GLUT_KEY_RIGHT: // look right
			yrot -= 1.5f;
		break;

    default:
			printf ("Special key %d pressed. No action there yet.\n", key);
		break;
    }	
}

void idle()
{
	something += 0.1;
	display();
}

int main(int argc, char *argv[])
{

		using namespace std;

		glutInit(&argc, argv);  
    glutInitWindowSize(640, 480);  
    glutInitWindowPosition(0, 0);  
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);  
    glutCreateWindow("Khoom - Khana Own Doom");  
		glutDisplayFunc(display);  
    glutFullScreen();
		glutIdleFunc(&idle); 
		glutReshapeFunc(&reshape);
		glutKeyboardFunc(&keyPressed);
		glutSpecialFunc(&specialKeyPressed);
		init(640, 480);
  
    glutMainLoop();  

    return 1;
}
