
/* Derived from scene.c in the The OpenGL Programming Guide */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

/* mac includes 
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
*/

	/* flags used to control the appearance of the image */
int lineDrawing = 1;	// draw polygons as solid or lines
int lighting = 0;	// use diffuse and specular lighting
int smoothShading = 0;  // smooth or flat shading
int textures = 0;
float rotateAngle =0;
int maxDepth = -1;
char* strDraw;

GLubyte  Image[64][64][4];
GLuint   textureID[1];


void createImage( int currentDepth){
   int i,len;

   currentDepth++;
   if(currentDepth > maxDepth) return;

   len = strlen(strDraw);

   for(i=0; i < len;i++){
       if(strDraw[i] == 'F'){
         //moves to new location and draws sphere then recursively calls function
         glTranslatef (0.0, 0.2, 0.0);
         glutSolidSphere (0.1, 20, 20);
         createImage(currentDepth);

      } else if(strDraw[i] == '['){
         //push location to stack
         glPushMatrix ();
      }else if(strDraw[i] == ']'){
         //pop location from stack
         glPopMatrix ();
      }else if(strDraw[i] == '+'){
         //rotate angle to the right
         glRotatef (rotateAngle, 0.0, 0.0, 1.0);
      }else if(strDraw[i] == '-'){
         //rotate angle to the left
         glRotatef (((-1)*rotateAngle), 0.0, 0.0, 1.0);
      }

   }
  
}

// function to initialize 
void init (void) 
{ 
   GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
   GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light_full_off[] = {0.0, 0.0, 0.0, 1.0};
   GLfloat light_full_on[] = {1.0, 1.0, 1.0, 1.0};

   GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

   glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular);
 
   glLightfv (GL_LIGHT0, GL_POSITION, light_position);
   
   glEnable (GL_LIGHTING);
   glEnable (GL_LIGHT0);
   glEnable(GL_DEPTH_TEST);

  
} 

void display (void)
{
 
   GLfloat blue[]  = {0.0, 0.0, 1.0, 1.0};
   GLfloat red[]   = {1.0, 0.0, 0.0, 1.0};
   GLfloat green[] = {0.0, 1.0, 0.0, 1.0};
   GLfloat white[] = {1.0, 1.0, 1.0, 1.0};

   glMatrixMode (GL_MODELVIEW);
   glScalef(0.01, 0.01, 0.01);

   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* draw surfaces as either smooth or flat shaded */
   
   glShadeModel(GL_SMOOTH);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/* set starting location of objects */
   glPushMatrix ();
   glTranslatef(0.0, -9.0, -10.0);
  
	/* give all objects the same shininess value */
   glMaterialf(GL_FRONT, GL_SHININESS, 30.0);

	/* set colour of sphere */
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
   glMaterialfv(GL_FRONT, GL_SPECULAR, green);

   createImage(0);

   glFlush ();


}

void reshape(int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(90.0, (GLfloat)w/(GLfloat)h, 1.0, 1.0);
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity ();
}

int main(int argc, char** argv){
   char* filename;
   int stringLength,i;
   FILE *fp;
   char buffer[3][256];

   int depth;
   float angle;
   char* lsystem;

   filename = argv[1];
   


   if((fp=fopen(filename,"r")) == NULL){
      printf("Error! opening file");
      exit(1);
   }

   i=0;
   while (fgets(buffer[i], 256, fp)) {
      i++;
   }           
   
   //convert inputs to proper form
   depth = atoi(buffer[0]);
   angle = atof(buffer[1]);

   //assign global variables
   rotateAngle = angle;
   maxDepth = depth;

   //allocate memory for string 
   strDraw = malloc ( (strlen(buffer[2])+1) * sizeof(char));
   strcpy(strDraw,buffer[2]);
   
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize (1024, 768);
   glutCreateWindow ("LSystem");
   init();

  
   glutReshapeFunc (reshape);
   glutDisplayFunc(display);
   glutMainLoop();

   //printf("\n");
   fclose(fp);
   return 0; 
}

