#include "definitions.h"
#include "transformazioak.h"
#include "io.h"

//#include <GL/glut.h> //LINUX
#include <GLUT/glut.h> //macOS

//#include <GL/gl.h> //LINUX
#include <openGL/gl.h> //macOS

//#include <GL/glu.h> //LINUX
#include <OpenGL/glu.h> //macOS

/** EXTERNAL VARIABLES **/

extern GLdouble _window_ratio;
extern GLdouble _ortho_x_min,_ortho_x_max;
extern GLdouble _ortho_y_min,_ortho_y_max;
extern GLdouble _ortho_z_min,_ortho_z_max;

extern object3d *_first_object;
extern object3d *_selected_object;

extern int egoera_kamara;

extern camera3d *kamera, *kamera2;

extern GLdouble *eye_PK;
extern GLdouble *up_PK;
extern GLdouble *center_PK;

extern GLdouble *eye_KI;
extern GLdouble *up_KI;
extern GLdouble *center_KI;

extern GLdouble *view_mat;

/**
 * @brief Function to draw the axes
 */
void draw_axes()
{
    /*Draw X axis*/
    glColor3f(KG_COL_X_AXIS_R,KG_COL_X_AXIS_G,KG_COL_X_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(KG_MAX_DOUBLE,0,0);
    glVertex3d(-1*KG_MAX_DOUBLE,0,0);
    glEnd();
    /*Draw Y axis*/
    glColor3f(KG_COL_Y_AXIS_R,KG_COL_Y_AXIS_G,KG_COL_Y_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(0,KG_MAX_DOUBLE,0);
    glVertex3d(0,-1*KG_MAX_DOUBLE,0);
    glEnd();
    /*Draw Z axis*/
    glColor3f(KG_COL_Z_AXIS_R,KG_COL_Z_AXIS_G,KG_COL_Z_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(0,0,KG_MAX_DOUBLE);
    glVertex3d(0,0,-1*KG_MAX_DOUBLE);
    glEnd();
}


/**
 * @brief Callback reshape function. We just store the new proportions of the window
 * @param width New width of the window
 * @param height New height of the window
 */
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    /*  Take care, the width and height are integer numbers, but the ratio is a GLdouble so, in order to avoid
     *  rounding the ratio to integer values we need to cast width and height before computing the ratio */
    _window_ratio = (GLdouble) width / (GLdouble) height;
}


/**
 * @brief Callback display function
 */
void display(void) {
    GLdouble *eye_;
    GLdouble *up_;
    GLdouble *center_;
    GLdouble *erantzun_mat;
    
    eye_ = malloc ( sizeof ( GLdouble )*4);
    center_ = malloc ( sizeof ( GLdouble )*4);
    up_ = malloc ( sizeof ( GLdouble )*4);
    erantzun_mat = malloc(sizeof(GLdouble)*16);
    
    GLint v_index, v, f;
    object3d *aux_obj = _first_object;

    /* Clear the screen */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Define the projection */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    int itxura = (_ortho_x_max - _ortho_x_min)/(_ortho_y_max - _ortho_y_min);
    if (egoera_kamara == KAM_ORTO) {
        /*When the window is wider than our original projection plane we extend the plane in the X axis*/
        if ((_ortho_x_max - _ortho_x_min) / (_ortho_y_max - _ortho_y_min) < _window_ratio) {
            /* New width */
            GLdouble wd = (_ortho_y_max - _ortho_y_min) * _window_ratio;
            /* Midpoint in the X axis */
            GLdouble midpt = (_ortho_x_min + _ortho_x_max) / 2;
            /*Definition of the projection*/
            glOrtho(midpt - (wd / 2), midpt + (wd / 2), _ortho_y_min, _ortho_y_max, _ortho_z_min, _ortho_z_max);
        } else {/* In the opposite situation we extend the Y axis */
            /* New height */
            GLdouble he = (_ortho_x_max - _ortho_x_min) / _window_ratio;
            /* Midpoint in the Y axis */
            GLdouble midpt = (_ortho_y_min + _ortho_y_max) / 2;
            /*Definition of the projection*/
            glOrtho(_ortho_x_min, _ortho_x_max, midpt - (he / 2), midpt + (he / 2), _ortho_z_min, _ortho_z_max);
        }
    }else if (egoera_kamara == KAM_PERS){
        gluPerspective(40.0f, itxura, 1.0, 10.0);
    }else if (egoera_kamara == KAM_IBIL){
        gluPerspective(25.0f, itxura, 1.0, 10.0);
    }
    

    /* Now we start drawing the object */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (egoera_kamara==KAM_PERS){
        gluLookAt(eye_PK[0],eye_PK[1],eye_PK[2],center_PK[0],center_PK[1],center_PK[2],up_PK[0],up_PK[1],up_PK[2]);
    }else if(egoera_kamara == KAM_IBIL){
        erantzun_mat= biderkatumatrizea(view_mat, kamera2 -> pila -> matrizea);
        eye_= matrizeBektoreBiderketa(erantzun_mat, kamera2 -> eye);
        center_= matrizeBektoreBiderketa(erantzun_mat, kamera2 -> center);
        up_= matrizeBektoreBiderketa(erantzun_mat, kamera2 -> up);
        gluLookAt(eye_[0],eye_[1],eye_[2],center_[0],center_[1],center_[2],up_[0],up_[1],up_[2]);
    }
    
    /*Argiak sartuko ditugu*/
    GLfloat horia[4] = {0.0, 1.0, 1.0, 1.0};
    GLfloat grisa [4] = {0.2, 0.2, 0.2, 1.0};
    GLfloat txuria [4] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, grisa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, horia);
    glLightfv(GL_LIGHT0, GL_SPECULAR, txuria);

    /*First, we draw the axes*/
    draw_axes();

    /*Now each of the objects in the list*/
    while (aux_obj != 0) {

        /* Select the color, depending on whether the current object is the selected one or not */
        if (aux_obj == _selected_object){
            glColor3f(KG_COL_SELECTED_R,KG_COL_SELECTED_G,KG_COL_SELECTED_B);
        }else{
            glColor3f(KG_COL_NONSELECTED_R,KG_COL_NONSELECTED_G,KG_COL_NONSELECTED_B);
        }

        /* Draw the object; for each face create a new polygon with the corresponding vertices */
        glLoadIdentity();
        if (egoera_kamara == KAM_PERS){
            gluLookAt(eye_PK[0],eye_PK[1],eye_PK[2],center_PK[0],center_PK[1],center_PK[2],up_PK[0],up_PK[1],up_PK[2]);
        }else if(egoera_kamara==KAM_IBIL){
            erantzun_mat= biderkatumatrizea(view_mat,kamera2 -> pila -> matrizea);
            eye_= matrizeBektoreBiderketa(erantzun_mat, kamera2 -> eye);
            center_= matrizeBektoreBiderketa(erantzun_mat, kamera2 -> center);
            up_= matrizeBektoreBiderketa(erantzun_mat, kamera2 -> up);
            gluLookAt(eye_[0],eye_[1],eye_[2],center_[0],center_[1],center_[2],up_[0],up_[1],up_[2]);
        }
        
        glMultMatrixd(aux_obj->matrizea); //BERRIA
        for (f = 0; f < aux_obj->num_faces; f++) {
            glBegin(GL_POLYGON);
            glNormal3d(aux_obj->norm_taula[f].x, aux_obj->norm_taula[f].y, aux_obj->norm_taula[f].z);
            for (v = 0; v < aux_obj->face_table[f].num_vertices; v++) {
                v_index = aux_obj->face_table[f].vertex_table[v];
                glVertex3d(aux_obj->vertex_table[v_index].coord.x,
                        aux_obj->vertex_table[v_index].coord.y,
                        aux_obj->vertex_table[v_index].coord.z);

            }
            glEnd();
        }
        aux_obj = aux_obj->next;
    }
    /*Do the actual drawing*/
    glFlush();
}
