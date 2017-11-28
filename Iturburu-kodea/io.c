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

extern camera3d * kamera;

extern GLdouble *eye_PK;
extern GLdouble *up_PK;
extern GLdouble *center_PK;

extern GLdouble _ortho_x_min,_ortho_x_max;
extern GLdouble _ortho_y_min,_ortho_y_max;
extern GLdouble _ortho_z_min,_ortho_z_max;

int EGOERA1 = 0;
int EGOERA2 = 5;
int EGOERA3 = 6;
int EGOERA_MAIN = 9;

/**
 * @brief This function just prints information about the use
 * of the keys
 */

void print_help(){
    printf("KbG Irakasgaiaren Praktika. Programa honek 3D objektuak \n");
    printf("aldatzen eta bistaratzen ditu.  \n\n");

    printf("Egileak: Ugaitz Alonso eta Oihan Arroyo \n");
    printf("Data: Azaroa, 2017 \n");
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

GLdouble *matrizeBektoreBiderketa(GLdouble* matrize, GLdouble* bektore){
    int c, d;
    float sum = 0;
    GLdouble * mult = malloc (sizeof ( GLdouble )*4);
    for (c = 0; c < 4; c++) {
        for (d = 0; d < 4; d++) {
            sum = sum + matrize[c+d*4]*bektore[d];
        }
        mult[c] = sum;
        sum = 0;
    }
    return mult;
}

void nodobatuketa(GLdouble* mx_1){
    elementua *berria=0;
    berria = (elementua *) malloc(sizeof (elementua));
    GLdouble * matrizeemaitza = malloc(sizeof(GLdouble)*16);
    if (EGOERA2 == LOKALA) {
        matrizeemaitza = biderkatumatrizea(mx_1, _selected_object -> matrizea); //OBJEKTUAREN ARDATZEAN
    }else if (EGOERA2 == GLOBALA){
        matrizeemaitza = biderkatumatrizea(_selected_object -> matrizea, mx_1); //ARDATZ NAGUSIAN
    }
    berria -> matrizea = matrizeemaitza;
    _selected_object -> pila -> aurrera = berria;
    berria -> atzera = _selected_object -> pila;
    berria -> aurrera = NULL;
    _selected_object -> pila = berria;
    _selected_object-> matrizea = _selected_object -> pila -> matrizea;
}

void nodobatuketakamera(GLdouble* cam_mat){
    elementua *nodo=0;
    nodo = (elementua *) malloc(sizeof (elementua));
    nodo -> matrizea = biderkatumatrizea(kamera -> pila -> matrizea, cam_mat);
    eye_PK = matrizeBektoreBiderketa(kamera -> eye, nodo -> matrizea);
    center_PK = matrizeBektoreBiderketa(kamera -> center, nodo -> matrizea);
    up_PK = matrizeBektoreBiderketa(kamera -> up, nodo -> matrizea);
    kamera -> pila -> aurrera = nodo;
    nodo -> atzera = kamera -> pila;
    nodo -> aurrera = NULL;
    kamera -> pila = nodo;
}

void keyboard(unsigned char key, int x, int y) {

    GLdouble *mx_t1;
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
            _selected_object -> pila = (elementua *) malloc (sizeof(elementua));
            _selected_object -> pila -> matrizea = identitate_matrizea();
            _selected_object -> pila -> atzera = NULL;
            _selected_object -> pila -> aurrera = NULL;
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
        }else{
            if (_selected_object != NULL){
                mx_t1 = scale(0.5, 0.5, 0.5);
            	nodobatuketa(mx_t1);
            }
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
        }else{
            if (_selected_object != NULL){
                mx_t1 = scale(2, 2, 2);
                nodobatuketa(mx_t1);
            }
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
            
    case 'g':
    case 'G':
            EGOERA2 = GLOBALA;
            printf("%s\n", KG_MSSG_GLOBALA);
            break;
            
    case 'l':
    case 'L':
            EGOERA2 = LOKALA;
            printf("%s\n", KG_MSSG_LOKALA);
	    break;

    //CTRL + z dec code -> 26
    case 26:
	    if (_selected_object != NULL){
	  	  if (glutGetModifiers() == GLUT_ACTIVE_CTRL){
	    		if (_selected_object -> pila -> atzera != NULL){
				_selected_object -> pila = _selected_object -> pila -> atzera;
				_selected_object -> matrizea = _selected_object -> pila -> matrizea;
                }else{
                    printf("Ez dago atzera egiteko aukerarik!\n");
                }
	 	   }
        }else{
            printf("Ez dago objekturik atzera pausua aplikazteko!\n");
	    }
	    break;

    //CTRL + x dec code -> 24
    case 24:
	    if (_selected_object != NULL){
	  	  if (glutGetModifiers() == GLUT_ACTIVE_CTRL){
	    		if (_selected_object -> pila -> aurrera != NULL){
				_selected_object -> pila = _selected_object -> pila -> aurrera;
				_selected_object -> matrizea = _selected_object -> pila -> matrizea;
                    
                }else{
                    printf("Ez dago pausua berregiteko aukerarik!\n");
                }
	 	   }
            
        }else{
            printf("Ez dago objekturik pausua berregiteko!\n");
	    }
	    break;
            
    case 'c':
    case 'C':
            if (_selected_object != NULL){
                if (EGOERA3 == KAM_ORTO) {
                    EGOERA3 = KAM_OBJ_MOTA;
                    printf("%s\n", KG_MSSG_KAM_OBJ_MOTA);
                }else if (EGOERA3 == KAM_OBJ_MOTA){
                    EGOERA3 = KAM_IBILTARIA;
                    printf("%s\n", KG_MSSG_KAM_IBIL);
                }else if (EGOERA3 == KAM_IBILTARIA){
                    EGOERA3 = KAM_ORTO;
                    printf("%s\n", KG_MSSG_KAM_ORTO);
                }
            }else{
                printf("%s\n", KG_MSSG_KAM_EMPTY);
            }
            break;
            
    case 'o':
    case 'O':
            EGOERA_MAIN = TRANSFORMAZIOA;
            printf("%s\n", KG_MSSG_TRANSFORM);
            break;
            
    case 'k':
    case 'K':
            EGOERA_MAIN = KAMERA;
            printf("%s\n", KG_MSSG_KAMERA);
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
    GLdouble *cam_mat=0;
    
    switch (key) {
           
        //GORA
        case GLUT_KEY_UP:
            if (_selected_object != 0) {
                if (EGOERA_MAIN == KAMERA) {
                    if (EGOERA3 == KAM_OBJ_MOTA && (EGOERA1 > 0)) {
                        if (EGOERA1 == TRASLAZIOA) {
                            cam_mat = translate(0, 1, 0);
                        }else if (EGOERA1 == BIRAKETA){
                            cam_mat = rotateX(-pi/18);
                        }
                        nodobatuketakamera(cam_mat);
                    }
                }else if (EGOERA_MAIN == TRANSFORMAZIOA){
                    if (EGOERA1 > 0) {
                        if (EGOERA1 == TRASLAZIOA) {
                            mx_t = translate(0, 1, 0);
                        }else if (EGOERA1 == BIRAKETA){
                            mx_t = rotateX(-pi/18);
                        }else if (EGOERA1 == TAMAINA){
                            mx_t = scale(1, 0.5, 1);
                        }
                        nodobatuketa(mx_t);
                    }else{
                        printf("%s\n", KG_MSS_OPTION_EMPTY);
                    }
                }else{
                    printf("%s\n", KG_MSS_OPTION_MAIN_EMPTY);
                }
            }else{
                printf("%s\n", KG_MSS_UP_EMPTY);
            }
            break;
        
        //BEHERA
        case GLUT_KEY_DOWN:
            if (_selected_object != 0) {
                if (EGOERA_MAIN == KAMERA) {
                    if (EGOERA3 == KAM_OBJ_MOTA && (EGOERA1 > 0)) {
                        if (EGOERA1 == TRASLAZIOA) {
                            cam_mat = translate(0, -1, 0);
                        }else if (EGOERA1 == BIRAKETA){
                            cam_mat = rotateX(pi/18);
                        }
                        nodobatuketakamera(cam_mat);
                    }
                } else if (EGOERA_MAIN == TRANSFORMAZIOA) {
                    if (EGOERA1 > 0) {
                        if (EGOERA1 == TRASLAZIOA) {
                            mx_t = translate(0, -1, 0);
                        }else if (EGOERA1 == BIRAKETA){
                            mx_t = rotateX(pi/18);
                        }else if (EGOERA1 == TAMAINA){
                            mx_t = scale(1, 2, 1);
                        }
                        nodobatuketa(mx_t);
                    }else{
                        printf("%s\n", KG_MSS_OPTION_EMPTY);
                    }
                }
            }else{
                printf("%s\n", KG_MSS_DOWN_EMPTY);
            }
            break;
        
        //EZKERRERA
        case GLUT_KEY_LEFT:
            if (_selected_object != 0) {
                if (EGOERA_MAIN == KAMERA) {
                    if (EGOERA3 == KAM_OBJ_MOTA && (EGOERA1 > 0)) {
                        if (EGOERA1 == TRASLAZIOA) {
                            cam_mat = translate(-1, 0, 0);
                        }else if (EGOERA1 == BIRAKETA){
                            cam_mat = rotateY(-pi/18);
                        }
                        nodobatuketakamera(cam_mat);
                    }
                }else if (EGOERA_MAIN == TRANSFORMAZIOA){
                    if (EGOERA1 > 0) {
                        if (EGOERA1 == TRASLAZIOA) {
                            mx_t = translate(-1, 0, 0);
                        }else if (EGOERA1 == BIRAKETA){
                            mx_t = rotateY(-pi/18);
                        }else if (EGOERA1 == TAMAINA){
                            mx_t = scale(2, 1, 1);
                        }
                        nodobatuketa(mx_t);
                    }else{
                        printf("%s\n", KG_MSS_OPTION_EMPTY);
                    }
                }
            }else{
                printf("%s\n", KG_MSS_LEFT_EMPTY);
            }
            break;
        
        //ESKUINERA
        case GLUT_KEY_RIGHT:
            if (_selected_object != 0) {
                if (EGOERA_MAIN == KAMERA) {
                    if (EGOERA3 == KAM_OBJ_MOTA && (EGOERA1 > 0)) {
                        if (EGOERA1 == TRASLAZIOA) {
                            cam_mat = translate(1, 0, 0);
                        }else if (EGOERA1 == BIRAKETA){
                            cam_mat = rotateY(pi/18);
                        }
                        nodobatuketakamera(cam_mat);
                    }
                }else if (EGOERA_MAIN == TRANSFORMAZIOA){
                    if (EGOERA1 > 0) {
                        if (EGOERA1 == TRASLAZIOA) {
                            mx_t = translate(1, 0, 0);
                        }else if (EGOERA1 == BIRAKETA){
                            mx_t = rotateY(pi/18);
                        }else if (EGOERA1 == TAMAINA){
                            mx_t = scale(0.5, 1, 1);
                        }
                        nodobatuketa(mx_t);
                    }else{
                        printf("%s\n", KG_MSS_OPTION_EMPTY);
                    }
                }
            }else{
                printf("%s\n", KG_MSS_RIGHT_EMPTY);
            }
            break;
        
        //AV_PAG
        case GLUT_KEY_PAGE_UP:
            if (_selected_object != 0) {
                if (EGOERA_MAIN == KAMERA) {
                    if (EGOERA3 == KAM_OBJ_MOTA && (EGOERA1 > 0)) {
                        if (EGOERA1 == TRASLAZIOA) {
                            cam_mat = translate(0, 0, 1);
                        }else if (EGOERA1 == BIRAKETA){
                            cam_mat = rotateZ(-pi/18);
                        }
                        nodobatuketakamera(cam_mat);
                    }
                }else if (EGOERA_MAIN == TRANSFORMAZIOA){
                    if (EGOERA1 > 0) {
                        if (EGOERA1 == TRASLAZIOA) {
                            mx_t = translate(0, 0, 1);
                        }else if (EGOERA1 == BIRAKETA){
                            mx_t = rotateZ(-pi/18);
                        }else if (EGOERA1 == TAMAINA){
                            mx_t = scale(1, 1, 0.5);
                        }
                        nodobatuketa(mx_t);
                    }else{
                        printf("%s\n", KG_MSS_OPTION_EMPTY);
                    }
                }
            }else{
                printf("%s\n", KG_MSSG_PAGE_UP_EMPTY);
            }
            break;
           
        //RE_PAG
        case GLUT_KEY_PAGE_DOWN:
            if (_selected_object != 0) {
                if (EGOERA_MAIN == KAMERA) {
                    if (EGOERA3 == KAM_OBJ_MOTA && (EGOERA1 > 0)) {
                        if (EGOERA1 == TRASLAZIOA) {
                            cam_mat = translate(0, 0, -1);
                        }else if (EGOERA1 == BIRAKETA){
                            cam_mat = rotateZ(pi/18);
                        }
                        nodobatuketakamera(cam_mat);
                    }
                }else if (EGOERA_MAIN == TRANSFORMAZIOA){
                    if (EGOERA1 > 0) {
                        if (EGOERA1 == TRASLAZIOA) {
                            mx_t = translate(0, 0, -1);
                        }else if (EGOERA1 == BIRAKETA){
                            mx_t = rotateZ(pi/18);
                        }else if (EGOERA1 == TAMAINA){
                            mx_t = scale(1, 1, 2);
                        }
                        nodobatuketa(mx_t);
                    }else{
                        printf("%s\n", KG_MSS_OPTION_EMPTY);
                    }
                }
            }else{
                printf("%s\n", KG_MSSG_PAGE_DOWN_EMPTY);
            }
            break;
            
        default:
            break;
    }
    glutPostRedisplay();
}
