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

float **vertexMap;


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



    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(1.0);




    glEnd();
    
    glFlush();
    }

    int numLines(FILE *fp){
        int count = 0;
        char ch;
        while( !feof(fp)){
             ch = fgetc(fp);
                if(ch == '\n') count++;
            
        }

        return count;

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

    int numWords(char *line){
        int count;
        char c = *line;
        do{
            if(c == ' '){
                count++;
            }
        }while(c != '\0');

        return count;
    }
 
    int main(int argc, char** argv) {
        char* filename;
        char line[256];
        char words[10][40]; //holds 10 40letter words 
        int buffersize = 256 * sizeof(char);
        int headerFlag; // changed to 1 once we start reading in vertices

        FILE *fp;
        int i,j,numberofline;
        float x,y,z;

        int numberOfVertex,numberOfFaces;

    
        filename = argv[1];
        /* read input file */


        if((fp=fopen(filename,"r")) == NULL){
        printf("Error! opening file\n");
        exit(1);
        }

        numberofline = numLines(fp);
        vertexMap = malloc( numberofline * sizeof(float *)); 
        for(i=0;i<numberofline;i++){
            vertexMap[i] = malloc(3 * (sizeof(float *)));
        }
        
        //read in file until end header is found then start to read in lines that contain vertices.
        headerFlag=0;
        i=0;
        rewind(fp);

        char *word = malloc(sizeof(char)*256);
        numberOfFaces =0;
        numberOfVertex =0;

        //get elements data from header
        while (fgets(line, buffersize, fp) && !feof(fp) && headerFlag != 1){
        
            if(strcmp(line,"end_header\n")==0) {
                headerFlag =1;
            }

            i=0;
            word = strtok(line," ");
                while (word != NULL)
                    {
                        printf ("%s\n",word);
                        strcpy(words[i],word);
                        i++;
                        word = strtok (NULL, " ");
                    }
            if(strcmp("element",words[0])==0 &&strcmp("vertex",words[1])==0 ){numberOfVertex = atoi(words[2]); }
            if(strcmp("element",words[0])==0 &&strcmp("face",words[1])==0 ){numberOfFaces = atoi(words[2]); }
           
        }

        printf("number vertex: %d, num of faces:%d\n",numberOfVertex,numberOfFaces);

        //read in vertices
        while (fgets(line, buffersize, fp) && !feof(fp)){
            
                    sscanf( line, "%f %f %f",&x,&y,&z);

                    vertexMap[i][0] = x;
                    vertexMap[i][1] = y;
                    vertexMap[i][2] = z;
                    
                    i++;
                

        }


   /* 

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutCreateWindow("2D Test");
    glutInitWindowSize(SIZE,SIZE);
    glutKeyboardFunc (keyboard);
    glutDisplayFunc(display);
    glutMainLoop();
*/
    return 0;
    }