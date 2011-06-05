#include "khoom.hpp"
#include "world.hpp"

#define ESCAPE 27
#define PAGE_UP 73
#define PAGE_DOWN 81
#define UP_ARROW 72
#define DOWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77

#define BEZ_NUM 6

const double Pi = 3.1415;
const int FurCount = 16;
bool furred = false;
bool fire = false;

torus Thor(6, 2, 72, 34);
torus FurThors[FurCount];

int scene = 0;

GLenum GLTexture[GL_MAX_TEXTURE_UNITS_ARB]; //dirty hack (because I can't see any other way to use GL_TEXTUREn_ARB);

float timer; //дельта времени

GLfloat rtri;
GLfloat rtrq;
GLfloat rtrp;
GLuint loop;             // general loop variable
float something = 0.5f;
int divs;
float t = 0;

double x_angle = 30;
double y_angle = 0;

int light = 0;           // lighting on/off
int blend = 0;        // blending on/off
bool lines = true; //for mountains

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

GLfloat LightAmbient[]  = {0.0f, 0.0f, 0.0f, 0.0f}; 
GLfloat LightDiffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f}; 
GLfloat LightPosition[] = {0.0f, 1.0f, 0.0f, 1.0f};

//interior prism("prism.txt");
interior world("world.txt");
interior simple("simple.txt");
//interior land("map3_landscape.txt");
//bpatch bezier("bezier.txt");
//interior Fichte("Fichte.txt");
//interior Tree("tree.txt");
//interior cube("cube.txt");
interior ob("object.txt");
interior bg("bg.txt");

//const int GroundSize = 5;
//const double TreeSize = 1.0;
//const int treeCount = 30;

//double fichten[treeCount];

int bk_width;
int bk_width2;
int bk_height;
int bk_height2;
unsigned char *bk_bits;
unsigned char *bk_bits1;
unsigned char *bk_bits2;
GLuint texture[100];     // storage for 100 textures;

unsigned char* map;
int mapx;
int mapy;

//GLuint cube;
//GLuint torus;
//GLuint fichte;
//GLuint tree;


void landscape(const char* path, float x0, float y0)
{
	using namespace std;
	if((map=LoadBMPFile(path, &mapx, &mapy))== NULL)
			{
				cout << "Error loading texture: " << path << endl;	
				exit(33);
			}
	ofstream file;      
	string oneline;
	int total = 0;
	int save_j = 0;
	int pos;

	oneline = path;
	pos = oneline.find(".");
	oneline.replace(pos, 30, "_landscape.txt");

	file.open(oneline.c_str());
	if (file.fail())
	{
		cout << "Error opening file:" << oneline << endl;
		exit(1);
	}

	file << "TOTAL " << mapx*mapy << endl;
	file << "QUADS " << mapx*mapy << endl;

	for (int j = 0; j < mapx*mapy; j++) 
	{
		if (j == mapx) 
			j++;

		if (!(map[3*j] >= 250 && map[3*j+1] >= 250 && map[3*j+2] >= 250)
			&& !(map[3*j] <= 5 && map[3*j+1] <= 5 && map[3*j+2] <= 5))
		{
			save_j = j;
			for (int k = 0; k < 4; k++)
			{
					switch (k)
					{
						case 0:
							j = save_j;
						break;
						case 1:
							j = save_j + 1;
						break;
						case 2:
							j = save_j + mapx + 1;
						break;
						case 3:
							j = save_j + mapx;
						break;
					}
					file << (float) x0 -  (j/(mapx-1)) << " " << 
				  (float) ((3.4/1.4)*sqrt(map[3*j]*map[3*j] + (255-map[3*j+1])*(255-map[3*j+1]))/255 - map[3*j+2]/255 + 0.6)*5 - 20  << " "
					<< (float) y0 - (j % mapy)
					<< " 0.0 0.0 " << (float) map[3*j]/255 << " " << (float) map[3*j+1]/255 << " " << (float) map[3*j+2]/255 << endl ;
			}
			j = save_j;
			total++;
		}
	}
	
	file << "TEXTURES 0" << endl;

	file.seekp(0);
	file << "TOTAL " << total << endl;
	file << "QUADS " << total << endl;
	file.close();
	
}

void init(int Width, int Height)
{
	using namespace std;
	ifstream file;      
	string oneline;
	int total = 0;
	int pos;

//Load textures		
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
			oneline.replace(pos, 20, "_alpha.bmp");

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

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, bk_width, bk_height, GL_RGBA,
							 GL_UNSIGNED_BYTE,bk_bits);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		}

	}

	if (bk_bits != NULL)
		delete[] bk_bits;
	if (bk_bits1 != NULL)
		delete[] bk_bits1;
	if (bk_bits2 != NULL)
		delete[] bk_bits2;
  
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	
  glClearDepth(1.0);				
  glDepthFunc(GL_LESS);			
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();			

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);

  glMatrixMode(GL_MODELVIEW);
		
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LIGHTING);
    
	
	//for (int i = 0; i < treeCount; i++)
		//fichten[i] = -GroundSize + TreeSize + 2*rand()*(GroundSize - TreeSize)/RAND_MAX;

// основные display lists
	
	//cube = gen::cube();
	//torus = gen::torus(2.0,0.5,25,20); 
	//fichte = gen::other(Fichte);
	//tree = gen::other(Tree);
	
	//world.divide(1000);

	Thor.addTexture(6);
	Thor.addTexture(8);

	for (int i = 0; i < FurCount; i++)
	{
		FurThors[i].settorus(6, 2*(1 + (float) i/FurCount * 0.5), 32, 74);
		FurThors[i].addTexture(10);
	}

}

GLfloat dir[] = {0,-1,0,1};

vertex GetPos(int x, int y)
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float) x;
	winY = (float)viewport[3] - (float) y;
	glReadPixels(x, (int) winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

	gluUnProject(winX, winY, winZ, modelview, projection, viewport, 
						  &posX, &posY, &posZ);

	return vertex(posX, posY, posZ, 0, 0, 0, 0, 0);
}


void display()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  GLfloat xtrans, ztrans, ytrans;
  GLfloat sceneroty;

  xtrans = -xpos;
  ztrans = -zpos;
  ytrans = -walkbias-1.0f;
  sceneroty = 360.0f - yrot;
    	
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
  glLoadIdentity();

  glRotatef(lookupdown, 1.0f, 0, 0);
  glRotatef(sceneroty, 0, 1.0f, 0);
	
  glTranslatef(xtrans, ytrans, ztrans);    

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);  // add lighting. (ambient)
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);  // add lighting. (diffuse).
  glLightfv(GL_LIGHT1, GL_POSITION,LightPosition); // set light position.
  glEnable(GL_LIGHT1);                             // turn light 1 on.

	//волны с помощью B-spline (кривых Безье)
	
	//glTranslatef(0, 0, -20);
	//int k = 1;

	//bezier.dlBPatch = gen::bezier(bezier);
	//for (int j = 0; j < BEZ_NUM; j++)
	//{
		//glTranslatef(-1.50*BEZ_NUM+0.75*k, 0, 1.50);
		//for (int i = 0; i < BEZ_NUM; i++)
		//{
			//glTranslatef(1.50, 0, 0);
			//bezier.display();
		//}	
		//k = -k;
	//}
	
	//glTranslatef(0, 0, 10);
	//glRotatef(180*piover180, 0, 1, 0);

	//комната
	//glTranslatef(1.5, 0.5, -1.5);
	//glTranslatef(-1.5, -0.5, 1.5);
	// горы, сгенерированные из bmp
	//land.display(lines);

	//хвойный лес из billboards
	
	//glTranslatef(10, 0, 10);
	//glRotatef(180*piover180, 0, 1, 0);
		
	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.05);
	
	//for (int i = 0; i < treeCount; i++) 
	//{
		//int index = (yrot > 180) ? (i) : (treeCount - 1 - i);
		//double x = -GroundSize + TreeSize + 2*(GroundSize - TreeSize)*index/(treeCount - 1);
		//double z = fichten[index];

		//glPushMatrix();

	//	//glLoadIdentity();
		//glTranslated(0,0,-3*GroundSize);
		//glRotated(xrot,1,0,0);
		//glRotated(yrot,0,1,0);
		//glTranslated(x,0,z);
		//glRotated(-yrot,0,1,0);
		//glRotated(-xrot,1,0,0);

		//Fichte.display();

		//glPopMatrix();
	//}
	
	//glDisable(GL_ALPHA_TEST);
	
	//glTranslatef(-10, 0, -10);

  //usleep(1000);
	//полупрозрачный объект с упорядоченным выводом граней
	//vertex* viewer = new vertex(GetPos(0, 0));
	//ob.display(viewer);
	//delete viewer;


	switch (scene)
	{
		case 0:
		{
			glPushMatrix();
			glScalef(50, 50, 50);	
			//рисуем фон
			bg.display();
			glScalef(0.02, 0.02, 0.02);	
    
			rtri += 0.3;
			rtrq +=0.35;
			rtrp +=0.35;
	
			//задаём вращение
			glTranslatef(0.0f,0.0f, -27.0f);		
			glRotatef(rtri, 1.0f, 0.0f, 0.0f);
			glRotatef(rtrq, 0.0f, 1.0f, 0.0f);
			glRotatef(rtrp, 0.0f, 0.0f, 1.0f);


			// рисуем сферотор
  		Thor.display();	

			if (furred)
			{

				float shadowMin = 0.2;
				float shadowMax = 0.5;

				for (int i = 0; i < FurCount; i++)
				{
					float t = (float) i / FurCount;
					float shadow = shadowMin*(1-t) + shadowMax;
					glColor4f(1, 1, 1, shadow);

					if (fire)
						glBlendFunc(GL_SRC_ALPHA, GL_ONE);
					else
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

					FurThors[i].display();

				}

			}

			//Восстанавливаем матрицы
			glPopMatrix(); 
		}

	}
	
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
		int i;

		switch (scene)
		{
		case 0:
   		switch (key) 
			{    
   		case ESCAPE: 
				exit(1);                   	
			break; 
			case '=': case '+':
				Thor.sphere();   
				if (furred)
				{
					for (i = 0; i < FurCount; i++)
						FurThors[i].sphere();
				}				
			break; 
      case '-': 
				Thor.unsphere();                   	
				if (furred)
				{
					for (i = 0; i < FurCount; i++)
						FurThors[i].unsphere();
				}				
			break; 
			case'S':case 's':
				Thor.sphere(true);                   	
				if (furred)
				{
					for (i = 0; i < FurCount; i++)
						FurThors[i].sphere(true);
				}				
			break; 
			case 'U':case 'u': 
				Thor.unsphere(true);                   	
				if (furred)
				{
					for (i = 0; i < FurCount; i++)
						FurThors[i].unsphere(true);
				}				
			break; 
			case 'F': case 'f':
			if (!furred)
			{
				Thor.deleteLastTexture();
				Thor.textures[0] = 9;
			 	furred = true;	
			}
			else
			{
				Thor.addTexture(8);
				Thor.textures[0] = 6;
			 	furred = false;	
			}
			break;
			case 'O': case 'o':
				if (furred)
				{
					fire = !fire;
				}
			break;
			}
		break;
		}

				//hills:
        //case 'H': 
			 //case 'h':
				//lines = !lines;                   	
				//break; 
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
	t+=0.1;
	divs = 7 + (int) (7.0*sin(t));
	display();
}

int main(int argc, char *argv[])
{
		using namespace std;

		GLTexture[0] = GL_TEXTURE0_ARB;

		for (int i = 1; i < 32; i++)
		{
			GLTexture[i] = GLTexture[i-1] + 1;
		}
		
		//ifstream file;      
		//file.open("map3_landscape.txt");
		//if (file.fail())
		//{
			//landscape("map3.bmp", 55, 55);
		//}
		//else
			//file.close();

		glutInit(&argc, argv);  
    glutInitWindowSize(640, 480);  
    glutInitWindowPosition(0, 0);  
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);  
    glutCreateWindow("Khoom");  
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
