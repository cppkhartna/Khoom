#include "world.hpp"
#include "reader.hpp"

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
	if (u*u + v*v != 0)
		glTexCoord2f(u, v);
	if (r*r + g*g + b*b != 0)
		glColor3f(r, g, b);
	if (x*x + y*y + z*z != 0)
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

polygon::polygon(int count)
{
	vert_count = count;
	vertices = new vertex[count];
}
	
polygon::~polygon()
{
	vert_count = 0;
	delete [] vertices;
}

void polygon::addVertex(int index, const vertex add_v)
{
	vertices[index] = add_v;
}

void polygon::glVertices()
{
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
		int h1 = 0, l = 0, f = 0;
		char s[20];

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
								
					for (h1 = 0; h1 < i; h1++)
					{
						frames[k] = new polygon(j);
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
					if (sscanf(oneline.c_str(), "%d TEXTURES\n", &tex) == 2)
					{
						if (tex > 0)
							textures = new GLuint[tex];
							for (f = 0; f < tex; f++)
							{
								getline(file, oneline, '\n');
								if (oneline.length() > 0)
								{
				 					if (!LoadGLTexture(textures[f], oneline.c_str())) 
									{
										cout << "Error loading texture: " << oneline << endl;	
										exit(100);
									}
								}
							}
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
	delete[] frames;
}

void interior::display()
{
	for (int i = 0; i < total; i++)
	{
		frames[i]->glPolygon();
	}
}
