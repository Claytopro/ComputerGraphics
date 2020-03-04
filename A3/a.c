/* Starting code for assignment 3.
   Calculates a ray that passes through each pixel of the viewing window.
   Draws randomly coloured pixel in the viewing window. */

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
	// window size
#define SIZE 512

GLfloat lightX, lightY,lightZ;
//GLfloat sphereX, sphereY,sphereZ;
GLfloat sx,sy,sz;
GLfloat sphereRadius;

GLfloat ar, ag, ab;
GLfloat dr, dg, db;
GLfloat sr, sg, sb;



	/* calculate the length of a vector */
float length(float *x, float *y, float *z) {
   return( sqrtf( (*x * *x) + (*y * *y) + (*z * *z)) );
}

	/* creates a unit vector */
	/* divide a vector by its own length */
void normalize(float *xd, float *yd, float *zd) {
float len;
   len = length(xd, yd, zd);
   *xd = *xd / len;
   *yd = *yd / len;
   *zd = *zd / len;
}

 
	/* OoenGL calls this to draw the screen */
void display() {
int i, j;
float x, y;
float r, g, b;

GLfloat A,B,C;
GLfloat dis;
GLfloat tOne,tTwo;
//holds points for intersections {x,y,z}
GLfloat ri0[3];
GLfloat ri1[3];

//{x,y,z}
GLfloat normalVector[3]; 
GLfloat viewVector[3]; 
GLfloat lightVector[3];
GLfloat reflectionVector[3];

GLfloat dotproduct;
GLfloat reflectionDotproduct;


GLfloat x0, y0, z0;   // viewpoint origin
GLfloat xd, yd, zd;   // view ray direction


   glClearColor(0.0, 0.0, 0.0, 1.0);
   glClear(GL_COLOR_BUFFER_BIT);

   glPointSize(1.0);

	/* ray tracing code starts here */
	/* set viewer position (x0, y0, z0) */
   x0 = 0.0;
   y0 = 0.0;
   z0 = 0.0;

	/* draw individual pixels */
   glBegin(GL_POINTS);

	/* nested loops move over the viewing window */
	/* i and j move through the x and y coordinates respectively */
   for(i=0; i<SIZE; i++) {
      for(j=0; j<SIZE; j++) {
		/* screen location to draw pixel */
		/* used with glVertexf() but not in calculations for vectors */
         x = (float) (i - (SIZE/2)) / (SIZE/2);
         y = (float) (j - (SIZE/2)) / (SIZE/2);

		/* calculate the ray direction (xd, yd, zd) */
		/* ray starts at the origin (x0, y0, z0) and ends at
		   a point on the viewing window (xd, yd, zd) */
        	/* screen corner coordinates are (-1,-1) to (1,1) */
         xd = ((float) (i - (SIZE/2)) / (SIZE/2));
         yd = ((float) (j - (SIZE/2)) / (SIZE/2));
         zd = -1.0;
         normalize(&xd, &yd, &zd);

			/* ray tracing code */
			/* your code starts here */

		/* calculate B, C.  A == 1.0 */
         //printf("center of sphere at %f.%f,%f\n",sx,sy,sz);
         A = 1.0;
         B = 2 * ((xd * (x0 - sx)) + (yd * (y0 - sy))  +  (zd + (z0 - sz)) );
         C = ((x0 - sx)* (x0 - sx))  + ((y0 - sy)*(y0 - sy))   + ((z0 - sz)*(z0 - sz))  - ((sphereRadius)*(sphereRadius));

		/* calculate the discriminant */

         dis = (B*B) - 4*A*C;
      //   printf("discriment = %f\n",dis);
		/* if there is one intersection point (discriminant == 0)
		   then calculate intersection of ray and sphere at (xi, yi, zi) */
         if(dis == 0){
            tOne = ((-1)*B - sqrt(dis) ) / 2*A;
            ri0[0] = (x0 + xd*tOne); 
            ri0[1] = (y0 + yd*tOne);
            ri0[2] = (z0 + zd*tOne);
         }

		/* if there are two two intersection points (discriminant > 0) 
		   then determine which point is closer to the viewpoint (x0, y0, z0)
		   and calculate the intersection of point (xi, yi, zi) */
         if(dis > 0){
            tOne = ((-1)*B - sqrt(dis) ) / 2*A;
            tTwo = ((-1)*B + sqrt(dis) ) / 2*A;

            ri0[0] = (x0 + xd*tOne); 
            ri0[1] = (y0 + yd*tOne);
            ri0[2] = (z0 + zd*tOne);

            ri1[0] = (x0 + xd*tTwo); 
            ri1[1] = (y0 + yd*tTwo);
            ri1[2] = (z0 + zd*tTwo);
            
            if(length( &ri0[0], &ri0[1], &ri0[2]) > length( &ri1[0], &ri1[1], &ri1[2]) ){
               //ri0 is further so ri1 is point to use.
               ri0[0] = ri1[0];
               ri0[1] = ri1[1];
               ri0[2] = ri1[2];
            }

         }

		/* calculate normal vector (nx, ny, nz) to the intersection point */
         normalVector[0] = (ri0[0] - sx)/sphereRadius;
         normalVector[1] = (ri0[1] - sy)/sphereRadius;
         normalVector[2] = (ri0[2] - sz)/sphereRadius;
         normalize(&normalVector[0],&normalVector[1],&normalVector[2]);
		/* calculate viewing vector (vx, vy, vz) */
         viewVector[0] = x0-ri0[0];
         viewVector[1] = y0-ri0[1];
         viewVector[2] = z0-ri0[2];
         normalize(&viewVector[0],&viewVector[1],&viewVector[2]);
		/* calculate the light vector (lx, ly, lz)  */
         lightVector[0] = lightX - ri0[0];
         lightVector[1] = lightY - ri0[1];
         lightVector[2] = lightZ - ri0[2];
         normalize(&lightVector[0],&lightVector[1],&lightVector[2]);

		/* calculate the dot product N.L, using the normal vector
		   and the light vector */
         dotproduct = (normalVector[0] *lightVector[0]) + (normalVector[1] *lightVector[1]) + (normalVector[2] *lightVector[2]);

		/* calculate the reflection vector (rx, ry, rz) */
         reflectionVector[0] = 2 * dotproduct * normalVector[0] - lightVector[0];
         reflectionVector[1] = 2 * dotproduct * normalVector[1] - lightVector[1];
         reflectionVector[2] = 2 * dotproduct * normalVector[2] - lightVector[2];
		/* calculate the dot product R.V, using the reflection vector
		   and the viewing vector */
         reflectionDotproduct = (reflectionVector[0]*viewVector[0]) + (reflectionVector[1]*viewVector[1]) +(reflectionVector[2]*viewVector[2]);
		/* calculate illumination using the parameters read from the
		   file and N.L and R.V  */

		/* set the colour of the point - calculate the r,g,b
		   values using ambient, diffuse, and specular light parameters
		   and the dot products N.L and R.V  */
         
			/* end of ray tracing code */

		/* replace the following three lines with the illumination calculations */
         r = ar + dr*dotproduct + sr*reflectionDotproduct;  
         b = ag + dg*dotproduct + sg*reflectionDotproduct;
         g = ab + db*dotproduct + sb*reflectionDotproduct;
         glColor3f(r, g, b);
		/* draw the point on the display window at point (x, y) */
         glVertex2f(x, y);


      }  // end loop j
   }     // end loop i

   glEnd();
 
   glFlush();
}

	/* read the keyboard, when q pressed exit the program */
void keyboard(unsigned char key, int x, int y) {
   switch (key) {
      case 27:
      case 'q':
         exit(0);
         break;
   }
}
 
int main(int argc, char** argv) {
   char* filename;
   char line[256];
   char *nothing;
   FILE *fp;
   int i,j;
   float input[30];

   filename = argv[1];
	/* read input file */

   lightX = 4.0;
   lightY = 5.0;
   lightZ = 4.0;

   sx = 0.0;
   sy = 0.0;
   sz = 1.0;
   sphereRadius = 1.0;

   ar = 0.2;
   ag = 0.2;
   ab = 0.2;   

   dr = 0.0;
   dg = 0.0;
   db = 0.0;  

   sr = 0.4;
   sg = 0.4;
   sb = 0.4;


   if((fp=fopen(filename,"r")) == NULL){
      printf("Error! opening file\n");
      exit(1);
   }

   i = 0;
   while (fscanf(fp, "%s", line) && !feof(fp)){
     // printf("%s\n",line);
      if(isdigit(line[0]) > 0 || line[0] == '-'){
         input[i++] = atof(line);
      }
   }

   // lightX = input[0];
   // lightY = input[1];
   // lightZ = input[2];

   // sx = input[3];
   // sy = input[4];
   // sz = input[5];
   // sphereRadius = input[6];

   // ar = input[7];
   // ag = input[8];
   // ab = input[9];   

   // dr = input[10];
   // dg = input[11];
   // db = input[12];  

   // sr = input[13];
   // sg = input[14];
   // sb = input[15];  

   
   
	/* Initialize OpenGL and GLUT */
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(SIZE,SIZE);
   glutCreateWindow("2D Test");
   glutDisplayFunc(display);
   glutKeyboardFunc (keyboard);

   glutMainLoop();
   return 0;
}
