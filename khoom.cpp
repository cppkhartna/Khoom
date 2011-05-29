#include "khoom.hpp"
#include "world.hpp"
#include "reader.hpp"

#define ESCAPE 27
#define PAGE_UP 73
#define PAGE_DOWN 81
#define UP_ARROW 72
#define DOWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77


GLfloat rtri;
GLfloat rtrq;
GLfloat rtrp;
GLuint loop;             // general loop variable
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

//interior prism("prism.txt");
interior world("world.txt");
interior simple("simple.txt");

int bk_width;
int bk_width2;
int bk_height;
int bk_height2;
unsigned char *bk_bits;
unsigned char *bk_bits1;
unsigned char *bk_bits2;
GLuint texture[100];     // storage for 100 textures;


void init(int Width, int Height)
{
	using namespace std;
	ifstream file;      
	string oneline;
	int total = 0;
	int pos;

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	
  glClearDepth(1.0);				
  glDepthFunc(GL_LESS);			
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();			

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);

  glMatrixMode(GL_MODELVIEW);
		
	file.open("textures.txt");
	if (file.fail())
	{
		cout << "Error opening file: textures.txt" << endl;
		exit(1);
	}

	getline(file, oneline, '\n');
	sscanf(oneline.c_str(), "TEXTURES %d\n", &total);
    
	glEnable(GL_TEXTURE_2D);                    // Enable texture mapping.

	for (int i = 0; i < total; i++)
	{

		getline(file, oneline, '\n');
		if (oneline.length() > 0)
		{
			if((bk_bits1=LoadBMPFile(oneline.c_str(), &bk_width, &bk_height))== NULL)
			{
				cout << "Error loading texture: " << oneline << endl;	
				exit(20*i+2);
			}
			pos = oneline.find(".");
			oneline.replace(pos, 10, "_alpha.bmp");

			unsigned char *bk_bits = new unsigned char [bk_width*bk_height*4];

			bk_bits2=LoadBMPFile(oneline.c_str(), &bk_width2, &bk_height2);
			for (int j = 0; j < bk_width*bk_height; j++) 
			{
				bk_bits[4*j] = bk_bits1[3*j];
				bk_bits[4*j + 1] = bk_bits1[3*j + 1];
				bk_bits[4*j + 2] = bk_bits1[3*j + 2];
				if (bk_bits2 == NULL)
					bk_bits[4*j + 3] = (unsigned char) 255;
				else
				  bk_bits[4*j + 3] = (unsigned char)(((int)bk_bits2[3*j] + (int)bk_bits2[3*j + 1] + (int)bk_bits2[3*j + 2])/3);
			}
	
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &texture[i]);
			glActiveTexture(GL_TEXTURE0); 
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bk_width, bk_height, 0, GL_RGB,
							 //GL_UNSIGNED_BYTE,bk_bits);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, bk_width, bk_height, GL_RGBA,
							 GL_UNSIGNED_BYTE,bk_bits);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		}

	}
		
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


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


		usleep(1000);
    
	rtri += 0.2;
	rtrq -=0.15;
	rtrp +=0.35;
	
	glTranslatef(0.0f,0.0f,-6.0f);		
	glRotatef(rtrp, 0.0f, 1.0f, 0.0f);

	//paint
		//std::cout << "AAA\n";
	
	//LoadGLTexture(texture[0], "mud.bmp");
	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	world.display();
	//simple.display();
    

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

extern GLuint text[3];

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
