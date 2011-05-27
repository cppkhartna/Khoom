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

vertex::vertex(float c_x, float c_y, float c_z, float c_u, float c_v)
{
	x = c_x;
	y = c_y;
	z = c_z;
	u = c_u;
	v = c_v;
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
	if (u*u + v*v != 0)
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

void polygon::glVertex(int index, float r, float g, float b)
{
	glColor3f(r, g, b);
	vertices[index].glCoords();
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

