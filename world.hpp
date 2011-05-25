#include "khoom.hpp"

//class world 
//{
//public:
	//world ();
	//~world ();
//};
//

class vertex 
{
	GLfloat x, y, z; //3d
	GLfloat u, v;    //texture
public:
	vertex () {};
  vertex(const vertex& vert);
	~vertex () {};
	void set3d(float set_x, float set_y, float set_z);
	void setTex(float set_u, float set_v);
	void glCoords();
	vertex& operator=(const vertex vert);
};

class polygon 
{
	static int vert_i;
	int vert_count;
	vertex* vertices;
public:
	polygon () {};
	polygon (int count);
	~polygon ();
	void addVertex(const vertex add_v);
	void glVertices();
	virtual void glPolygon();
};

class triangle : public polygon 
{
	static int vert_i;
	static int vert_count;
	vertex vertices[3];
public:
	triangle () {};
	~triangle (){};
	virtual void glPolygon();
};

class quad : public polygon
{
	static int vert_i;
	static int vert_count;
	vertex vertices[4];
public:
	quad ();
	~quad ();
	virtual void glPolygon();
};

