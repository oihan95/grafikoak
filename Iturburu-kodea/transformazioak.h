//
//  transformazioak.h
//  Praktika_z2
//
//  Created by Oihan Arroyo on 3/11/17.
//  Copyright © 2017 Oihan Arroyo. All rights reserved.
//

#ifndef transformazioak_h
#define transformazioak_h

#include <stdio.h>

//#include <GL/glut.h> //LINUX
#include <GLUT/glut.h> //macOS

//#include <GL/gl.h> //LINUX
#include <openGL/gl.h> //macOS

//#include <GL/glu.h> //LINUX
#include <OpenGL/glu.h> //macOS

GLdouble* scale(GLdouble x, GLdouble y, GLdouble z);
GLdouble* translate(GLdouble x, GLdouble y, GLdouble z);
GLdouble* rotateX(float theta);
GLdouble* rotateY(float theta);
GLdouble* rotateZ(float theta);
GLdouble* identitate_matrizea();


#endif /* transformazioak_h */
