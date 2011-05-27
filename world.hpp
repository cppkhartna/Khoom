#include "khoom.hpp"

class vertex 
{
	GLfloat x, y, z; //3d
	GLfloat u, v;    //texture
public:
	vertex () {};
  vertex(const vertex& vert);
	vertex(float c_x, float c_y, float c_z, float c_u, float c_v);
	~vertex () {};
	void set3d(float set_x, float set_y, float set_z);
	void setTex(float set_u, float set_v);
	void glCoords();
	vertex& operator=(const vertex vert);
};

class polygon 
{
	int vert_count;
	vertex* vertices;
public:
	polygon (int count = 3);
	virtual ~polygon ();
	void addVertex(int index, const vertex add_v);
	void glVertex(int index, float r, float g, float b);
	void glVertices();
	virtual void glPolygon();
};

class triangle : public polygon 
{
	int vert_count;
	vertex *vertices;
public:
	triangle () : polygon(3), vert_count(3){};
	~triangle (){};
	virtual void glPolygon();
};

class quad : public polygon
{
	int vert_count;
	vertex *vertices;
public:
	quad () : polygon(4), vert_count(4) {};
	~quad () {};
	virtual void glPolygon();
};

