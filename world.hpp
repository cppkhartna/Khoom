#include "khoom.hpp"

class vertex;
class interior;

extern GLuint texture[100];
extern const double Pi;
extern int divs;
extern GLenum GLTexture[GL_MAX_TEXTURE_UNITS_ARB];
extern float timer;
extern bool cub;
extern GLuint cubic;
extern GLuint bumpMap;
extern GLuint normCubeMap;
extern vertex Light;
extern bool bumps;
extern bool bumps2;
extern bool fog;
extern float start, end;

const float g = 9.8;

class vertex 
{
	GLfloat x, y, z; //3d
	GLfloat u, v;    //texture
	GLfloat r, g, b; //colors
	float foggi;
	vertex* normal;
	vertex *s, *t;
	GLfloat spacelight[3];
public:
  vertex(const vertex& vert);
	vertex(float c_x = 0, float c_y = 0, float c_z = 0,
				 float c_u = 0, float c_v = 0, 
				 float c_r = 0, float c_g = 0, float c_b = 0);
	~vertex () {};
	void set3d(float set_x, float set_y, float set_z);
	void setTex(float set_u, float set_v);
	void setColors(float set_r, float set_g, float set_b);
  void setNormal(vertex set_normal);
  void set_tTangent(vertex set_T);
  void count_sTangent();
	void count_spacelight(vertex l);
	void glCoords(int tex = 0);
	vertex& operator=(const vertex vert);
	friend float distance(vertex v1, vertex v2);
	vertex operator-(const vertex v1) const; // точка = радиус-вектор
	vertex operator+(const vertex v1) const;
	vertex operator*(const vertex v1); //векторное произведение
	GLfloat operator/(const vertex v1); //скалярноe произведение
	vertex operator*(float lambda);
	friend vertex mult(GLfloat* m, GLfloat* v); //умножение вектора с 4 координатами на матрицу 4 x 4
	vertex operator/(float lambda);
	bool operator==(vertex v1);
	friend vertex Bernstein(float u, vertex* p);
	friend class gen;
	friend class object;
	friend class interior;

	void Normal();
};

class triangle;

class polygon 
{
	int vert_count;
	vertex* vertices;
	vertex normal;
	int D;
	bool flag;
	int* neighbours;
public:
	polygon (int count = 3);
	virtual ~polygon ();
	void addVertex(int index, const vertex add_v);
	void glVertices(int tex = 0);
	void glColorize(int index, float r, float g, float b);
	virtual void glPolygon(int tex = 0);
	void glLines();
	void Normal();
	friend class bpatch;
	triangle** divide(int& vert);
	float operator-(vertex A);
	friend class interior;
	bool visible;
};

class triangle : public polygon 
{
	int vert_count;
	vertex *vertices;
	vertex normal;
	int D;
	bool flag;
	int* neighbours;
public:
	triangle () : polygon(3), vert_count(3){};
	~triangle (){};
	virtual void glPolygon(int tex = 0);
	void glLines();
	friend class interior;
	bool visible;
};

class quad : public polygon
{
	int vert_count;
	vertex *vertices;
	vertex normal;
	int D;
	bool flag;
	int* neighbours;
public:
	quad () : polygon(4), vert_count(4) {};
	~quad () {};
	virtual void glPolygon(int tex = 0);
	void glLines();
	friend class interior;
	bool visible;
};

float distance(vertex v1, vertex v2);

class interior 
{
	polygon** frames;
	int total;
	struct data
	{
		float key;
		int index;
	};
	int save_total;
	data* base;
public:
	interior(){};
	interior (const char* path);
	virtual ~interior ();
	virtual void display(vertex* viewer = NULL, bool lines = false);
	friend class bpatch;
	void divide(int n = 1);
	void organize(vertex viewer);
	virtual void TexturesOn();
	virtual void TexturesOff();
	virtual GLuint gettex(int n);
	void processing();
	void setConnectivity();
	void castShadows(vertex Light);
	void doShadowPass(vertex Light);
public:
	int* textures;
	int tex;
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
	//static GLuint torus(double r1,double r2,int n1,int n2);
	static GLuint bezier(bpatch patch);
	static GLuint other(interior something);
};

class torus : public interior
{
	vertex** storage;
	int n, m;
	float a, r, R;
public:
	torus (){};
	torus (float t_a, float t_r, int t_n, int t_m);
	~torus ();
	void count();
	void display();
	void sphere(bool s = false);
	void unsphere(bool s = false);
	void addTexture(int t);
	void deleteLastTexture();
	void TexturesOn();
	void TexturesOff();
	GLuint gettex(int n);
	void settorus (float t_a, float t_r, int t_n, int t_m);
public:
	int* textures;
	int tex;
};

class object 
{
	interior* core;
	vertex axis;
	vertex center;
	vertex angle;
	vertex w;
	vertex velocity;
	vertex acceleration;
	vertex scale;
	vertex additionalForce;
	int mass;
	vertex SpaceLight;
public:
	object (interior& obj, int m = 0);
	~object () {};
	void display(GLfloat* Vector, bool dir = true, bool mirror = false);
	void move(bool dir, bool mirror);
	void set(int arg, vertex param);
	//void ForceMe(vertex addForce);
	void SpaceToObject(GLfloat* Vector);
	void divide(int n = 1);
};
