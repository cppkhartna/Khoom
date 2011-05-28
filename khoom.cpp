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

interior prism("prism.txt");
interior world("world.txt");

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
		
	//addVertex

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

		//glColor3f(0.5, 1.0f,0.2); 
		//world.display();

    //glBindTexture(GL_TEXTURE_2D, texture[filter]);   

    //triangle_count = sector1.triangle_count;

    //for (loop=0; loop<triangle_count; loop++) 
		//{     
	
    //}
		
		//test.glPolygon();
		usleep(1000);
    
	rtri += 0.2;
	rtrq -=0.15;
	rtrp +=0.35;
	
	glTranslatef(0.0f,0.0f,-6.0f);		
	glRotatef(rtrp, 0.0f, 1.0f, 0.0f);

	//paint
		//std::cout << "AAA\n";
	glColor3f(0.5f, 1.0f,0.2f); 
	world.display();

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
