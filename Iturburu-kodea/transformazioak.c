//
//  transformazioak.c
//  Praktika_z2
//
//  Created by Oihan Arroyo on 3/11/17.
//  Copyright © 2017 Oihan Arroyo. All rights reserved.
//

#include "transformazioak.h"
#include "definitions.h"

//#include <GL/glut.h> //LINUX
#include <GLUT/glut.h> //macOS

//#include <GL/gl.h> //LINUX
#include <openGL/gl.h> //macOS

//#include <GL/glu.h> //LINUX
#include <OpenGL/glu.h> //macOS

//#include <malloc.h> //LINUX
#include <malloc/malloc.h> //macOS
#include <math.h>

extern GLdouble _window_ratio;
extern GLdouble _ortho_x_min,_ortho_x_max;
extern GLdouble _ortho_y_min,_ortho_y_max;
extern GLdouble _ortho_z_min,_ortho_z_max;

extern object3d *_first_object;
extern object3d *_selected_object;

GLdouble* scale(float x, float y, float z){
    GLdouble * m = malloc (sizeof(GLdouble)*16);
    m[0]=x; m[4]=0; m[8] =0; m[12]=0;
    m[1]=0; m[5]=y; m[9] =0; m[13]=0;
    m[2]=0; m[6]=0; m[10]=z; m[14]=0;
    m[3]=0; m[7]=0; m[11]=0; m[15]=1;
    return m;
}

GLdouble* translate(float x, float y, float z){
    GLdouble * m = malloc (sizeof(GLdouble)*16);
    m[0]=1; m[4]=0; m[8] =0; m[12]=x;
    m[1]=0; m[5]=1; m[9] =0; m[13]=y;
    m[2]=0; m[6]=0; m[10]=1; m[14]=z;
    m[3]=0; m[7]=0; m[11]=0; m[15]=1;
    return m;
}

GLdouble* rotateX(float theta){
    GLdouble * m = malloc (sizeof(GLdouble)*16);
    m[0]=1; m[4]=0; m[8] =0; m[12]=0;
    m[1]=0; m[5]=cosf(theta); m[9] =-sinf(theta); m[13]=0;
    m[2]=0; m[6]=sinf(theta); m[10]=cosf(theta); m[14]=0;
    m[3]=0; m[7]=0; m[11]=0; m[15]=1;
    return m;
}

GLdouble* rotateY(float theta){ //open.gl/transformations
    GLdouble * m = malloc (sizeof(GLdouble)*16);
    m[0]=cosf(theta); m[4]=0; m[8]=sinf(theta); m[12]=0;
    m[1]=0; m[5]=1; m[9]=0; m[13]=0;
    m[2]=-sinf(theta); m[6]=0; m[10]=cosf(theta); m[14]=0;
    m[3]=0; m[7]=0; m[11]=0; m[15]=1;
    return m;
}


GLdouble* rotateZ(float theta){
    GLdouble * m = malloc (sizeof(GLdouble)*16);
    m[0]=cosf(theta); m[4]=-sinf(theta); m[8] =0; m[12]=0;
    m[1]=sinf(theta); m[5]=cosf(theta); m[9] =0; m[13]=0;
    m[2]=0; m[6]=0; m[10]=1; m[14]=0;
    m[3]=0; m[7]=0; m[11]=0; m[15]=1;
    return m;
}

GLdouble* identitate_matrizea(){
    GLdouble * m = malloc (sizeof(GLdouble)*16);
    m[0]=1; m[4]=0; m[8] =0; m[12]=0;
    m[1]=0; m[5]=1; m[9] =0; m[13]=0;
    m[2]=0; m[6]=0; m[10]=1; m[14]=0;
    m[3]=0; m[7]=0; m[11]=0; m[15]=1;
    return m;
}

