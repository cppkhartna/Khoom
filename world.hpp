#include "khoom.hpp"

extern GLuint texture[100];
extern const double Pi;
extern int divs;

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
	vertex operator-(const vertex v1); // точка = радиус-вектор
	vertex operator+(const vertex v1);
	vertex operator*(const vertex v1); //векторное
	vertex operator*(float lambda);
	vertex operator/(float lambda);
	friend vertex Bernstein(float u, vertex* p);
	friend class gen;

	void Normal();
};

class polygon 
{
	int vert_count;
	vertex* vertices;
	vertex normal;
	bool flag;
public:
	polygon (int count = 3);
	virtual ~polygon ();
	void addVertex(int index, const vertex add_v);
	void glVertices();
	void glColorize(int index, float r, float g, float b);
	virtual void glPolygon();
	void glLines();
	void Normal();
	friend class bpatch;
};

class triangle : public polygon 
{
	int vert_count;
	vertex *vertices;
	vertex normal;
	bool flag;
public:
	triangle () : polygon(3), vert_count(3){};
	~triangle (){};
	virtual void glPolygon();
	void glLines();
};

class quad : public polygon
{
	int vert_count;
	vertex *vertices;
	vertex normal;
	bool flag;
public:
	quad () : polygon(4), vert_count(4) {};
	~quad () {};
	virtual void glPolygon();
	void glLines();
	//quad* divide(int n = 1);
};

float distance(vertex v1, vertex v2);

class interior 
{
	polygon** frames;
	int* textures;
	int total;
	int tex;
public:
	interior (const char* path);
	~interior ();
	void display(bool lines = false);
	friend class bpatch;
};

class gen;							 

class bpatch
{
	vertex anchors[4][4];
	int* textures;
	int tex;
public:
	bpatch (const char* path);
	~bpatch () {};
	void display();
	friend class gen;
	static GLuint dlBPatch;
};

class gen 
{
public:
	static GLuint cube();
	static GLuint torus(double r1,double r2,int n1,int n2);
	static GLuint bezier(bpatch patch);
	static GLuint other(interior something);
};
