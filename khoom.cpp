#include "khoom.hpp"
#include "world.hpp"

void InitGL(int Width, int Height)
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	
  glClearDepth(1.0);				
  glDepthFunc(GL_LESS);			
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();			

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);

  glMatrixMode(GL_MODELVIEW);
}

void ReSizeGLScene(int Width, int Height)
{
  if (Height==0)				
    Height=1;

  glViewport(0, 0, Width, Height);		

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char *argv[])
{

		glutInit(&argc, argv);  
    glutInitWindowSize(640, 480);  
    glutInitWindowPosition(0, 0);  
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);  
    glutCreateWindow("Khoom - Khana Own Doom");  
    //glutDisplayFunc(DrawGLScene);  
    glutFullScreen();
    //glutIdleFunc(&DrawGLScene); 
		glutReshapeFunc(&ReSizeGLScene);
    //glutKeyboardFunc(&keyPressed);
    //glutSpecialFunc(&specialKeyPressed);
		InitGL(640, 480);
  
    glutMainLoop();  

    return 1;
}
