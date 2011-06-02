#include "world.hpp"

//world является интерфейсом игры

//vertex

vertex::vertex(const vertex& vert)
{
	x = vert.x;
	y = vert.y;
	z = vert.z;
	u = vert.u;
	v = vert.v;
	r = vert.r;
	g = vert.g;
	b = vert.b;
}

vertex::vertex(float c_x, float c_y, float c_z, float c_u, float c_v,
							 float c_r, float c_g, float c_b)
{
	x = c_x;
	y = c_y;
	z = c_z;
	u = c_u;
	v = c_v;
	r = c_r;
	g = c_g;
	b = c_b;
}

void vertex::set3d(float set_x, float set_y, float set_z)
{
	x = set_x;
	y = set_y;
	z = set_z;
}

void vertex::setTex(float set_u, float set_v)
{
	u = set_u;
	v = set_v;
}

void vertex::setColors(float set_r, float set_g, float set_b)
{
	r = set_r;
	g = set_g;
	b = set_b;
}
void vertex::glCoords()
{
	glMultiTexCoord2fARB(GL_TEXTURE0_ARB, u, v);
	glMultiTexCoord2fARB(GL_TEXTURE1_ARB, u, v);
	if (r*r + g*g + b*b != 0)
		glColor3f(r, g, b);
	glVertex3f(x, y, z);
}

vertex& vertex::operator=(const vertex vert)
{
	x = vert.x;
	y = vert.y;
	z = vert.z;
	u = vert.u;
	v = vert.v;
	r = vert.r;
	g = vert.g;
	b = vert.b;
	return *this;
}
	
vertex vertex::operator-(const vertex v1)
{
	vertex aux;
	aux = vertex(x - v1.x, y - v1.y, z - v1.z, 0, 0, 0, 0, 0);
	return aux;
}

vertex vertex::operator+(const vertex v1)
{
	vertex aux;
	aux = vertex(x + v1.x, y + v1.y, z + v1.z, 0, 0, 0, 0, 0);
	return aux;
}
vertex vertex::operator*(const vertex v1)
{
	vertex aux;
	aux = vertex(y*v1.z - z*v1.y, z*v1.x - x*v1.z, x*v1.y - y*v1.x, 0, 0, 0, 0, 0);
	return aux;
}

vertex vertex::operator*(float lambda)
{
	vertex aux;
	aux = vertex(x*lambda, y*lambda, z*lambda, 0, 0, 0, 0, 0);
	return aux;
}

vertex vertex::operator/(float lambda)
{
	vertex aux;
	aux = vertex(x/lambda, y/lambda, z/lambda, 0, 0, 0, 0, 0);
	return aux;
}

void vertex::Normal()
{
	glNormal3f(x, y, z);
}
	
vertex Bernstein(float u, vertex* p)
{
	vertex a, b, c, d, r;

	a = p[0] * pow(u, 3);
	b = p[1] * pow(u, 2) * 3 * (1 - u);
	c = p[2] * pow((1-u), 2) * 3 * u;
	d = p[3] * pow((1 - u), 3);

	r = a + b + c + d;
	return r;
}

polygon::polygon(int count)
{
	vert_count = count;
	vertices = new vertex[count];
	flag = false;
}
	
polygon::~polygon()
{
	vert_count = 0;
	delete [] vertices;
}
	
void polygon::Normal()
{
	if (!flag)
	{
		vertex vector1, vector2, inception;
		float d;
		vector1 = vertices[1] - vertices[0];
		vector2 = vertices[2] - vertices[1];
		normal = vector1*vector2;
		inception = vertex(0.0f, 0.0f, 0.0f, 0,0, 0,0,0);
		d = distance(normal, inception);
		if (d < 0.00001)
		{
			std::cout << "Нулевой вектор нормали:" << (float) d << std::endl;
		}
		else
		{
			normal = normal/d;
			normal.Normal();
		}
		flag = !flag;
	}
	else
		normal.Normal();
}

void polygon::addVertex(int index, const vertex add_v)
{
	vertices[index] = add_v;
}

void polygon::glVertices()
{
	Normal();

	for (int i = 0; i < vert_count; i++)
	{
		vertices[i].glCoords();
	}
}

void polygon::glColorize(int index, float r, float g, float b)
{
	if (index == -1)
	{
		for (int i = 0; i < vert_count; i++)
			glColorize(i, r, g, b);
	}
	else vertices[index].setColors(r, g, b);
}

void polygon::glPolygon()
{
	glBegin(GL_POLYGON);
			glVertices();
	glEnd();
}

void polygon::glLines()
{
	glBegin(GL_LINES);
			glVertices();
	glEnd();
}

void triangle::glPolygon()
{
	glBegin(GL_TRIANGLES);
			glVertices();
	glEnd();
}

void quad::glPolygon()
{
	glBegin(GL_QUADS);
			glVertices();
	glEnd();
}

float distance(vertex v1, vertex v2)
{
	return sqrt((v1.x - v2.x)*(v1.x - v2.x) 
				+ (v1.y - v2.y)*(v1.y - v2.y)
				+ (v1.z - v2.z)*(v1.z - v2.z));
}

//quad* quad::divide(int n)
//{
	//float a, b;
	//quad* aux;
	//quad = new quad[n*n];
	//float x, y, z;

	//a = distance(vertices[0], vertices[1]);
  //b = distance(vertices[1], vertices[2]);
	
	//for (int k = 0; k < 4; k++)
	//{
		//x = i*(n-1)*vertices[0]	
	//}
		
	
	//return 0;
//}

interior::interior(const char* path) 
{
		using namespace std;
		float x, y, z, u, v, r, g, b;
		ifstream file;      
		string oneline;
		int i = 0, j = 0, k = 0;
		int h1 = 0, l = 0; 
		int f = 0;
		char s[20];
		int num = 0;

		file.open(path);
		if (file.fail())
		{
			cout << "Error opening file: " << path << endl;
			exit(1);
		}

		getline(file, oneline, '\n');
		sscanf(oneline.c_str(), "TOTAL %d\n", &total);

		frames = new polygon* [total]; 

		while (!file.eof())
		{
			getline(file, oneline, '\n');
			if (oneline.length() > 0)
			{
				if (sscanf(oneline.c_str(), "%s %d %d\n", s, &i, &j))
				{
					if (!strcmp("POLYGONS", s))
								 ;
					else if (!strcmp("QUADS", s))
						j = 4;
					else if (!strcmp("TRIANGLES", s))
						j = 3;
					else if (!strcmp("TEXTURES", s))
					{
						//textures
						tex = i;
						if (tex > 0)
							textures = new int[tex];
							for (f = 0; f < tex; f++)
							{
								getline(file, oneline, '\n');
								if (oneline.length() > 0)
								{
									sscanf(oneline.c_str(), "%d\n", &num);
									textures[f] = num;
								}
							}
						i = 0;
					}
					//polygons			
					for (h1 = 0; h1 < i; h1++)
					{
						switch (j)
						{
							case 3:
								frames[k] = new triangle;
							break;
							case 4:
								frames[k] = new quad;
							break;
							default:
								frames[k] = new polygon(j);
							break;
						}
						for (l = 0; l < j; l++)
						{
							do
								getline(file, oneline, '\n');
							while (oneline[0] == '/');

							if (sscanf(oneline.c_str(),"%f  %f  %f  %f  %f  %f  %f  %f",
																					&x, &y, &z, &u, &v, &r, &g, &b))
							{
								frames[k]->addVertex(l, vertex(x, y, z, u, v, r, g, b));
							}
						}
						k++; //next polygon
					}
				}
			}
		}

		if (k < total) 
			exit(k*10 + 1);

    file.close();

}

interior::~interior()
{
	if (total)
		delete[] frames;
	if (tex)
		delete[] textures;
}

void interior::display(bool lines)
{
	if (tex >= 1)
	{
		glActiveTexture(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[textures[0]]); 
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
		glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);

		glActiveTexture(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[textures[tex - 1]]); 
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
		glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_ADD);
	}
	for (int i = 0; i < total; i++)
	{
		if (!lines)
			frames[i]->glPolygon();
		else
			frames[i]->glLines();
	}
		
	glActiveTexture(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
}

GLuint gen::cube ()
{
	GLuint list;
	list = glGenLists(1);	
	if (!list)
		return 0;
	glNewList(list,GL_COMPILE);
		interior box("cube.txt"); 
		box.display();
	glEndList();
	return list;
}

GLuint gen::other(interior something)
{
	GLuint list;
	list = glGenLists(1);	
	if (!list)
		return 0;
	glNewList(list,GL_COMPILE);
		something.display();
	glEndList();
	return list;
}

inline void TorusVertex(double r1,double r2,double phi,double psi)
{
	double nx = cos(phi)*cos(psi);
	double ny = sin(psi);
	double nz = sin(phi)*cos(psi);
	glNormal3d(nx,ny,nz);
	glVertex3d(r1*cos(phi) + r2*nx,r2*ny,r1*sin(phi) + r2*nz);
}

GLuint gen::torus (double r1,double r2,int n1,int n2)
{
	GLuint list = glGenLists(1);
	if(!list)
		return 0;
	glNewList(list,GL_COMPILE);
	glBegin(GL_QUADS);
	for(int i = 0;i<n1;i++) {
		int i2 = (i<n1-1)?(i+1):(0);
		double phi1 = 2*i*Pi/n1;
		double phi2 = 2*i2*Pi/n1;
		for(int j = 0;j<n2;j++) {
			int j2 = (j<n2-1)?(j+1):(0);
			double psi1 = 2*j*Pi/n2;
			double psi2 = 2*j2*Pi/n2;
			glMultiTexCoord2dARB(GL_TEXTURE0, 5*i/(double)n1,j/(double)n2);
			glMultiTexCoord2dARB(GL_TEXTURE1, 5*i/(double)n1,j/(double)n2);
			TorusVertex(r1,r2,phi1,psi1);

			glMultiTexCoord2dARB(GL_TEXTURE0,5*i/(double)n1,(j+1)/(double)n2);
			glMultiTexCoord2dARB(GL_TEXTURE1,5*i/(double)n1,(j+1)/(double)n2);
			TorusVertex(r1,r2,phi1,psi2);

			glMultiTexCoord2dARB(GL_TEXTURE0, 5*(i+1)/(double)n1,(j+1)/(double)n2);
			glMultiTexCoord2dARB(GL_TEXTURE1,5*i/(double)n1,(j+1)/(double)n2);
			TorusVertex(r1,r2,phi2,psi2);

			glMultiTexCoord2dARB(GL_TEXTURE0, 5*(i+1)/(double)n1,j/(double)n2);
			glMultiTexCoord2dARB(GL_TEXTURE1,5*i/(double)n1,(j+1)/(double)n2);
			TorusVertex(r1,r2,phi2,psi1);
		}
	}
	glEnd();
	glEndList();
	return list;
}

GLuint gen::bezier(bpatch patch)
{
	int u = 0, v;
	float py, px, pyold;
	GLuint drawlist = glGenLists(1);
	vertex temp[4];
	vertex* last;

	last = new vertex[divs+1];

	if (patch.dlBPatch != 0)
		glDeleteLists(patch.dlBPatch, 1);

	for (int k = 0; k < 4; k++)
	{
		temp[k] = patch.anchors[k][3];
	}

	for (v = 0; v < divs + 1; v++)
	{
		px = ((float)v)/((float)divs);
		last[v] = Bernstein(px, temp);
	}

	glNewList(drawlist, GL_COMPILE);
	glActiveTexture(GL_TEXTURE0_ARB);
	glBindTexture(GL_TEXTURE_2D, texture[patch.textures[0]]);
	glEnable(GL_TEXTURE_2D);

	for (u = 1; u < divs + 1; u++)
	{
		py = ((float)u)/((float)divs);
		pyold = ((float)u - 1.0f)/((float)divs);

		for (int k = 0; k < 4; k++)
		{
			temp[k] = Bernstein(py, patch.anchors[k]);
		}

		glBegin(GL_TRIANGLE_STRIP);
			for (v = 0; v < divs + 1; v++)
			{
				px = ((float)v)/((float)divs);
				
			 	glTexCoord2f(pyold, px);
				glVertex3d(last[v].x, last[v].y, last[v].z);

				last[v] = Bernstein(px, temp);
			 	glTexCoord2f(py, px);
				glVertex3d(last[v].x, last[v].y, last[v].z);
			}

		glEnd();
	}
	
	glEndList();

	glActiveTexture(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);

	delete [] last;
	return drawlist;
}

void bpatch::display()
{
	if (dlBPatch != 0)
		glCallList(dlBPatch);
}

bpatch::bpatch(const char* path)
{
	interior bez(path);
	if (bez.total != 4)
		exit(bez.total+4);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			anchors[i][j] = bez.frames[i]->vertices[j];
		}

  tex = bez.tex;
	textures = new int[tex];
	for (int k = 0; k < tex; k++)
			textures[k] = bez.textures[k];

}

GLuint bpatch::dlBPatch = 0;
