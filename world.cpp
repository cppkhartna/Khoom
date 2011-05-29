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
	glMultiTexCoord2f(GL_TEXTURE0, u, v);
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

void interior::display()
{
	glBindTexture(GL_TEXTURE_2D, texture[textures[0]]); 
	for (int i = 0; i < total; i++)
	{
		frames[i]->glPolygon();
	}
}
