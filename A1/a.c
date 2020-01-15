
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

void createImage(int maxDepth, int currentDepth, char* strDraw){
   int i,len;

   currentDepth++;
   if(currentDepth > maxDepth) return;

   len = strlen(strDraw);

   for(i=0; i < len;i++){
       if(strDraw[i] == 'F'){
         //TODO add sphere to thing here
         printf("%c",strDraw[i]);
         createImage(maxDepth,currentDepth,strDraw);

      } else if(strDraw[i] == '['){
         //push location to stack
         printf("%c",strDraw[i]);
      }else if(strDraw[i] == ']'){
         //pop location from stack
         printf("%c",strDraw[i]);
      }else if(strDraw[i] == '+'){
         //rotate angle to the right
         printf("%c",strDraw[i]);
      }else if(strDraw[i] == '-'){
         //rotate angle to the left
         printf("%c",strDraw[i]);
      }

   }
  
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
   
   //allocate memory for string 
   lsystem = malloc ( (strlen(buffer[2])+1) * sizeof(char));
   strcpy(lsystem,buffer[2]);

   //call create image that recursively draws image 
   createImage( depth, 0 , lsystem);
   
   printf("\n");
   fclose(fp);
   return 0; 
}

