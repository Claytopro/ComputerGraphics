
/* Derived from scene.c in the The OpenGL Programming Guide */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
// Linux OpenGL Headers

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


// MacOS OpenGL Headers
/*
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
*/

const float PI = 3.14;
const float vRange = 3.14;
const float uRange = 6.28;
const int radius = 1;

	/* flags used to control the appearance of the image */
int lineDrawing = 0;	// draw polygons as solid or lines
int lighting = 1;	// use diffuse and specular lighting
int drawNormals = 0;	// draw normals on object
int heightmap = 0;	// use heightmap to move vertices when == 1
int drawDots = 0;	// draw only vertices when == 1
int smoothShade = 1;	// use normal vertices when ==1,surface normals when ==0 

int pixelMap[50*50]; // holds heightmap information

	/* used to rotate object in update() */
float rot = 0.0;

	/* used for height map */
int iheight, iwidth, idepth;
int image[100][100];

/*  Initialize material property and light source.  */
void init (void) {
   GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
   GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
   GLfloat light_specular[] = { 0.7, 0.7, 0.7, 1.0 };
   GLfloat light_ambient1[] = { 0.2, 0.2, 0.2, 1.0 };
   GLfloat light_diffuse1[] = { 0.4, 0.4, 0.4, 1.0 };
   GLfloat light_specular1[] = { 0.9, 0.9, 0.9, 1.0 };
   GLfloat light_full_off[] = {0.0, 0.0, 0.0, 1.0};
   GLfloat light_full_on[] = {1.0, 1.0, 1.0, 1.0};

   GLfloat light_position[] = { 10.0, 10.0, 10.0, 0.0 };
   GLfloat light_position2[] = { -5.0, -5.0, -5.0, 0.0 };

	/* if lighting is turned on then use ambient, diffuse and specular
	   lights, otherwise use ambient lighting only */
   if (lighting == 1) {
      glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
      glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
      glLightfv (GL_LIGHT0, GL_SPECULAR, light_full_on);

      glLightfv (GL_LIGHT1, GL_AMBIENT, light_ambient1);
      glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
      glLightfv (GL_LIGHT1, GL_SPECULAR, light_full_on);
   } else {
      glLightfv (GL_LIGHT0, GL_SPECULAR, light_full_off);
      glLightfv (GL_LIGHT0, GL_AMBIENT, light_full_on);
      glLightfv (GL_LIGHT0, GL_DIFFUSE, light_full_off);
      glLightfv (GL_LIGHT1, GL_SPECULAR, light_full_off);
      glLightfv (GL_LIGHT1, GL_AMBIENT, light_full_on);
      glLightfv (GL_LIGHT1, GL_DIFFUSE, light_full_off);
   }
   glLightfv (GL_LIGHT0, GL_POSITION, light_position);
   glLightfv (GL_LIGHT1, GL_POSITION, light_position2);
   
   glEnable (GL_LIGHTING);
   glEnable (GL_LIGHT0);
   glEnable (GL_LIGHT1);
   glEnable(GL_DEPTH_TEST);
}


void display (void)
{
   int i,j,p;
   float segments,u,v;
   GLfloat x1,x2,x3,x4;
   GLfloat y1,y2,y3,y4;
   GLfloat z1,z2,z3,z4;

   float uStepsize, vStepsize;

   GLfloat blue[]  = {0.0, 0.0, 1.0, 1.0};
   GLfloat red[]   = {1.0, 0.0, 0.0, 1.0};
   GLfloat darkred[]   = {0.3, 0.0, 0.0, 1.0};
   GLfloat green[] = {0.0, 1.0, 0.0, 1.0};
   GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
   GLfloat gray[] = {0.8, 0.8, 0.8, 1.0};
   GLfloat darkgray[] = {0.3, 0.3, 0.3, 1.0};


   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* draw surfaces as smooth shaded */
	/* do not change this to GL_FLAT, use normals for flat shading */
   glShadeModel(GL_SMOOTH);

	/* draw polygons as either solid or outlines */
   if (lineDrawing == 1)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else 
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   


	/* give all objects the same shininess value */
   glMaterialf(GL_FRONT, GL_SHININESS, 128);

	/* set starting location of objects */
   glPushMatrix ();
   glTranslatef(0.0, 0.0, -7.0);
	/* rotate around the y axis, angle or rotation (rot) modified in
	   the update() function */
   glRotatef (rot, 0.0, 1.0, 0.0);

	/* set polygon colour */
   glMaterialfv(GL_FRONT, GL_AMBIENT, darkgray);
   glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);
   glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	/* set point size so vertices are visible */
   glPointSize(5.0);

   segments = 50;

   uStepsize = uRange / segments;
   vStepsize = vRange / segments;

   i = 0;

   if(drawDots == 1){
  
       for(j=0; u<segments ; j++ ){
         u = j * uStepsize;
         for(v= -PI/2; v<PI/2 ; v += vStepsize){
            
            glBegin(GL_POINTS);  //starts drawing with points
            x1 = radius * cos((double)v) * cos((double)u);
            y1 = radius * cos((double)v) * sin((double)u);
            z1 = radius * sin((double)v);

            x2 = radius * cos((double)v) * cos((double)u + uStepsize);
            y2 = radius * cos((double)v) * sin((double)u + uStepsize);
            z2 = radius * sin((double)v);
            
            x3 = radius * cos((double)v + vStepsize) * cos((double)u + uStepsize);
            y3 = radius * cos((double)v + vStepsize) * sin((double)u + uStepsize);
            z3 = radius * sin((double)v + vStepsize);

            x4 = radius * cos((double)v + vStepsize) * cos((double)u);
            y4 = radius * cos((double)v + vStepsize) * sin((double)u);
            z4 = radius * sin((double)v + vStepsize);

            if(heightmap == 1){

               x1 *= 1.0 + pixelMap[i] / 512.0;
               y1 *= 1.0 + pixelMap[i] / 512.0;
               z1 *= 1.0 + pixelMap[i] / 512.0;


               x2 *= 1.0 + pixelMap[i] / 512.0;
               y2 *= 1.0 + pixelMap[i] / 512.0;
               z2 *= 1.0 + pixelMap[i] / 512.0;

               x3 *= 1.0 + pixelMap[i] / 512.0;
               y3 *= 1.0 + pixelMap[i] / 512.0;
               z3 *= 1.0 + pixelMap[i] / 512.0;

               x4 *= 1.0 + pixelMap[i] / 512.0;
               y4 *= 1.0 + pixelMap[i] / 512.0;
               z4 *= 1.0 + pixelMap[i] / 512.0;


            //   printf("pix = %d ;i = %d\n",pixelMap[i],i);
               i++;
            }
            
            glVertex3f(x1,y1,z1);
            glVertex3f(x2,y2,z2);
            glVertex3f(x3,y3,z3);
            glVertex3f(x4,y4,z4);   

            glEnd();//end drawing of points

            if(drawNormals == 1){
               glBegin(GL_LINES);
               glVertex3f(x1,y1,z1);
               glVertex3f(x1 *1.1,y1 *1.1,z1 *1.1);
               glEnd();

               glBegin(GL_LINES);
               glVertex3f(x2,y2,z2);
               glVertex3f(x2 *1.1,y2 *1.1,z2 *1.1);
               glEnd();

               glBegin(GL_LINES);
               glVertex3f(x3,y3,z3);
               glVertex3f(x3 *1.1,y3 *1.1,z3 *1.1);
               glEnd();

               glBegin(GL_LINES);
               glVertex3f(x4,y4,z4);
               glVertex3f(x4 * 1.1,y4 *1.1,z4 *1.1);
               glEnd();
            }
         }
      }

   }else if( drawDots == 0) {
      
      for(j=0; j<segments;j++ ){
         u = j * uStepsize;
         for(v= -PI/2; v<PI/2; v += vStepsize){
            glBegin(GL_QUADS);  //starts drawing with points

            x1 = radius * cos((double)v) * cos((double)u);
            y1 = radius * cos((double)v) * sin((double)u);
            z1 = radius * sin((double)v);

            x2 = radius * cos((double)v) * cos((double)u + uStepsize);
            y2 = radius * cos((double)v) * sin((double)u + uStepsize);
            z2 = radius * sin((double)v);
            
            x3 = radius * cos((double)v + vStepsize) * cos((double)u + uStepsize);
            y3 = radius * cos((double)v + vStepsize) * sin((double)u + uStepsize);
            z3 = radius * sin((double)v + vStepsize);

            x4 = radius * cos((double)v + vStepsize) * cos((double)u);
            y4 = radius * cos((double)v + vStepsize) * sin((double)u);
            z4 = radius * sin((double)v + vStepsize);

            if(heightmap == 1){

               x1 *= 1.0 + pixelMap[i] / 512.0;
               y1 *= 1.0 + pixelMap[i] / 512.0;
               z1 *= 1.0 + pixelMap[i] / 512.0;


               x2 *= 1.0 + pixelMap[i] / 512.0;
               y2 *= 1.0 + pixelMap[i] / 512.0;
               z2 *= 1.0 + pixelMap[i] / 512.0;

               x3 *= 1.0 + pixelMap[i] / 512.0;
               y3 *= 1.0 + pixelMap[i] / 512.0;
               z3 *= 1.0 + pixelMap[i] / 512.0;


               x4 *= 1.0 + pixelMap[i] / 512.0;
               y4 *= 1.0 + pixelMap[i] / 512.0;
               z4 *= 1.0 + pixelMap[i] / 512.0;


               printf("pix = %d ;i = %d\n",pixelMap[i],i);
               i++;
            }

            if(smoothShade == 1){ //smooth shade it
               glNormal3f(2*x1,2*y1,2*z1);
               glVertex3f(x1,y1,z1);
               glNormal3f(2*x2,2*y2,2*z2);
               glVertex3f(x2,y2,z2);
               glNormal3f(2*x3,2*y3,2*z3);
               glVertex3f(x3,y3,z3);
               glNormal3f(2*x4,2*y4,2*z4);
               glVertex3f(x4,y4,z4);   
            }else{ //flat shad it
               glNormal3f(2*x1,2*y1,2*z1);
               glVertex3f(x1,y1,z1);
               glVertex3f(x2,y2,z2);
               glVertex3f(x3,y3,z3);
               glVertex3f(x4,y4,z4);   
            }
            glEnd();//end drawing of points

            if(drawNormals == 1){
               glBegin(GL_LINES);
               glVertex3f(x1,y1,z1);
               glVertex3f(x1 *1.1,y1 *1.1,z1 *1.1);
               glEnd();

               glBegin(GL_LINES);
               glVertex3f(x2,y2,z2);
               glVertex3f(x2 *1.1,y2 *1.1,z2 *1.1);
               glEnd();

               glBegin(GL_LINES);
               glVertex3f(x3,y3,z3);
               glVertex3f(x3 *1.1,y3 *1.1,z3 *1.1);
               glEnd();

               glBegin(GL_LINES);
               glVertex3f(x4,y4,z4);
               glVertex3f(x4 * 1.1,y4 *1.1,z4 *1.1);
               glEnd();

            }
             
                     
            
         }
      }
   }
      

   glPopMatrix ();
   glFlush ();
}

void reshape(int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 0.1, 100.0);
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity ();
}

void keyboard(unsigned char key, int x, int y) {
   switch (key) {
      case 27:
      case 'q':
         exit(0);
         break;
      case '1':		// draw polygons as outlines
         lineDrawing = 1;
         lighting = 0;
         init();
         display();
         break;
      case '2':		// draw polygons as filled but not shaded (ambient only)
         lineDrawing = 0;
         lighting = 0;
         init();
         display();
         break;
      case '3':		// diffuse and specular lighting, smooth shading
         lineDrawing = 0;
         lighting = 1;
         init();
         display();
         break;
      case '4':		// draw vertices only, no polygons when ==1
         if (drawDots == 0) drawDots = 1;
         else drawDots = 0;
         init();
         display();
         break;
      case '5':		// flat shade, use only one normal
         if (smoothShade == 0) smoothShade = 1;
         else smoothShade = 0;
         break;
      case '6':		// draw normals to points when ==1
         if (drawNormals == 0) drawNormals = 1;
	 else drawNormals = 0;
         break;
      case '7':		// add height map to sphere when ==1
         if (heightmap == 0) heightmap = 1;
         else heightmap = 0;
         break;
   }
}

void update() {
  rot += 1.0;
   display();
}


/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv) {
   char* filename;
   FILE *fp;
   int i,temp;
   char buffer[256];
   filename = argv[1];

   if((fp=fopen(filename,"r")) == NULL){
      printf("Error! opening file\n");
      exit(1);
   }

  

   //skip first 3 lines 
   for(i=0;i<3;i++){
      fgets(buffer, 256, fp);
   }
   
   i=0;
   while (fscanf(fp, "%d", &temp) && !feof(fp)) {
    
      pixelMap[i] = temp;
      i++;
   }
   

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize (1024, 768);
   glutCreateWindow (argv[0]);
   init();
   glutReshapeFunc (reshape);
   glutDisplayFunc(display);
   glutKeyboardFunc (keyboard);
   glutIdleFunc(update);
   glutMainLoop();

   fclose(fp);
   return 0; 
}

