#include "world.hpp"

bool count_flag = false;
//object args
const int AXIS = 0;
const int CENTER = 1;
const int ANGLE = 2; 
const int W = 3;
const int VELOCITY = 4;
const int ACCELERATION = 5;
const int SCALE = 6;
const int INFIN = 1000;

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
	normal = NULL;
	s = NULL;
	t = NULL;
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
	normal = NULL;
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

void vertex::setNormal(vertex set_normal)
{
	if (normal == NULL)
	  normal = new vertex(set_normal);
	else
	{
		delete normal;
	  normal = new vertex(set_normal);
	}
}

void vertex::set_tTangent(vertex set_T)
{
	if (t != NULL )
		delete t;
	t = new vertex(set_T);
}

void vertex::count_sTangent()
{
	if (s != NULL)
		delete s;
	s = new vertex((*normal) * (*t)); 
}

void vertex::count_spacelight(vertex l)
{
	spacelight[0]	= (*s) / l;
	spacelight[1] = (*t) / l;
	spacelight[2] = (*normal) / l;
}

void vertex::setColors(float set_r, float set_g, float set_b)
{
	r = set_r;
	g = set_g;
	b = set_b;
}

void vertex::glCoords(int tex)
{
	if (tex >= 1)
	{
		if (bumps)
		{
			glMultiTexCoord2fARB(GLTexture[0], u, v);
			glMultiTexCoord3fvARB(GLTexture[1], spacelight);
		}
		else if (bumps2)
		{
			glMultiTexCoord2fARB(GLTexture[0], u, v);
		}
		else 
			for (int i = 0; i < tex; i++)
				glMultiTexCoord2fARB(GLTexture[i], u, v);
	}
	if (r*r + g*g + b*b != 0)
		glColor3f(r, g, b);
	if (normal != NULL)
		glNormal3f(normal->x, normal->y, normal->z);
	if (fog)
	{
		if (y < start && y > end && x < 3.0 && x > -3.0 && z < 3.0 && z > -3.0 )
			glFogCoordf((start-y)/(start-end));
		else
			glFogCoordf(0.0);
	}
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
	
vertex vertex::operator-(const vertex v1) const
{
	vertex aux;
	aux = vertex(x - v1.x, y - v1.y, z - v1.z, 
							 u - v1.u, v - v1.v, 
							 r - v1.r, g - v1.g, b - v1.b);
	return aux;
}

vertex vertex::operator+(const vertex v1) const
{
	vertex aux;
	aux = vertex(x + v1.x, y + v1.y, z + v1.z,
							 u + v1.u, v + v1.v, 
							 r + v1.r, g + v1.g, b + v1.b);
	return aux;
}

vertex vertex::operator*(const vertex v1)
{
	vertex aux;
	aux = vertex(y*v1.z - z*v1.y, z*v1.x - x*v1.z, x*v1.y - y*v1.x, 0, 0, 0, 0, 0);
	return aux;
}

GLfloat vertex::operator/(const vertex v1)
{
	return x*v1.x+y*v1.y+z*v1.z;
}
	
vertex mult(GLfloat* m, GLfloat* v) //умножение вектора с 4 координатами на матрицу 4 x 4
{
	GLfloat res[4];
	vertex result;
	res[0] = m[0] * v[0] + m[1] *v[1] + m[2] * v[2] + m[3] *v[3];
	res[1] = m[4] * v[0] + m[5] *v[1] + m[6] * v[2] + m[7] *v[3];
	res[2] = m[8] * v[0] + m[9] *v[1] + m[10]* v[2] + m[11]*v[3];	
	res[3] = m[12]* v[0] + m[13]*v[1] + m[14]* v[2] + m[15]*v[3]; // или m[15] ? неясно	
	//res[3] = m[15];
	if (res[3] != 0)
	{
		result.x = res[0]/res[3];
		result.y = res[1]/res[3];
		result.z = res[2]/res[3];
	}
	return result;
}

vertex vertex::operator*(float lambda)
{
	vertex aux;
	aux = vertex(x*lambda, y*lambda, z*lambda, 
							 u*lambda, v*lambda, 
							 r*lambda, g*lambda, b*lambda);
	return aux;
}

vertex vertex::operator/(float lambda)
{
	vertex aux;
	aux = vertex(x/lambda, y/lambda, z/lambda, 
							 u/lambda, v/lambda, 
							 r/lambda, g/lambda, b/lambda);
	return aux;
}
	
bool vertex::operator==(vertex v1)
{
	return (x == v1.x && y == v1.y && z == v1.z );
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
  neighbours = new int[vert_count];
	for (int i = 0; i < vert_count; i++)
	{
	  neighbours[i] = -1;
	}
}
	
polygon::~polygon()
{
	vert_count = 0;
	delete [] vertices;
	delete [] neighbours; // o/o/o/
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
		d = distance(normal, inception);
		if (d < 0.00001)
		{
			std::cout << "Нулевой вектор нормали:" << (float) d << std::endl;
		}
		else
		{
			normal = normal/d;
			if (!count_flag) 
				normal.Normal();
		}
		flag = !flag;
	}
	else if (!count_flag) 
		normal.Normal();

	D = - (vertices[0] / normal);
}

void polygon::addVertex(int index, const vertex add_v)
{
	vertices[index] = add_v;
}

void polygon::glVertices(int tex)
{
	Normal();

	for (int i = 0; i < vert_count; i++)
	{
		vertices[i].glCoords(tex);
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

void polygon::glPolygon(int tex)
{
	glBegin(GL_POLYGON);
			glVertices(tex);
	glEnd();
}

void polygon::glLines()
{
	glBegin(GL_LINES);
			glVertices();
	glEnd();
}

triangle** polygon::divide(int& vert)
{
	vertex new_vertex;
	vertex summ;
	for (int i = 0; i < vert_count; i++)
	{
		summ = summ + vertices[i];	
	}

	new_vertex = summ / vert_count;
	
	triangle** result = new triangle* [vert_count];
	for (int j = 0; j < vert_count; j++)
	{
		result[j] = new triangle;
		result[j]->addVertex(0, vertices[j]);
		result[j]->addVertex(1, vertices[(j+1)%vert_count]);
		result[j]->addVertex(2, new_vertex); 
	}	
	vert = vert_count;
	return result;
}

float polygon::operator-(vertex A)
{
	float d;
	vertex inception;
	if ((d = distance(normal, inception)) > 0.00001)
		return (A/normal + D)/d;
	else 
	  return 0;
}

void triangle::glPolygon(int tex)
{
	glBegin(GL_TRIANGLES);
			glVertices(tex);
	glEnd();
}

void quad::glPolygon(int tex)
{
	glBegin(GL_QUADS);
			glVertices(tex);
	glEnd();
}

float distance(vertex v1, vertex v2)
{
	return sqrt((v1.x - v2.x)*(v1.x - v2.x) 
				+ (v1.y - v2.y)*(v1.y - v2.y)
				+ (v1.z - v2.z)*(v1.z - v2.z));
}

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

		save_total = total;
		base = new data[total];
		for (int w = 0; w < total; w++)
		{
			base[w].index = w;
		}

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

void interior::setConnectivity()
{
	//for each frame A
	for (int i = 0; i < total; i++)
	{
		// for each edge in A
		for (int j = 0; j < frames[i]->vert_count; j++)
		{
			// if there are no neighbours
			if (frames[i]->neighbours[j] == -1)
			{
				// for each frame B except A
				for (int k = 0; k < total && k != i; k++)
				{
					//for each edge in B
					for (int h = 0; h < frames[k]->vert_count; h++)
					{
					//сравнение
						vertex A1 = frames[i]->vertices[j];
						vertex A2 = frames[i]->vertices[(j+1)%frames[i]->vert_count];

						vertex B1 = frames[k]->vertices[h];
						vertex B2 = frames[k]->vertices[(h+1)%frames[k]->vert_count];

						if ((A1 == B1 && A2 == B2) || (A1 == B2 && A2 == B1))
						{
							frames[i]->neighbours[j] = k;
							frames[k]->neighbours[h] = i;
						}

						//std::cout << "A: " << i << " B: " << k << std::endl;

					}
				}
			}
		}
	}
}

void interior::castShadows(vertex Light)
{
	for (int i = 0; i < total; i++)
	{
		float side = frames[i]->normal / Light + frames[i]->D;
	 	frames[i]->visible = (side > 0) ;	
	}

	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT);
	//glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_STENCIL_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFFL);
	//first pass
	glFrontFace(GL_CCW);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	doShadowPass(Light);
	//second pass
	glFrontFace(GL_CW);
	glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
	doShadowPass(Light);

	glFrontFace(GL_CCW);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glStencilFunc(GL_NOTEQUAL, 0, 0xFFFFFFFFL);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(-0.1f, 0.1f, -0.10f);
		glVertex3f(-0.1f,-0.1f, -0.10f);
		glVertex3f( 0.1f, 0.1f, -0.10f);
		glVertex3f( 0.1f,-0.1f, -0.10f);
	glEnd();
	glPopMatrix();
	glPopAttrib();

}

void interior::doShadowPass(vertex Light)
{
	for (int i = 0; i < total; i++)
	{
		if (frames[i]->visible)
		{
			//for every edge
			for (int j = 0; j < frames[i]->vert_count; j++)
			{
				int nindex = frames[i]->neighbours[j];
				if (nindex == -1 || frames[nindex]->visible == false)
				{
					const vertex& v1 = frames[i]->vertices[j];
					const vertex& v2 = frames[i]->vertices[(j+1)%frames[i]->vert_count];

					vertex v3, v4;
					v3 = (v1 - Light)*INFIN + v1;
					v4 = (v2 - Light)*INFIN + v2;

					glBegin(GL_TRIANGLE_STRIP);
						glVertex3f(v1.x, v1.x, v1.x);
						glVertex3f(v2.x, v2.x, v2.x);
						glVertex3f(v3.x, v3.x, v3.x);
						glVertex3f(v4.x, v4.x, v4.x);
					glEnd();

				}
			}
		}
	}
}

void interior::display(vertex* viewer, bool lines)
{
	if (viewer != NULL)
		organize(*viewer);
	else if (save_total != total)
	{
		vertex inception;
		organize(inception);
	}

	for (int i = 0; i < total; i++)
	{
		if (!lines)
		{
			TexturesOn();
			frames[base[i].index]->glPolygon(tex);
			TexturesOff();
		}
		else
			frames[i]->glLines();
	}
		
}
	
void interior::TexturesOn()
{
	if (tex >= 1)
	{
		glActiveTexture(GL_TEXTURE0_ARB);
		glBindTexture(GL_TEXTURE_2D, gettex(0)); 
		glEnable(GL_TEXTURE_2D);
		if (tex > 1 || bumps || bumps2)
		{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);
		}

		for (int i = 1; i < tex; i++)
		{
			glActiveTexture(GLTexture[i]);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, gettex(i)); 
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_ADD);
		}
	}
}

void interior::TexturesOff()
{
	if (tex >= 1)
	{
		for (int i = 0; i < tex; i++)
		{
			glActiveTexture(GLTexture[i]);
			glDisable(GL_TEXTURE_2D);
		}
	}
}

GLuint interior::gettex(int n)
{
	return texture[textures[n]];
}

void interior::divide(int n)
{
	using namespace std;
	triangle** aux;
	polygon** frames_new;
	int count;
	for (int i = 0; i < n; i++)
	{
		aux = frames[0]->divide(count);
		frames_new = new polygon* [total + count - 1];
		for (int j = 1; j < total; j++)
		{
			frames_new[j-1] = frames[j];
		}
		for (int k = 0; k < count; k++)
		{
			frames_new[total - 1 + k] = aux[k];
		}
		delete[] frames;
		total += (count-1);
		frames = new polygon* [total + count - 1];
		frames = frames_new;
	}
}

void interior::processing()
{
		setConnectivity();
		count_flag = true;
		for (int q = 0; q < total; q++)
		{
			frames[q]->Normal();
		}
		count_flag = false;
}

void interior::organize(vertex viewer)
{

	if (save_total != total)
	{
		if (base != NULL)
			delete[] base;
		base = new data[total];
		save_total = total;
	}
	//считаем расстояния
	
	for (int h = 0; h < total; h++)
	{
		base[h].index = h;
		base[h].key = *frames[h] - viewer;
	}

	//упорядочиваем
	data aux;
	bool ordnung = true;

	for (int i = 0; i < total; i++)
	{
		for (int j = 0; j < total - 1; j++)
		{
			if (base[j].key < base[j+1].key)
			{
				aux = base[j];
				base[j] = base[j+1];
				base[j+1] = aux;	
				ordnung = false;
			}				
		}
		if (ordnung)
			break;
	}
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

torus::torus (float t_a, float t_r, int t_n, int t_m)
{
	a = t_a;
	r = t_r;
	R = a;
	n = t_n;
	m = t_m;
	storage = new vertex* [n + 1];
 	for (int i = 0; i < n + 1; i++)
	{
		storage[i] = new vertex[m];
	}
	count();
	tex = 0;
}

void torus::settorus (float t_a, float t_r, int t_n, int t_m)
{
	a = t_a;
	r = t_r;
	R = a;
	n = t_n;
	m = t_m;
	storage = new vertex* [n + 1];
 	for (int i = 0; i < n + 1; i++)
	{
		storage[i] = new vertex[m];
	}
	count();
	tex = 0;
}

torus::~torus ()
{
 	for (int i = 0; i < n + 1; i++)
	{
		delete[] storage[i];
	}
	delete[] storage;
	delete[] textures;
}

void torus::count()
{
	int i2;
	vertex dot;
	vertex normal;
	float S, ri;
	float phi = 0, alpha = 0;
	float beta = a/R;
 	for (int i = 0; i <= n; i++)
	{
		i2 = i - (n/2);
		phi = (2*Pi/n)*(a/(R*(Pi+(1-Pi)*a/R)))*i2;
		dot.set3d((R-a)-R*cos(phi), 0, -R*sin(phi));

		S = a*sqrt(1.0 - (4.0 * (float)abs(i2)/n * (float) abs(i2)/n));
		ri = S + (r + (a-r)*(1-beta) - S)*beta;

		if ((i == n || i == 0) && beta < 0.05)
			ri = 0;

		for (int k = 0; k < m; k++)
		{
	alpha = 2*Pi*k/m;

	storage[i][k].set3d(cos(alpha)*cos(phi),sin(alpha),cos(alpha)*sin(phi));
	normal = storage[i][k];
	storage[i][k].setNormal(normal);
	
	if (bumps)
	{
	storage[i][k].set_tTangent(vertex(-(float)sin(phi),0,(float)cos(phi)));
		storage[i][k].count_sTangent();
		storage[i][k].count_spacelight(Light);
	}

	storage[i][k] = storage[i][k] * ri;
	storage[i][k] = storage[i][k] - dot;

	storage[i][k].setTex(10*ri/r*i/(float)n, ri/r * k/(float)m);
		}

 	} 

}

void torus::display()
{
	TexturesOn();
  for (int i = 0; i <= (int) (n - 1 + a/R); i++)
	{
		glBegin(GL_QUAD_STRIP);
		for (int k = 0; k < m; k++)
		{
				storage[i][k].glCoords(tex);
				storage[(i+1)%(n+1)][k].glCoords(tex);
		}	

			storage[i][0].glCoords(tex);
			storage[(i+1)%(n+1)][0].glCoords(tex);
		glEnd();
	}
	TexturesOff();
}

void torus::sphere(bool s)
{
	if (s)
	{
		R = 21*a;
		count();
	}
	else if (R < 21*a)
	{
		R += 1;
		count();
	}
}

void torus::unsphere(bool s)
{
	if (s)
	{
		R = a;
		count();
	}
	else if (R >= a+1)
	{
		R -= 1;
		count();
	}
}
	
void torus::addTexture(int t)
{
	if (tex == 0)
	{
		tex = 1;
		textures = new int[1];
		textures[0] = t;
	}
	else
	{
		int* aux = new int[tex];
		for (int i = 0; i < tex; i++)
		{
			aux[i] = textures[i];
		}
		tex++;
		if (textures != NULL)
		{
			delete[] textures;
		}
		textures = new int[tex];
		for (int i = 0; i < tex - 1; i++)
		{
			textures[i] = aux[i];
		}
		textures[tex-1] = t;
        delete[] aux;
	}
}
	
void torus::deleteLastTexture()
{
	tex--;
}

void torus::TexturesOn()
{
	if (bumps)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, bumpMap);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_REPLACE);

		glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_CUBE_MAP_ARB);
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, normCubeMap);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_DOT3_RGB_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);

	}
	else if (bumps2)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, gettex(0));
	}
	else if (tex >= 1)
	{
		glActiveTexture(GL_TEXTURE0_ARB);
		glBindTexture(GL_TEXTURE_2D, gettex(0)); 
		glEnable(GL_TEXTURE_2D);
		//if (tex > 1)
		//{
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);
		//}

		for (int i = 1; i < tex; i++)
		{
			glActiveTexture(GLTexture[i]);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, gettex(i)); 
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
			glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_ADD);
				
			if (i == tex - 1 && !cub)
			{
				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
				glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
				glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
			}
		}
	}
}

void torus::TexturesOff()
{
	if (bumps)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glDisable(GL_TEXTURE_2D);
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glDisable(GL_TEXTURE_CUBE_MAP_ARB);
		//glActiveTextureARB(GL_TEXTURE0_ARB);
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	else if (bumps2)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glDisable(GL_TEXTURE_2D);
	}
	else if (tex >= 1)
	{
 		glActiveTexture(GL_TEXTURE0_ARB);
		glDisable(GL_TEXTURE_2D);
		for (int i = 1; i < tex; i++)
		{
			glActiveTexture(GLTexture[i]);
			if (i == tex - 1 && !cub)
			{
				glDisable(GL_TEXTURE_GEN_S);
				glDisable(GL_TEXTURE_GEN_T);
			}
			glDisable(GL_TEXTURE_2D);
		}
	}
}

GLuint torus::gettex(int n)
{
	return texture[textures[n]];
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

object::object(interior& obj, int m)
{
	mass = m;
	core = &obj;
	axis = vertex (1.0, 1.0, 1.0);
	scale = vertex (1.0, 1.0, 1.0);
	//core->processing();
}

void object::set(int arg, vertex param)
{
	switch (arg)
	{
		case AXIS:
			axis = param;
		break;
		case CENTER:
			center = param;
		break;
		case ANGLE:
			angle = param;
		break;
		case W:
			w = param;
		break;
		case VELOCITY:
			velocity = param;
		break;
		case ACCELERATION:
			acceleration = param;
		break;
		case SCALE:
			scale = param;
		break;
	}
}

void object::move(bool dir, bool mirror)
{
	int i = dir ? 1 : -1;
	glPushMatrix();

	if (dir)
	{
		velocity = velocity + acceleration*timer;	
		center = center + velocity*timer;
		angle = angle + w;
	}
	if (!mirror)
		glTranslatef(i*center.x, i*center.y, i*center.z);	
	else
		glTranslatef(center.x, i*center.y, center.z);	

	glRotatef(i*angle.x, axis.x, 0, 0);
	glRotatef(i*angle.y, 0, axis.y, 0);
	glRotatef(i*angle.z, 0, 0, axis.z);
	glScalef(scale.x, scale.y, scale.z);
}

void object::SpaceToObject(GLfloat* Vector)
{
	GLfloat Minv[16];

	glPushMatrix();
	glLoadIdentity();
	move(false, false);
	glGetFloatv(GL_MODELVIEW_MATRIX, Minv);
	SpaceLight = mult(Minv, Vector);
	glPopMatrix();
	glPopMatrix();
}

void object::display(GLfloat* Vector, bool dir, bool mirror)
{
	move(dir, mirror);
	core->display();
	SpaceToObject(Vector);
	core->castShadows(SpaceLight);
	//glEnable(GL_LIGHTING);
	glPopMatrix();
}

void object::divide(int n)
{
	core->divide(n);
	core->processing();
}
