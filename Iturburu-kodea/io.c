#include "definitions.h"
#include "load_obj.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "transformazioak.h"
#include <math.h>

#include <stdlib.h> //LINUX

//#include <GL/glut.h> //LINUX
#include <GLUT/glut.h> //macOS

#include <stdio.h> //LINUX

extern object3d * _first_object;
extern object3d * _selected_object;

extern GLdouble _ortho_x_min,_ortho_x_max;
extern GLdouble _ortho_y_min,_ortho_y_max;
extern GLdouble _ortho_z_min,_ortho_z_max;

int EGOERA1 = 0;

/**
 * @brief This function just prints information about the use
 * of the keys
 */

void print_help(){
    printf("KbG Irakasgaiaren Praktika. Programa honek 3D objektuak \n");
    printf("aldatzen eta bistaratzen ditu.  \n\n");

    printf("Egilea: Borja Calvo (borja.calvo@ehu.es) \n");
    printf("Data: Irailak, 2014 \n");
    printf("\n\n");
    printf("FUNTZIO NAGUSIAK \n");
    printf("<?>\t\t Laguntza hau bistaratu \n");
    printf("<ESC>\t\t Programatik irten \n");
    printf("<F>\t\t Objektua bat kargatu\n");
    printf("<TAB>\t\t Kargaturiko objektuen artean bat hautatu\n");
    printf("<DEL>\t\t Hautatutako objektua ezabatu\n");
    printf("<CTRL + ->\t Bistaratze-eremua handitu\n");
    printf("<CTRL + +>\t Bistaratze-eremua txikitu\n");
    printf("\n\n");
}

/**
 * @brief Callback function to control the basic keys
 * @param key Key that has been pressed
 * @param x X coordinate of the mouse pointer when the key was pressed
 * @param y Y coordinate of the mouse pointer when the key was pressed
 */

//Metodo honek m1 eta m2 matrizeak biderkatzen ditu
GLdouble *biderkatumatrizea(GLdouble* m1, GLdouble* m2){
    GLdouble * mult = malloc (sizeof(GLdouble)*16);
    float sum = 0;
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            for (int k=0; k<4; k++){
                sum = sum + m1[(4*i)+k] * m2[j+(4*k)];
            }
            mult[(i*4)+j] = sum;
            sum = 0;
        }
    }
    return mult;
}

void nodobatuketa(GLdouble* mx_1){
    //elementua *nodo=0;
    //nodo = (elementua *) malloc(sizeof (elementua));
    GLdouble * matrizeemaitza = malloc(sizeof(GLdouble)*16);
    //matrizeemaitza= biderkatumatrizea(_selected_object-> pila -> matrizea, mx_1);
    matrizeemaitza = biderkatumatrizea(mx_1, _selected_object -> matrizea); //OBJEKTUAREN ARDATZEAN
    //nodo -> matrizea = matrizeemaitza;
    //_selected_object -> pila -> next = nodo;
    //nodo -> prev = _selected_object -> pila;
    //nodo -> next = NULL;
    //_selected_object -> pila = nodo;
    //_selected_object-> matrizea = _selected_object -> pila -> matrizea;
    _selected_object -> matrizea = matrizeemaitza;
}

void keyboard(unsigned char key, int x, int y) {

    char* fname = malloc(sizeof (char)*128); /* Note that scanf adds a null character at the end of the vector*/
    int read = 0;
    object3d *auxiliar_object = 0;
    GLdouble wd,he,midx,midy;

    switch (key) {
    case 'f':
    case 'F':
        /*Ask for file*/
        printf("%s", KG_MSSG_SELECT_FILE);
        scanf("%s", fname);
        /*Allocate memory for the structure and read the file*/
        auxiliar_object = (object3d *) malloc(sizeof (object3d));
        read = read_wavefront(fname, auxiliar_object);
        switch (read) {
        /*Errors in the reading*/
        case 1:
            printf("%s: %s\n", fname, KG_MSSG_FILENOTFOUND);
            break;
        case 2:
            printf("%s: %s\n", fname, KG_MSSG_INVALIDFILE);
            break;
        case 3:
            printf("%s: %s\n", fname, KG_MSSG_EMPTYFILE);
            break;
        /*Read OK*/
        case 0:
            /*Insert the new object in the list*/
            auxiliar_object->next = _first_object;
            _first_object = auxiliar_object;
            _selected_object = _first_object;
            //_selected_object -> pila = (elementua *) malloc (sizeof(elementua));
            //_selected_object -> pila -> matrizea = identitate_matrizea();
            //_selected_object -> pila -> prev = NULL;
            //_selected_object -> pila -> next = NULL;
            _selected_object -> matrizea = identitate_matrizea();
            printf("%s\n",KG_MSSG_FILEREAD);
            break;
        }
        break;

    case 9: /* <TAB> */
            if (_selected_object == NULL) {
                printf("%s\n", KG_MSSG_TABFILE);
            } else {
                _selected_object = _selected_object->next;
                /*The selection is circular, thus if we move out of the list we go back to the first element*/
                if (_selected_object == 0) _selected_object = _first_object;
               
            }
            break;

    case 127: /* <SUPR> */
        /*Erasing an object depends on whether it is the first one or not*/
        if (_selected_object == _first_object)
        {
            if (_selected_object == NULL) {
                printf("%s\n", KG_MSSG_SUPRFILE);
            }else{
                /*To remove the first object we just set the first as the current's next*/
                _first_object = _first_object->next;
                /*Once updated the pointer to the first object it is save to free the memory*/
                free(_selected_object);
                /*Finally, set the selected to the new first one*/
                _selected_object = _first_object;
            }
        } else {
            /*In this case we need to get the previous element to the one we want to erase*/
            auxiliar_object = _first_object;
            while (auxiliar_object->next != _selected_object)
                auxiliar_object = auxiliar_object->next;
            /*Now we bypass the element to erase*/
            auxiliar_object->next = _selected_object->next;
            /*free the memory*/
            free(_selected_object);
            /*and update the selection*/
            _selected_object = auxiliar_object;
        }
        break;

    case '-':
        if (glutGetModifiers() == GLUT_ACTIVE_CTRL){
            /*Increase the projection plane; compute the new dimensions*/
            wd=(_ortho_x_max-_ortho_x_min)/KG_STEP_ZOOM;
            he=(_ortho_y_max-_ortho_y_min)/KG_STEP_ZOOM;
            /*In order to avoid moving the center of the plane, we get its coordinates*/
            midx = (_ortho_x_max+_ortho_x_min)/2;
            midy = (_ortho_y_max+_ortho_y_min)/2;
            /*The the new limits are set, keeping the center of the plane*/
            _ortho_x_max = midx + wd/2;
            _ortho_x_min = midx - wd/2;
            _ortho_y_max = midy + he/2;
            _ortho_y_min = midy - he/2;
        }
        break;

    case '+':
        if (glutGetModifiers() == GLUT_ACTIVE_CTRL){
            /*Increase the projection plane; compute the new dimensions*/
            wd=(_ortho_x_max-_ortho_x_min)*KG_STEP_ZOOM;
            he=(_ortho_y_max-_ortho_y_min)*KG_STEP_ZOOM;
            /*In order to avoid moving the center of the plane, we get its coordinates*/
            midx = (_ortho_x_max+_ortho_x_min)/2;
            midy = (_ortho_y_max+_ortho_y_min)/2;
            /*The the new limits are set, keeping the center of the plane*/
            _ortho_x_max = midx + wd/2;
            _ortho_x_min = midx - wd/2;
            _ortho_y_max = midy + he/2;
            _ortho_y_min = midy - he/2;
        }
        break;

    case '?':
        print_help();
        break;

    case 27: /* <ESC> */
        exit(0);
        break;
            
    case 'i':
    case 'I':
            if (_selected_object == NULL) {
                printf("Ez dago objekturik\n");
            } else {
                printf("%s\n", KG_MSSG_INFO);
                int vertices = _selected_object->num_vertices;
                int faces = _selected_object->num_faces;
                printf("Objektu honek %i aurpegi ditu\n",faces);
                printf("Objektu honek %i erpin ditu\n", vertices);
            }
        break;
            
    case 'm':
    case 'M':
            EGOERA1 = TRASLAZIOA;
            printf("%s\n", KG_MSSG_TRANS);
            break;
            
    case 'b':
    case 'B':
        EGOERA1 = BIRAKETA;
        printf("%s\n", KG_MSSG_BIRAKETA);
        break;
            
    case 't':
    case 'T':
        EGOERA1 = TAMAINA;
        printf("%s\n", KG_MSSG_TAMAINA);
        break;
        
    default:
        /*In the default case we just print the code of the key. This is usefull to define new cases*/
        printf("%d %c\n", key, key);
    }
    /*In case we have do any modification affecting the displaying of the object, we redraw them*/
    glutPostRedisplay();
}

void keyboard_berezia(int key, int x, int y){
    
    GLdouble *mx_t=0;
    
    switch (key) {
           
        //GORA
        case GLUT_KEY_UP:
            if (_selected_object != 0 && EGOERA1 > 0) {
                if (EGOERA1 == TRASLAZIOA) {
                    mx_t = translate(0, 1, 0);
                }
                nodobatuketa(mx_t);
            }else{
                printf("%s\n", KG_MSS_UP_EMPTY);
            }
            break;
        
        //BEHERA
        case GLUT_KEY_DOWN:
            if (_selected_object != 0 && EGOERA1 > 0) {
                if (EGOERA1 == TRASLAZIOA) {
                    mx_t = translate(0, -1, 0);
                }
                nodobatuketa(mx_t);
            }else{
                printf("%s\n", KG_MSS_DOWN_EMPTY);
            }
            break;
        
        //EZKERRERA
        case GLUT_KEY_LEFT:
            if (_selected_object != 0 && EGOERA1 > 0) {
                if (EGOERA1 == TRASLAZIOA) {
                    mx_t = translate(-1, 0, 0);
                }
                nodobatuketa(mx_t);
            }else{
                printf("%s\n", KG_MSS_LEFT_EMPTY);
            }
            break;
        
        //ESKUINERA
        case GLUT_KEY_RIGHT:
            if (_selected_object != 0 && EGOERA1 > 0) {
                if (EGOERA1 == TRASLAZIOA) {
                    mx_t = translate(1, 0, 0);
                }
                nodobatuketa(mx_t);
            }else{
                printf("%s\n", KG_MSS_RIGHT_EMPTY);
            }
            break;
        
        //AV_PAG
        case GLUT_KEY_PAGE_UP:
            if (_selected_object != 0 && EGOERA1 > 0) {
                if (EGOERA1 == TRASLAZIOA) {
                    mx_t = translate(0, 0, 1);
                }else if (EGOERA1 == BIRAKETA){
                    mx_t = rotateZ(-pi/18);
                }
                nodobatuketa(mx_t);
            }else{
                printf("%s\n", KG_MSSG_PAGE_UP_EMPTY);
            }
            break;
           
        //RE_PAG
        case GLUT_KEY_PAGE_DOWN:
            if (_selected_object != 0 && EGOERA1 > 0) {
                if (EGOERA1 == TRASLAZIOA) {
                    mx_t = translate(0, 0, -1);
                }
                nodobatuketa(mx_t);
            }else{
                printf("%s\n", KG_MSSG_PAGE_DOWN_EMPTY);
            }
            break;
            
        default:
            break;
    }
    glutPostRedisplay();
}

