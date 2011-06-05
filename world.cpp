#include "world.hpp"

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

void vertex::setColors(float set_r, float set_g, float set_b)
{
	r = set_r;
	g = set_g;
	b = set_b;
}

void vertex::glCoords(int tex)
{
	if (tex == 1)
		glTexCoord2f(u, v);
	else if (tex > 1)
	{
		glMultiTexCoord2fARB(GL_TEXTURE0_ARB, u, v);
		for (int i = 1; i < tex; i++)
			glMultiTexCoord2fARB(GLTexture[i], u, v);
	}
	if (r*r + g*g + b*b != 0)
		glColor3f(r, g, b);
	if (normal != NULL)
		glNormal3f(normal->x, normal->y, normal->z);
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
	aux = vertex(x - v1.x, y - v1.y, z - v1.z, 
							 u - v1.u, v - v1.v, 
							 r - v1.r, g - v1.g, b - v1.b);
	return aux;
}

vertex vertex::operator+(const vertex v1)
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

int vertex::operator/(const vertex v1)
{
	return x*v1.x+y*v1.y+z*v1.z;
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
		if (tex > 1)
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
 		glActiveTexture(GL_TEXTURE0_ARB);
		glDisable(GL_TEXTURE_2D);
		for (int i = 1; i < tex; i++)
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
		R = 23*a;
		count();
	}
	else if (R < 23*a)
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
	}
}
	
void torus::deleteLastTexture()
{
	tex--;
}

void torus::TexturesOn()
{
	if (tex >= 1)
	{
		glActiveTexture(GL_TEXTURE0_ARB);
		glBindTexture(GL_TEXTURE_2D, gettex(0)); 
		glEnable(GL_TEXTURE_2D);
		if (tex > 1)
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
				
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
			glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_SPHERE_MAP);
		}
	}
}

void torus::TexturesOff()
{
	if (tex >= 1)
	{
 		glActiveTexture(GL_TEXTURE0_ARB);
		glDisable(GL_TEXTURE_2D);
		for (int i = 1; i < tex; i++)
		{
			glActiveTexture(GLTexture[i]);
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			glDisable(GL_TEXTURE_2D);
		}
	}
}

GLuint torus::gettex(int n)
{
	return texture[textures[n]];
}
//GLuint gen::bezier(bpatch patch)
//{
	//int u = 0, v;
	//float py, px, pyold;
	//GLuint drawlist = glGenLists(1);
	//vertex temp[4];
	//vertex* last;

	//last = new vertex[divs+1];

	//if (patch.dlBPatch != 0)
		//glDeleteLists(patch.dlBPatch, 1);

	//for (int k = 0; k < 4; k++)
	//{
		//temp[k] = patch.anchors[k][3];
	//}

	//for (v = 0; v < divs + 1; v++)
	//{
		//px = ((float)v)/((float)divs);
		//last[v] = Bernstein(px, temp);
	//}

	//glNewList(drawlist, GL_COMPILE);
	//glActiveTexture(GL_TEXTURE0_ARB);
	//glBindTexture(GL_TEXTURE_2D, texture[patch.textures[0]]);
	//glEnable(GL_TEXTURE_2D);

	//for (u = 1; u < divs + 1; u++)
	//{
		//py = ((float)u)/((float)divs);
		//pyold = ((float)u - 1.0f)/((float)divs);

		//for (int k = 0; k < 4; k++)
		//{
			//temp[k] = Bernstein(py, patch.anchors[k]);
		//}

		//glBegin(GL_TRIANGLE_STRIP);
			//for (v = 0; v < divs + 1; v++)
			//{
				//px = ((float)v)/((float)divs);
				
				 //glTexCoord2f(pyold, px);
				//glVertex3d(last[v].x, last[v].y, last[v].z);

				//last[v] = Bernstein(px, temp);
				 //glTexCoord2f(py, px);
				//glVertex3d(last[v].x, last[v].y, last[v].z);
			//}

		//glEnd();
	//}
	
	//glEndList();

	//glActiveTexture(GL_TEXTURE0_ARB);
	//glDisable(GL_TEXTURE_2D);

	//delete [] last;
	//return drawlist;
//}

//void bpatch::display()
//{
	//if (dlBPatch != 0)
		//glCallList(dlBPatch);
//}

//bpatch::bpatch(const char* path)
//{
	//interior bez(path);
	//if (bez.total != 4)
		//exit(bez.total+4);
	//for (int i = 0; i < 4; i++)
		//for (int j = 0; j < 4; j++)
		//{
			//anchors[i][j] = bez.frames[i]->vertices[j];
		//}

  //tex = bez.tex;
	//textures = new int[tex];
	//for (int k = 0; k < tex; k++)
			//textures[k] = bez.textures[k];

//}

//GLuint bpatch::dlBPatch = 0;

//vertex object::iforce(vertex dot) // сила, которую производит объект в заданной точке на единичную массу
//{
	//float r = distance(dot, center);
	//vertex direction = center - dot;
	//vertex inception;
	//float d = distance(direction, inception);
	//direction = direction / d;
	//return direction * (mass*G/(r*r));
//}

//float object::ipot(vertex dot)
//{
	//float r = distance(dot, center);
	//return (-mass*G/r);
//}

//void object::move()
//{
	//glPushMatrix();

	//velocity = velocity + acceleration*timer;	
	//center = center + velocity;
	//angle += w;

	//glTranslatef(center.x, center.y, center.z);	
	//glRotatef(angle, axis.x, axis.y, axis.z);
//}

//void object::display()
//{
	//move();
	//core.display();
	//glPopMatrix();
//}
