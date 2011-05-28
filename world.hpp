#include "khoom.hpp"

class vertex 
{
	GLfloat x, y, z; //3d
	GLfloat u, v;    //texture
	GLfloat r, g, b; //colors
public:
  vertex(const vertex& vert);
	vertex(float c_x = 0, float c_y = 0, float c_z = 0,
				 float c_u = 0, float c_v = 0, 
				 float c_r = 0, float c_g = 0, float c_b = 0);
	~vertex () {};
	void set3d(float set_x, float set_y, float set_z);
	void setTex(float set_u, float set_v);
	void setColors(float set_r, float set_g, float set_b);
	void glCoords();
	vertex& operator=(const vertex vert);
	friend float distance(vertex v1, vertex v2);
};

class polygon 
{
	int vert_count;
	vertex* vertices;
public:
	polygon (int count = 3);
	virtual ~polygon ();
	void addVertex(int index, const vertex add_v);
	void glVertices();
	void glColorize(int index, float r, float g, float b);
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
	//quad* divide(int n = 1);
};

float distance(vertex v1, vertex v2);

class interior 
{
	polygon** frames;
	GLuint* textures;
	int total;
	int tex;
public:
	interior (const char* path);
	~interior ();
	void display();
};
