#include <stdio.h>

//#include <GL/gl.h> //LINUX
#include <openGL/gl.h> //macOS

//#include <GL/glu.h> //LINUX
#include <OpenGL/glu.h> //macOS

//#include <GL/glut.h> //LINUX
#include <GLUT/glut.h> //macOS

#include "display.h"
#include "io.h"
#include "definitions.h"
#include "transformazioak.h"

/** GLOBAL VARIABLES **/

GLdouble _window_ratio;                     /*Control of window's proportions */
GLdouble _ortho_x_min,_ortho_x_max;         /*Variables for the control of the orthographic projection*/
GLdouble _ortho_y_min ,_ortho_y_max;        /*Variables for the control of the orthographic projection*/
GLdouble _ortho_z_min,_ortho_z_max;         /*Variables for the control of the orthographic projection*/

object3d * _first_object= 0;                /*List of objects*/
object3d * _selected_object = 0;            /*Object currently selected*/

camera3d *kamera = 0;
camera3d *kamera2 = 0;

GLdouble * eye_PK;
GLdouble * up_PK;
GLdouble * center_PK;

GLdouble *view_mat;

GLdouble *eye_KI;
GLdouble *up_KI;
GLdouble *center_KI;

argia *arg1;
argia *arg2;
argia *arg3;
argia *arg4;
argia *arg5;

/** GENERAL INITIALIZATION **/
void initialization (){

    eye_PK = malloc ( sizeof ( GLdouble )*4);
    center_PK = malloc ( sizeof ( GLdouble )*4);
    up_PK = malloc ( sizeof ( GLdouble )*4);
    
    eye_KI = malloc ( sizeof ( GLdouble )*4);
    center_KI = malloc ( sizeof ( GLdouble )*4);
    up_KI = malloc ( sizeof ( GLdouble )*4);
    
    view_mat = (GLdouble *) malloc(sizeof (GLdouble)*16);
    view_mat=identitate_matrizea();
    
    arg1 = (argia *) malloc(sizeof (argia));
    arg1 -> pila = (elementua2 *) malloc(sizeof (elementua2));
    arg1 -> kokapena = malloc ( sizeof ( GLdouble )*4);
    arg1 -> kokapena[0] = 1;
    arg1 -> kokapena[1] = 4;
    arg1 -> kokapena[2] = 2;
    arg1 -> kokapena[3] = 1;
    arg1 -> mota = ARG_BONBILLA;
    arg1 -> pila -> aurrera= NULL;
    arg1 -> pila -> atzera= NULL;
    
    arg2 = (argia *) malloc(sizeof (argia));
    arg2 -> pila = (elementua2 *) malloc(sizeof (elementua2));
    arg2 -> kokapena = malloc ( sizeof ( GLdouble )*4);
    arg2 -> kokapena[0] = 0;
    arg2 -> kokapena[1] = 30;
    arg2 -> kokapena[2] = 15;
    arg2 -> kokapena[3] = 1;
    arg2 -> mota = ARG_BONBILLA;
    arg2 -> pila -> aurrera= NULL;
    arg2 -> pila -> atzera= NULL;
    
    arg3 = (argia *) malloc(sizeof (argia));
    arg3 -> pila = (elementua2 *) malloc(sizeof (elementua2));
    arg3 -> kokapena = malloc ( sizeof ( GLdouble )*4);
    arg3 -> kokapena[0] = 2;
    arg3 -> kokapena[1] = 7;
    arg3 -> kokapena[2] = -20;
    arg3 -> kokapena[3] = 1;
    arg3 -> mota = ARG_BONBILLA;
    arg3 -> pila -> aurrera= NULL;
    arg3 -> pila -> atzera= NULL;
    
    arg4 = (argia *) malloc(sizeof (argia));
    arg4 -> pila = (elementua2 *) malloc(sizeof (elementua2));
    arg4 -> kokapena = malloc ( sizeof ( GLdouble )*4);
    arg4 -> kokapena[0] = 0;
    arg4 -> kokapena[1] = -4;
    arg4 -> kokapena[2] = 8;
    arg4 -> kokapena[3] = 1;
    arg4 -> mota = ARG_BONBILLA;
    arg4 -> pila -> aurrera= NULL;
    arg4 -> pila -> atzera= NULL;
    
    arg5 = (argia *) malloc(sizeof (argia));
    arg5 -> pila = (elementua2 *) malloc(sizeof (elementua2));
    arg5 -> kokapena = malloc ( sizeof ( GLdouble )*4);
    arg5 -> kokapena[0] = 7;
    arg5 -> kokapena[1] = 2;
    arg5 -> kokapena[2] = 5;
    arg5 -> kokapena[3] = 1;
    arg5 -> mota = ARG_BONBILLA;
    arg5 -> pila -> aurrera= NULL;
    arg5 -> pila -> atzera= NULL;

    /*Initialization of all the variables with the default values*/
    _ortho_x_min = KG_ORTHO_X_MIN_INIT;
    _ortho_x_max = KG_ORTHO_X_MAX_INIT;
    _ortho_y_min = KG_ORTHO_Y_MIN_INIT;
    _ortho_y_max = KG_ORTHO_Y_MAX_INIT;
    _ortho_z_min = KG_ORTHO_Z_MIN_INIT;
    _ortho_z_max = KG_ORTHO_Z_MAX_INIT;

    _window_ratio = (GLdouble) KG_WINDOW_WIDTH / (GLdouble) KG_WINDOW_HEIGHT;

    /*Definition of the background color*/
    glClearColor(KG_COL_BACK_R, KG_COL_BACK_G, KG_COL_BACK_B, KG_COL_BACK_A);

    /*Definition of the method to draw the objects*/
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    kamera = malloc ( sizeof ( camera3d ));
    kamera -> pila = (elementua *) malloc(sizeof (elementua));
    kamera -> pila -> matrizea = identitate_matrizea();
    kamera -> pila -> matrizea[14] = 6;
    kamera -> pila -> aurrera= NULL;
    kamera -> pila -> atzera= NULL;
    kamera -> eye = malloc ( sizeof ( GLdouble )*4);
    kamera -> center = malloc ( sizeof ( GLdouble )*4);
    kamera -> up = malloc ( sizeof ( GLdouble )*4);
    kamera -> eye[0]=0;
    kamera -> eye[1]=0;
    kamera -> eye[2]=0;
    kamera -> eye[3]=1;
    kamera -> center[0]=0;
    kamera -> center[1]=0;
    kamera -> center[2]=-6;
    kamera -> center[3]=1;
    kamera -> up[0]=0;
    kamera -> up[1]=1;
    kamera -> up[2]=0;
    kamera -> up[3]=0;
    eye_PK = matrizeBektoreBiderketa(kamera -> pila -> matrizea, kamera -> eye);
    center_PK = matrizeBektoreBiderketa(kamera -> pila -> matrizea, kamera -> center);
    up_PK = matrizeBektoreBiderketa(kamera -> pila -> matrizea, kamera -> up);
    
    kamera2 = malloc ( sizeof ( camera3d ));
    kamera2 -> angelua =0;
    kamera2 -> pila = (elementua *) malloc(sizeof (elementua));
    kamera2 -> pila -> matrizea = biderkatumatrizea(translate(0,0,5),identitate_matrizea());
    kamera2-> pila -> aurrera= NULL;
    kamera2-> pila -> atzera= NULL;
    kamera2 -> eye = malloc ( sizeof ( GLdouble )*4);
    kamera2 -> center = malloc ( sizeof ( GLdouble )*4);
    kamera2 -> up = malloc ( sizeof ( GLdouble )*4);
    kamera2 -> eye[0]=0;
    kamera2 -> eye[1]=0;
    kamera2 -> eye[2]=0;
    kamera2 -> eye[3]=1;
    kamera2 -> center[0]=0;
    kamera2 -> center[1]=0;
    kamera2 -> center[2]=-5;
    kamera2 -> center[3]=1;
    kamera2 -> up[0]=0;
    kamera2 -> up[1]=1;
    kamera2 -> up[2]=0;
    kamera2 -> up[3]=0;
    eye_KI = matrizeBektoreBiderketa(kamera2 -> pila -> matrizea, kamera2 -> eye);
    center_KI = matrizeBektoreBiderketa(kamera2 -> pila -> matrizea, kamera2 -> center);
    up_KI = matrizeBektoreBiderketa(kamera2 -> pila -> matrizea, kamera2 -> up);
    
}


/** MAIN FUNCTION **/
int main(int argc, char** argv) {

    /*First of all, print the help information*/
    print_help();

    /* glut initializations */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(KG_WINDOW_WIDTH, KG_WINDOW_HEIGHT);
    glutInitWindowPosition(KG_WINDOW_X, KG_WINDOW_Y);
    glutCreateWindow(KG_WINDOW_TITLE);
    glEnable(GL_DEPTH_TEST);

    /* set the callback functions */
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboard_berezia);
    
    

    /* this initialization has to be AFTER the creation of the window */
    initialization();

    /* start the main loop */
    glutMainLoop();
    return 0;
}
