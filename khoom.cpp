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

const int AXIS = 0;
const int CENTER = 1;
const int ANGLE = 2;
const int W = 3;
const int VELOCITY = 4;
const int ACCELERATION = 5;
const int SCALE = 6;
int gtime, save_time;

extern vertex mult(GLfloat* m, GLfloat* v);

bool fog = false;
GLfloat fogColor[] = {0.5, 0.5, 0.6};
float start, end;

int total = 0;
GLuint cubic;
GLuint bumpMap;
GLuint normCubeMap;
bool cub = false;
bool b = true;
int save;

bool nature = false;

bool bumps = false;
bool bumps2 = false;
GLfloat Minv[16];
vertex Light;

const double Pi = 3.1415;
const int FurCount = 16;
bool furred = false;
bool fire = false;

const int R = 6;
const int r = 2;

torus Thor(R, r, 72, 34);
torus FurThors[FurCount];

int scene = 1;

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

bool light = false;   
int blend = 0;        // blended object on/off
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

GLfloat LightAmbient[]  = {0.5f, 0.5f, 0.5f, 1.0f}; 
GLfloat LightDiffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f}; 
GLfloat LightPosition1[] = {0.0f, 0.85f, 0.0f, 1.0f};
GLfloat LightPosition2[] = {0.0f, 0.5f, 0.0f, 0.0f};

//interior prism("prism.txt");
interior world("world.txt");
//object World(world);
interior Floor("Floor.txt");
interior simple("simple.txt");
interior land("map3_landscape.txt");
bpatch bezier("bezier.txt");
interior Fichte("Fichte.txt");
interior cube("cube.txt");
//object Cube(cube);
//object Cube2(cube);
interior ob("object.txt");
interior bg("bg.txt");
interior top("bg-top.txt");
interior bottom("bg-bottom.txt");
interior box("box.txt");

const int GroundSize = 5;
const double TreeSize = 1.0;
const int treeCount = 30;

double fichten[treeCount];

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

void getCubeVector(int side, int cubesize, int i, int j, 
								float& x, float& y, float& z)
{
	float s = ((float) i + 0.5f) / (float) cubesize;
	float t = ((float) j + 0.5f) / (float) cubesize;
	float sc = 2*s - 1;
	float tc = 2*t - 1;

	switch (side)
	{
		case 0:
			x = 1;
			y = -tc;
			z = -sc;
		break;
		case 1:
			x = -1;
			y = -tc;
			z = sc;
		break;
		case 2:
			x = sc;
			y = 1;
			z = tc;
		break;
		case 3:
			x = sc;
			y = -1;
			z = -tc;
		break;
		case 4:
			x = sc;
			y = -tc;
			z = 1;
		break;
		case 5:
			x = -sc;
			y = -tc;
			z = -1;
		break;
	
	}
	float normal = sqrt(sc*sc + tc*tc + 1); 
	if (normal != 0)
		x /= normal;
		y /= normal;
		z /= normal;
}

void createNormalizationCubeMap(int cubesize)
{
	unsigned char* pixels = new unsigned char[3*cubesize*cubesize];
	float x, y, z;

	glGenTextures(1, &normCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, normCubeMap);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (int side = 0; side < 6; side++)
	{
		for (int i = 0; i < cubesize; i++)
		{
			for (int j = 0; j < cubesize; j++)
			{
				int offs = 3 * (j * cubesize + i);

				getCubeVector(side, cubesize, i, j, x, y, z);
				pixels [offs] = 128 + 127 * x;
				pixels [offs + 1] = 128 + 127 * y;
				pixels [offs + 2] = 128 + 127 * z;
			}
		}
		//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB + side, 0, GL_RGB, 
								//cubesize, cubesize, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB+side, GL_RGB8, cubesize, cubesize, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	}
	delete[] pixels;

	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glDisable(GL_TEXTURE_CUBE_MAP_ARB);

}

void LoadCubicMaps()
{
	using namespace std;
	ifstream file;      
	string oneline;

//Load textures		
	file.open("cubictextures.txt");
	if (file.fail())
	{
		cout << "Error opening file: cubictextures.txt" << endl;
		exit(13);
	}

	total++;
	cubic = total;
	glGenTextures(1, &texture[total]);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, texture[total]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; i++)
	{
		getline(file, oneline, '\n');
			
		if((bk_bits=LoadBMPFile(oneline.c_str(), &bk_width, &bk_height))== NULL)
		{
			cout << "Error loading texture: " << oneline << endl;	
			exit(40*i+2);
		}
	
		gluBuild2DMipmaps(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB+i, GL_RGB8, bk_width, bk_height, GL_RGB, GL_UNSIGNED_BYTE, bk_bits);

		if (bk_bits != NULL)	
			delete[] bk_bits;
	}
	
	glEnable(GL_TEXTURE_CUBE_MAP_ARB);
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glDisable(GL_TEXTURE_CUBE_MAP_ARB);
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
    
	glEnable(GL_TEXTURE_2D);  

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
			
			if (oneline.find("normal") != oneline.npos)
			{
				bumpMap = texture[i];
				if (bk_width != bk_height)
					exit(bk_width);
				createNormalizationCubeMap(bk_height);
			}
		}

	}
	
	glDisable(GL_TEXTURE_2D);  

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
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_LIGHTING);
	glClearStencil(0);
    
	
	for (int i = 0; i < treeCount; i++)
		fichten[i] = -GroundSize + TreeSize + 2*rand()*(GroundSize - TreeSize)/RAND_MAX;

	
	world.divide(100);
	Floor.divide(100);

	Thor.addTexture(6);
	Thor.addTexture(8);

	for (int i = 0; i < FurCount; i++)
	{
		FurThors[i].settorus(R, r*(1 + (float) i/FurCount * 0.5), 32, 74);
		FurThors[i].addTexture(10);
	}

	LoadCubicMaps();
	
	glLightfv(GL_LIGHT2, GL_AMBIENT, LightAmbient);  
  glLightfv(GL_LIGHT2, GL_DIFFUSE, LightDiffuse);  
  glLightfv(GL_LIGHT2, GL_POSITION, LightPosition2);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);  
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);  
	glEnable(GL_LIGHT1);
	
	//Cube2.set(SCALE, vertex(0.25, 0.25, 0.25));
	//Cube2.set(CENTER, vertex(0.5, 0.5, 0.5));
	//Cube.set(SCALE, vertex(0.1125, 0.1125, 0.1125));
	//Cube.set(CENTER, vertex(1.5, 0.4, 1.5));

}


//GLfloat dir[] = {0,-1,0,1};

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

void RenderObjects(bool dir)
{
	world.display();
	//Cube.display(LightPosition1, dir);
	//Cube2.display(LightPosition1, dir);

	if (blend)
	{
	//полупрозрачный объект с упорядоченным выводом граней
		glEnable(GL_BLEND);
		vertex* viewer = new vertex(GetPos(0, 0));
		ob.display(viewer);
		delete viewer;
		glDisable(GL_BLEND);
	}

}


void display()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  GLfloat xtrans, ztrans, ytrans;
  GLfloat sceneroty;

  xtrans = -xpos;
  ztrans = -zpos;
  ytrans = -walkbias-0.5f;
  sceneroty = 360.0f - yrot;
    	
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);		
	double eqr[] = {0.0f,-1.0f, 0.0f, 0.0f};

  glLoadIdentity();

  glRotatef(lookupdown, 1.0f, 0, 0);
  glRotatef(sceneroty, 0, 1.0f, 0);
	
  glTranslatef(xtrans, ytrans, ztrans);    

  glLightfv(GL_LIGHT1, GL_POSITION, LightPosition1);
	
	//рисуем фон
	glPushMatrix();
	glScalef(50, 50, 50);	
	bg.display();
	top.display();
	bottom.display();
	glScalef(0.02, 0.02, 0.02);	
	glPopMatrix();

	switch (scene)
	{
		case 0:
		{
  		glDisable(GL_LIGHT1);                           
  		glEnable(GL_LIGHT2);                           

			rtri += 0.3;
			rtrq +=0.35;
			rtrp +=0.35;
	
			//задаём вращение
			glTranslatef(0.0f,0.0f, -27.0f);		
			glRotatef(rtri, 1.0f, 0.0f, 0.0f);
			glRotatef(rtrq, 0.0f, 1.0f, 0.0f);
			glRotatef(rtrp, 0.0f, 0.0f, 1.0f);


			// рисуем сферотор
			
			if (bumps)
			{
				//координаты источника света в с.к. объекта
				glPushMatrix();
				glLoadIdentity();
				glTranslatef(0.0f,0.0f, 27.0f);		
				glRotatef(-rtri, 1.0f, 0.0f, 0.0f);
				glRotatef(-rtrq, 0.0f, 1.0f, 0.0f);
				glRotatef(-rtrp, 0.0f, 0.0f, 1.0f);
				glGetFloatv(GL_MODELVIEW_MATRIX, Minv);
				Light = mult(Minv, LightPosition2);
				glPopMatrix();

				glDisable(GL_BLEND);
				Thor.display();
				glBlendFunc(GL_DST_COLOR, GL_ZERO);
				glEnable(GL_BLEND);
				bumps = false;
				bumps2 = true;
				Thor.display();
				bumps = true;
				bumps2 = false;
			}
			else if (!cub)
			{
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
	
						//if (fire)
							glBlendFunc(GL_SRC_ALPHA, GL_ONE);
						//else
							//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
						FurThors[i].display();
	
					}
	
				}
			}
			else
			{
			//кубические текстуры в режиме окружающей среды
				glActiveTextureARB(GL_TEXTURE5_ARB);
				glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
				glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
				glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_ARB);
				glEnable(GL_TEXTURE_CUBE_MAP_ARB);
				glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, texture[cubic]);
				glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
				glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
				glEnable(GL_TEXTURE_GEN_R);

				//сделаем текстуру тора полупрозрачной
				save = Thor.textures[0];
				Thor.textures[0] = 0;
				if (b)
				{
						Thor.sphere(true);
						b = !b;
				}
				Thor.display();
				Thor.textures[0] = save;

				glActiveTextureARB(GL_TEXTURE5_ARB);
				glDisable(GL_TEXTURE_GEN_S);
				glDisable(GL_TEXTURE_GEN_T);
				glDisable(GL_TEXTURE_GEN_R);
				glDisable(GL_TEXTURE_CUBE_MAP_ARB);
			}

			//Восстанавливаем матрицы
			glPopMatrix(); 
		}
		break;
		case 1:
		{
			bumps = false;
			bumps2 = false;
			glDisable(GL_CULL_FACE);
  		glDisable(GL_LIGHT2);                           
  		glLightfv(GL_LIGHT1, GL_POSITION, LightPosition1);
  		glEnable(GL_LIGHT1);                           
			//glEnable(GL_LIGHTING);
	
			//задаём туман
			if (fog)
			{
				start = 0.4;
				end = 0.0;
				glEnable(GL_FOG);
				//glFogi(GL_FOG_MODE, GL_LINEAR);
				glFogfv(GL_FOG_COLOR, fogColor);
				glFogf(GL_FOG_START, start);
				glFogf(GL_FOG_END, end);
				glFogi(GL_FOG_HINT, GL_NICEST); // должен быть попиксельным
				glHint(GL_FOG_HINT, GL_NICEST); // должен быть попиксельным
				glFogi(GL_FOG_COORDINATE_SOURCE_EXT, GL_FOG_COORDINATE_EXT);
			}	
			else 
				glDisable(GL_FOG);

			glPushMatrix();
	
			//комната		

			glEnable(GL_STENCIL_TEST);	
			glStencilFunc(GL_ALWAYS, 1, 1);	
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);	
			glDisable(GL_DEPTH_TEST);					
			
			Floor.display();			
													
			glEnable(GL_DEPTH_TEST);
			glColorMask(1,1,1,1);		
			glStencilFunc(GL_EQUAL, 1, 1);			
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);		
			glEnable(GL_CLIP_PLANE0);							
			glClipPlane(GL_CLIP_PLANE0, eqr);			

			glPushMatrix();	
					glScalef(1.0f, -1.0f, 1.0f);	
					glLightfv(GL_LIGHT1, GL_POSITION, LightPosition1);
					
					RenderObjects(false);

			glPopMatrix();

			glDisable(GL_CLIP_PLANE0);
			glDisable(GL_STENCIL_TEST);	
			glLightfv(GL_LIGHT1, GL_POSITION, LightPosition1);
			glEnable(GL_BLEND);								
			glDisable(GL_LIGHTING);					
			glColor4f(1.0f, 1.0f, 1.0f, 0.8f);				
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
			Floor.display();		
	
			if (light)
				glEnable(GL_LIGHTING);
			glDisable(GL_BLEND);				

			RenderObjects(true);									

			glPopMatrix();

	// природа		
	
			if (nature)
			{
	
	//горы, сгенерированные из bmp
	glDisable(GL_TEXTURE_2D);
	land.display(NULL, lines);
	glEnable(GL_TEXTURE_2D);
	//волны с помощью B-spline (кривых Безье)

	glPushMatrix();	
	glTranslatef(3.0, 0, -13.0);
	int k = 1;

	bezier.dlBPatch = gen::bezier(bezier);
	for (int j = 0; j < BEZ_NUM; j++)
	{
		glTranslatef(-1.50*BEZ_NUM+0.75*k, 0, 1.50);
		for (int i = 0; i < BEZ_NUM; i++)
		{
			glTranslatef(1.50, 0, 0);
			bezier.display();
		}	
		k = -k;
	}

	glPopMatrix();
	
	
	//хвойный лес из billboards
	
	glPushMatrix();	
	glTranslatef(-3.0, 0, -3.0);
	glRotatef(-90, 0.0, 1.0, 0.0);
		
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.05);
	
	for (int i = 0; i < treeCount; i++) 
	{
		int index = (yrot > 180) ? (i) : (treeCount - 1 - i);
		double x = -GroundSize + TreeSize + 2*(GroundSize - TreeSize)*index/(treeCount - 1);
		double z = fichten[index];

		glPushMatrix();

		//glLoadIdentity();
		glTranslated(0,0,-3*GroundSize);
		glRotated(xrot,1,0,0);
		glRotated(yrot,0,1,0);
		glTranslated(x,0,z);
		glRotated(-yrot,0,1,0);
		glRotated(-xrot,1,0,0);

		Fichte.display();

		glPopMatrix();
	}
	
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();
		}
	
		}
		break;
		default:
			scene = 0;
		break;
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
			case 'C': case 'c':
				cub = !cub;
			break;
			case 'B': case 'b':
				b = !b;
			break;
			case 'T': case 't':
				bumps = !bumps;
			break;
			case '1': 
				scene = 1;
			break;
			}
		case 1:
   		switch (key) 
			{    
   		case ESCAPE: 
				exit(1);                   	
			break; 
    	case 'l': case 'L': 
			{
				light = !light; 
				if (!light) 
	    		glDisable(GL_LIGHTING);
	 			else 
	   	 		glEnable(GL_LIGHTING);
			}
			break;
			//hills:
			case 'H':  case 'h':
				lines = !lines;                   	
			break; 
			case 'y': case 'Y':
				fog = !fog;
			break;
			case '0': 
				scene = 0;
			break;
			case 'n': case 'N' :
				nature = !nature;
			break;
			case 'b': case 'B' :
				blend = !blend;
			break;
	
			}
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
			//printf ("Special key %d pressed. No action there yet.\n", key);
		break;
    }	
}

void idle()
{
	something += 0.1;
	t+=0.1;
	divs = 7 + (int) (7.0*sin(t));
	display();
	
	gtime = glutGet(GLUT_ELAPSED_TIME);
	timer = (save_time - gtime)/1000.0; // секунда
	save_time = gtime;
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
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			cout << "glewInit failed!" << endl;
			exit(42);
		}
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
