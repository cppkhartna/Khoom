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

void vertex::glCoords()
{
	glTexCoord2f(u, v);
	glVertex3f(x, y, z);
}

vertex& vertex::operator=(const vertex vert)
{
	x = vert.x;
	y = vert.y;
	z = vert.z;
	u = vert.u;
	v = vert.v;
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

void polygon::addVertex(const vertex add_v)
{
	vert_i++;
	//if (vert_i > vert_count)
		//throw "Ошибка формирования полигона %d - %d", vert_count, vert_i;
	vertices[vert_i] = add_v;
}

void polygon::glVertices()
{
	for (int i = 0; i < vert_count; i++)
	{
		vertices[i].glCoords();
	}
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



int polygon::vert_i = 0;
int triangle::vert_i = 0;
int triangle::vert_count = 3;
int quad::vert_count = 4;
int quad::vert_i = 0;
