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

float **vertexMap;
int **faceMap;
float rotate;
int numberOfVertex,numberOfFaces;
GLfloat light_position[4];

/*  Initialize material property and light source.  */
void init (void) {
   light_position[0] =  0.0;
   light_position[1] =  10.0;
   light_position[2] =  15.0;
   light_position[3] =  0.0;
 
}



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
        int i,j;
        int v1,v2,v3,v4,v5;
        GLfloat shadowMatrix[4][4];
        GLfloat shadmat[16];
        GLfloat groundplane[4];
        GLfloat green[] = {0.0, 1.0, 0.0, 1.0};
        GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
        
        //draw light as sphere 
        glMatrixMode(GL_MODELVIEW);
        
        glPushMatrix();
        glClear(GL_COLOR_BUFFER_BIT);
        // clear the identity matrix.
        glLoadIdentity();
    
        glTranslatef(light_position[0],light_position[1],light_position[2]);
        glutSolidSphere (1, 20, 20);
    
        glPopMatrix();

        // z plane
        // GLfloat floorVertices[4][3] = {
        //     { -20.0, 20.0, 00.0 },
        //     { 20.0, 20.0, 0.0 },
        //     { 20.0, -20.0, 0.0 },
        //     { -20.0, -20.0, 0.0 },
        //     };

        //x plane

        GLfloat floorVertices[4][3] = {
            { -20.0, 0.0, 20.0 },
            { 20.0, 0.0, 20.0 },
            { 20.0, 0.0, -20.0 },
            { -20.0, 0.0, -20.0 },
            };


        
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        /* set starting location of objects */
        glPushMatrix ();
        glTranslatef(0.0,0.0, -40.0);
        glRotatef(40,1,0,0); // rotate so looking down on cube

        /* rotate around the y axis, angle or rotation (rot) modified in
        the update() function */
        glRotatef (10, 0.0, 1.0, 0.0);

        glPointSize(5.0);
        //draws floor 
        glBegin(GL_POLYGON);
       
        glVertex3fv(floorVertices[0]);
        
        glVertex3fv(floorVertices[1]);
       
        glVertex3fv(floorVertices[2]);
        
        glVertex3fv(floorVertices[3]);
        glEnd();


        for(i=0;i<4;i++){
            for(j=0;j<4;j++){
                shadowMatrix[i][j]=0;
            }
        }

        // 0 4 8 c
        // 1 5 9 d
        // 2 6 a e
        // 3 7 b f
        //create shadow matrix

        //collunm row on z plane
        // shadowMatrix[0][0] = light_position[2];
        // shadowMatrix[0][2] = -light_position[2];
        // shadowMatrix[1][1] = light_position[2];
        // shadowMatrix[1][2] = -light_position[1];
        // shadowMatrix[3][2] = -1; //local light source
        // shadowMatrix[3][3] = light_position[2];
        
        //on x plane
        shadowMatrix[0][0] = light_position[1];
        shadowMatrix[1][0] = -1 * light_position[0];
        shadowMatrix[1][2] = -1 * light_position[2];
        shadowMatrix[2][2] = light_position[1];
        shadowMatrix[1][3] = -1;
        shadowMatrix[3][3] = light_position[1];


       /*
        shadmat[0] = light_position[2];
        shadmat[2] = -light_position[2];
        shadmat[5] = light_position[2];
        shadmat[6] = -light_position[1];
        shadmat[14] = -1;
        shadmat[15] = light_position[2];
        */

        //draw line from light to center
        glBegin(GL_LINES);
        glVertex3f(light_position[0],light_position[1],light_position[2]);
        glVertex3f(0,0,0);
        glEnd();

        glPushMatrix ();
        glColor3f(0.3, 0.3, 0.3);
        glPolygonMode(GL_FRONT, GL_FILL);
      //  glScalef(1,1,0);
       // gluPerspective(90, 1000000, 0.1,100.0);

        glMultMatrixf((const float*)shadowMatrix);
        for(i=0;i<numberOfFaces;i++){
            
            v1 = faceMap[i][0];
            v2 = faceMap[i][1];
            v3 = faceMap[i][2];
            v4 = faceMap[i][3];
        //   printf("i at %d, %d,%d,%d,%d",i,v1,v2,v3,v4);
        //   printf(", xyz = %f, %f, %f\n",vertexMap[v1][0],vertexMap[v1][1],vertexMap[v1][2]);
            if(v1 == 3){
                glBegin(GL_TRIANGLES); 
                glVertex3f(vertexMap[v2][0],vertexMap[v2][1],vertexMap[v2][2]);
                glVertex3f(vertexMap[v3][0],vertexMap[v3][1],vertexMap[v3][2]);
                glVertex3f(vertexMap[v4][0],vertexMap[v4][1],vertexMap[v4][2]);
            }else{
                glBegin(GL_QUADS); 
                v5 = faceMap[i][4];
                
                glVertex3f(vertexMap[v2][0],vertexMap[v2][1],vertexMap[v2][2]);
                glVertex3f(vertexMap[v3][0],vertexMap[v3][1],vertexMap[v3][2]);
                glVertex3f(vertexMap[v4][0],vertexMap[v4][1],vertexMap[v4][2]);
                glVertex3f(vertexMap[v5][0],vertexMap[v5][1],vertexMap[v5][2]);
                /*
                glVertex3f(vertexMap[v2][0]/vertexMap[v2][2],vertexMap[v2][1]/vertexMap[v2][2],1);
                glVertex3f(vertexMap[v3][0]/vertexMap[v3][2],vertexMap[v3][1]/vertexMap[v3][2],1);
                glVertex3f(vertexMap[v4][0]/vertexMap[v4][2],vertexMap[v4][1]/vertexMap[v4][2],1);
                glVertex3f(vertexMap[v5][0]/vertexMap[v5][2],vertexMap[v5][1]/vertexMap[v5][2],1);
                */
            }
            glEnd();
        }
        glColor3f(1.0, 1.0, 1.0);
        glPopMatrix ();

          glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        for(i=0;i<numberOfFaces;i++){
            v1 = faceMap[i][0];
            v2 = faceMap[i][1];
            v3 = faceMap[i][2];
            v4 = faceMap[i][3];
            if(v1 == 3){
                glBegin(GL_TRIANGLES); 
                glVertex3f(vertexMap[v2][0],vertexMap[v2][1],vertexMap[v2][2]);
                glVertex3f(vertexMap[v3][0],vertexMap[v3][1],vertexMap[v3][2]);
                glVertex3f(vertexMap[v4][0],vertexMap[v4][1],vertexMap[v4][2]);
            }else{
                glBegin(GL_QUADS); 
                v5 = faceMap[i][4];
                glVertex3f(vertexMap[v2][0],vertexMap[v2][1],vertexMap[v2][2]);
                glVertex3f(vertexMap[v3][0],vertexMap[v3][1],vertexMap[v3][2]);
                glVertex3f(vertexMap[v4][0],vertexMap[v4][1],vertexMap[v4][2]);
                glVertex3f(vertexMap[v5][0],vertexMap[v5][1],vertexMap[v5][2]);
            }

            glEnd();
        }

        
        glPopMatrix ();
        glFlush();
    }

    //count numlines in file
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

    void reshape(int w, int h){
        glViewport (0, 0, (GLsizei) w, (GLsizei) h);
        glMatrixMode (GL_PROJECTION);
        glLoadIdentity ();
        gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 0.1, 200.0);
        glMatrixMode (GL_MODELVIEW);
        glLoadIdentity ();
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
 
    void update() {
       rotate += 0.01;
       float theta = 2.0f * 3.1415926f * rotate / 50;//get the current angle
        light_position[0] = 10 * cosf(theta);//calculate the x component
        light_position[2] = 10 * sinf(theta);//calculate the y component
        if(rotate==50){
            rotate =0;
        }

       display();
    }

    int main(int argc, char** argv) {
        char* filename;
        char line[256];
        char words[10][40]; //holds 10 40letter words 
        int buffersize = 256 * sizeof(char);
        int headerFlag; // changed to 1 once we start reading in vertices

        FILE *fp;
        int i,j,numberofline;
        int v1,v2,v3,v4,v5;//holds vertex numbers for each polygon
        float x,y,z;

       

    
        filename = argv[1];
        /* read input file */


        if((fp=fopen(filename,"r")) == NULL){
        printf("Error! opening file\n");
        exit(1);
        }

        numberofline = numLines(fp);
     
        
        //read in file until end header is found then start to read in lines that contain vertices.
        headerFlag=0;
        i=0;
        rewind(fp);

        char *word = malloc(sizeof(char)*256);
        numberOfFaces =0;
        numberOfVertex =0;

        //get elements data from header
        while (headerFlag != 1 &&fgets(line, buffersize, fp) && !feof(fp) ){
        
            if(strcmp(line,"end_header\n")==0) {
                headerFlag =1;
            }

            i=0;
            word = strtok(line," ");
                while (word != NULL)
                    {
                       // printf ("%s\n",word);
                        strcpy(words[i],word);
                        i++;
                        word = strtok (NULL, " ");
                    }
            if(strcmp("element",words[0])==0 &&strcmp("vertex",words[1])==0 ){numberOfVertex = atoi(words[2]); }
            if(strcmp("element",words[0])==0 &&strcmp("face",words[1])==0 ){numberOfFaces = atoi(words[2]); }
           
        }

     //   printf("number vertex: %d, num of faces:%d\n",numberOfVertex,numberOfFaces);

        vertexMap = malloc( (1 + numberOfVertex )* sizeof(float *)); 
        for(i=0;i<numberOfVertex;i++){
            vertexMap[i] = malloc(3 * (sizeof(float *)));
        }

        faceMap = malloc( (1+numberOfFaces) * sizeof(int *)); 
        for(i=0;i<numberOfFaces;i++){
            faceMap[i] = malloc(5 * (sizeof(int *)));
        }

        //read in vertices
        i=0;
        j=0;
        while (fgets(line, buffersize, fp) && !feof(fp)){
                if(i<numberOfVertex){
                    sscanf( line, "%f %f %f",&x,&y,&z);
                    vertexMap[i][0] = x;
                    vertexMap[i][1] = y+4;
                    vertexMap[i][2] = z;
                  //  printf("at %d, xyz = %f, %f, %f\n",i,x,y,z);
                    i++;
                }else{
                    sscanf( line,"%d ",&v1);
                    if(v1 == 3){
                        sscanf( line,"%d %d %d %d",&v1,&v2,&v3,&v4);
                        faceMap[j][0] = v1;
                        faceMap[j][1] = v2;
                        faceMap[j][2] = v3;
                        faceMap[j][3] = v4;
                    }else if (v1 ==4){
                        sscanf( line,"%d %d %d %d %d",&v1,&v2,&v3,&v4,&v5);
                        faceMap[j][0] = v1;
                        faceMap[j][1] = v2;
                        faceMap[j][2] = v3;
                        faceMap[j][3] = v4;
                        faceMap[j][4] = v5;
                    }
                    //printf(" j at %d, %d,%d,%d,%d\n",j,v1,v2,v3,v4);
                    j++;
                }       
        }


   
    init();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (1024, 768);
    glutCreateWindow("3D Ply Shadow");
    glutKeyboardFunc (keyboard);
    glutDisplayFunc(display);
    glutReshapeFunc (reshape);
    glutIdleFunc(update);
    glutMainLoop();

    return 0;
    }