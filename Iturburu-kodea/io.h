#ifndef IO_H
#define IO_H

#include "definitions.h"

void keyboard(unsigned char key, int x, int y);
void keyboard_berezia(int key, int x, int y);
GLdouble *biderkatumatrizea(GLdouble* m1, GLdouble* m2);
void nodobatuketa(GLdouble* mx_1);
void nodobatuketakameraPers(GLdouble* cam_mat);
void nodobatuketakameraIbil(GLdouble* cam_mat);
GLdouble *matrizeBektoreBiderketa(GLdouble* matrize, GLdouble* bektore);
GLfloat *matrizeBektoreBiderketa2(GLfloat* matrize, GLfloat* bektore);
void print_help();
void printMatrix(GLdouble *lehena);
void aldaketakargiak(argia* arg, GLdouble* arg_mat);

#endif // IO_H
