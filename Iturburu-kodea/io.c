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

extern GLdouble *view_mat;

extern camera3d *kamera, *kamera2;

extern GLdouble *eye_PK;
extern GLdouble *up_PK;
extern GLdouble *center_PK;

extern GLdouble *eye_KI;
extern GLdouble *up_KI;
extern GLdouble *center_KI;

extern GLdouble _ortho_x_min,_ortho_x_max;
extern GLdouble _ortho_y_min,_ortho_y_max;
extern GLdouble _ortho_z_min,_ortho_z_max;

float angle = 20.0;

int egoera_trans = 0;
int egoera_glolok = 5;
int egoera_kamara = 6;
int egoera_main = 0;
int argi_mota = 11;
int argi_zenb = 14;
int argi_egoera = 18;
int argi1egoera = 20;
int argi2egoera = 26;
int argi3egoera = 28;
int argi4egoera = 32;
int argi5egoera = 34;
int objektu_bistaratzea = 22;
int materiala = 24;

extern argia *eguzkia;
extern argia *fokua;
extern argia *bonbila;

float angle2 = 20.0;

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
            sum = sum + matrize[c+(d*4)]*bektore[d];
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
    if (egoera_glolok == LOKALA) {
        matrizeemaitza = biderkatumatrizea(mx_1, _selected_object -> matrizea); //OBJEKTUAREN ARDATZEAN
    }else if (egoera_glolok == GLOBALA){
        matrizeemaitza = biderkatumatrizea(_selected_object -> matrizea, mx_1); //ARDATZ NAGUSIAN
    }
    berria -> matrizea = matrizeemaitza;
    _selected_object -> pila -> aurrera = berria;
    berria -> atzera = _selected_object -> pila;
    berria -> aurrera = NULL;
    _selected_object -> pila = berria;
    _selected_object-> matrizea = _selected_object -> pila -> matrizea;
}

void nodobatuketakameraPers(GLdouble* cam_mat){
    elementua *nodo=0;
    nodo = (elementua *) malloc(sizeof (elementua));
    GLdouble * matrizeemaitza2 = malloc(sizeof(GLdouble)*16);
    GLdouble* matrix2 = malloc(sizeof(GLdouble)*16);
    matrix2 = kamera -> pila -> matrizea;
    if (egoera_glolok == LOKALA) {
        matrizeemaitza2 = biderkatumatrizea(cam_mat, matrix2);
    } else if (egoera_glolok == GLOBALA) {
        matrizeemaitza2 = biderkatumatrizea(matrix2, cam_mat);
    }
    nodo -> matrizea = matrizeemaitza2;
    kamera -> pila -> aurrera = nodo;
    nodo -> atzera = kamera -> pila;
    nodo -> aurrera = NULL;
    kamera -> pila = nodo;
    eye_PK = matrizeBektoreBiderketa(nodo -> matrizea, kamera -> eye);
    center_PK = matrizeBektoreBiderketa(nodo -> matrizea, kamera -> center);
    up_PK = matrizeBektoreBiderketa(nodo -> matrizea, kamera -> up);
}

void nodobatuketakameraibiltari(GLdouble* cam_mat){
    elementua *nodo=0;
    nodo = (elementua *) malloc(sizeof (elementua));
    nodo -> matrizea = biderkatumatrizea(cam_mat, kamera2 -> pila -> matrizea);
    eye_KI = matrizeBektoreBiderketa(nodo -> matrizea, kamera2 -> eye);
    center_KI = matrizeBektoreBiderketa(nodo -> matrizea, kamera2 -> center);
    up_KI = matrizeBektoreBiderketa(nodo -> matrizea, kamera2 -> up);
    kamera2 -> pila -> aurrera = nodo;
    nodo -> atzera = kamera2 -> pila;
    nodo -> aurrera = NULL;
    kamera2 -> pila = nodo;
}

void printMatrix(GLdouble *lehena){
    int c;
    for (c = 0; c < 4; c++){
        printf("\t %f \t %f \t %f \t %f \n",lehena[c], lehena[c+4], lehena[c+8], lehena[c+12]);
    }
    printf("\n");
}

vector3 calc_norm(point3 *p1, point3 *p2, point3 *p3){
    vector3 bektorea, bektorenormala;
    GLdouble modulua;
    
    bektorea.x=((p2 -> y - p1 -> y) * (p3 -> z - p1 -> z))-((p2 -> z - p1 -> z) * (p3 -> y - p1 -> y));
    bektorea.y=((p2 -> z - p1 -> z) * (p3 -> x - p1 -> x))-((p2 -> x - p1 -> x) * (p3 -> z - p1 -> z));
    bektorea.z=((p2 -> x - p1 -> x) * (p3 -> y - p1 -> y))-((p2 -> y - p1 -> y) * (p3 -> x - p1 -> x));
    
    modulua = sqrt(bektorea.x * bektorea.x + bektorea.y * bektorea.y + bektorea.z * bektorea.z);
    bektorenormala.x= bektorea.x/modulua;
    bektorenormala.y= bektorea.y/modulua;
    bektorenormala.z= bektorea.z/modulua;
    
    return bektorenormala;
}

void kalkulatunormala(object3d *objektua){
    int i, j;
    int luzera = objektua->num_faces;
    vector3 *norm = malloc(sizeof(vector3)* luzera);
    point3 *vert = malloc (sizeof(point3)*3);
    
    for (i=0; i<luzera; i++) {
        for (j=0; j<3; j++) {
            vert[j] = objektua->vertex_table[objektua->face_table[i].vertex_table[j]].coord;
        }
        norm[i] = calc_norm(&vert[0], &vert[1], &vert[2]);
    }
    objektua -> norm_taula = norm;
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
        kalkulatunormala(auxiliar_object);
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
            egoera_trans = TRASLAZIOA;
            printf("%s\n", KG_MSSG_TRANS);
            break;
            
    case 'b':
    case 'B':
        egoera_trans = BIRAKETA;
        printf("%s\n", KG_MSSG_BIRAKETA);
        break;
            
    case 't':
    case 'T':
        egoera_trans = TAMAINA;
        printf("%s\n", KG_MSSG_TAMAINA);
        break;
            
    case 'g':
    case 'G':
            egoera_glolok = GLOBALA;
            printf("%s\n", KG_MSSG_GLOBALA);
            break;
            
    case 'l':
    case 'L':
            egoera_glolok = LOKALA;
            printf("%s\n", KG_MSSG_LOKALA);
	    break;
            
    case 'o':
    case 'O':
            egoera_main = TRANSFORMAZIOA;
            printf("%s\n", KG_MSSG_TRANSFORM);
            break;
    
    case 'k':
    case 'K':
            egoera_main = KAMARA;
            printf("%s\n", KG_MSSG_KAMERA);
            break;
            
    case 'a':
    case 'A':
        egoera_main = ARGIA;
        printf("Aldaketak argian aplikatuko dira\n");
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
                if (egoera_kamara == KAM_ORTO) {
                    egoera_kamara = KAM_PERS;
                    printf("%s\n", KG_MSSG_KAM_OBJ_MOTA);
                }else if (egoera_kamara == KAM_PERS){
                    egoera_kamara = KAM_IBIL;
                    printf("%s\n", KG_MSSG_KAM_IBIL);
                }else if (egoera_kamara == KAM_IBIL){
                    egoera_kamara = KAM_ORTO;
                    printf("%s\n", KG_MSSG_KAM_ORTO);
                }
            }else{
                printf("%s\n", KG_MSSG_KAM_EMPTY);
            }
            break;
            
    case 48:
            if (argi_mota == ARG_BONBILLA) {
                argi_mota = ARG_EGUZKIA;
                printf("Eguzkia argi mota aktibatu da\n");
            } else if (argi_mota == ARG_EGUZKIA){
                argi_mota = ARG_FOKUA;
                printf("Fokua argi mota aktibatu da\n");
            } else if (argi_mota == ARG_FOKUA){
                argi_mota = ARG_BONBILLA;
                printf("Bonbilla argi mota aktibatu da\n");
            }
            break;
            
    case 49:
            argi_zenb = 14;
            printf("Lehenengo argia aktibatu da\n");
            break;
            
    case 50:
            argi_zenb = 15;
            printf("Bigarren argia aktibatu da\n");
            break;
            
    case 51:
            argi_zenb = 16;
            printf("Hirugarren argia aktibatu da\n");
            break;
            
    case 52:
            argi_zenb = 30;
            printf("Laugarren argia aktibatu da\n");
            break;
        
    case 53:
            argi_zenb = 31;
            printf("Bostgarren argia aktibatu da\n");
            break;
            
    case 13:
            if (argi_egoera == ARGIA_DESGAITU) {
                argi_egoera = ARGIA_GAITU;
                printf("Argien kontrola gaitu da");
            } else if (argi_egoera == ARGIA_GAITU) {
                argi_egoera = ARGIA_DESGAITU;
                printf("Argien kontrola desgaitu da");
            }
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
                if (egoera_main == KAMARA) {
                    if (egoera_kamara == KAM_PERS) {
                        if (egoera_trans == TRASLAZIOA) {
                            cam_mat = translate(0, 1, 0);
                        }else if (egoera_trans == BIRAKETA){
                            if (egoera_glolok == LOKALA) {
                                cam_mat = rotateX(PI/18);
                            } else {
                                cam_mat = rotateX(-PI/18);
                            }
                        }
                        nodobatuketakameraPers(cam_mat);
                    }else if (egoera_kamara == KAM_IBIL){
                        cam_mat = translate(0, 0, -0.5);
                        nodobatuketakameraibiltari(cam_mat);
                    }
                } else if (egoera_main == TRANSFORMAZIOA) {
                    if (egoera_trans == TRASLAZIOA) {
                        mx_t = translate(0, 1, 0);
                    }else if (egoera_trans == BIRAKETA){
                        mx_t = rotateX(-PI/18);
                    }else if (egoera_trans == TAMAINA){
                        mx_t = scale(1, 0.5, 1);
                    }else{
                        printf("%s\n", KG_MSS_OPTION_EMPTY);
                    }
                    nodobatuketa(mx_t);
                }
            }else{
                printf("%s\n", KG_MSS_UP_EMPTY);
            }
            break;
        
            //BEHERA
        case GLUT_KEY_DOWN:
            if (_selected_object != 0) {
                if (egoera_main == KAMARA){
                    if (egoera_kamara == KAM_PERS) {
                        if (egoera_trans == TRASLAZIOA) {
                            cam_mat = translate(0, -1, 0);
                        }else if (egoera_trans == BIRAKETA){
                            if (egoera_glolok == LOKALA) {
                                cam_mat = rotateX(-PI/18);
                            } else {
                                cam_mat = rotateX(PI/18);
                            }
                        }
                        nodobatuketakameraPers(cam_mat);
                    }else if (egoera_kamara == KAM_IBIL){
                        cam_mat = translate(0, 0, 0.5);
                        nodobatuketakameraibiltari(cam_mat);
                    }
                }else if (egoera_main == TRANSFORMAZIOA) {
                    if (egoera_trans > 0) {
                        if (egoera_trans == TRASLAZIOA) {
                            mx_t = translate(0, -1, 0);
                        }else if (egoera_trans == BIRAKETA){
                            mx_t = rotateX(PI/18);
                        }else if (egoera_trans == TAMAINA){
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
                if (egoera_main == KAMARA) {
                    if (egoera_kamara == KAM_PERS) {
                        if (egoera_trans == TRASLAZIOA) {
                            cam_mat = translate(-1, 0, 0);
                        }else if (egoera_trans == BIRAKETA){
                            if (egoera_glolok == LOKALA) {
                                cam_mat = rotateY(PI/18);
                            } else {
                                cam_mat = rotateY(-PI/18);
                            }
                        }
                        nodobatuketakameraPers(cam_mat);
                    }else if (egoera_kamara == KAM_IBIL){
                        cam_mat = rotateY(PI/18);
                        nodobatuketakameraibiltari(cam_mat);
                    }
                }else if (egoera_main == TRANSFORMAZIOA){
                    if (egoera_trans > 0) {
                        if (egoera_trans == TRASLAZIOA) {
                            mx_t = translate(-1, 0, 0);
                        }else if (egoera_trans == BIRAKETA){
                            mx_t = rotateY(-PI/18);
                        }else if (egoera_trans == TAMAINA){
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
                if (egoera_main == KAMARA) {
                    if (egoera_kamara == KAM_PERS) {
                        if (egoera_trans == TRASLAZIOA) {
                            cam_mat = translate(1, 0, 0);
                        }else if (egoera_trans == BIRAKETA){
                            if (egoera_glolok == LOKALA) {
                                cam_mat = rotateY(-PI/18);
                            } else {
                                cam_mat = rotateY(PI/18);
                            }
                        }
                        nodobatuketakameraPers(cam_mat);
                    }else if (egoera_kamara == KAM_IBIL){
                        cam_mat = rotateY(-PI/18);
                        nodobatuketakameraibiltari(cam_mat);
                    }
                }else if (egoera_main == TRANSFORMAZIOA){
                    if (egoera_trans > 0) {
                        if (egoera_trans == TRASLAZIOA) {
                            mx_t = translate(1, 0, 0);
                        }else if (egoera_trans == BIRAKETA){
                            mx_t = rotateY(PI/18);
                        }else if (egoera_trans == TAMAINA){
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
                if (egoera_main == KAMARA) {
                    if (egoera_kamara == KAM_PERS) {
                        if (egoera_trans == TRASLAZIOA) {
                            cam_mat = translate(0, 0, 1);
                        }else if (egoera_trans == BIRAKETA){
                            cam_mat = rotateZ(-PI/18);
                        }
                        nodobatuketakameraPers(cam_mat);
                    }else if (egoera_kamara == KAM_IBIL){
                        if (kamera2 ->angelua < 0.9) {
                            kamera2 -> angelua = (kamera2 -> angelua) + 0.05;
                        }else{
                            printf("Cuello partido\n");
                        }
                        view_mat = rotateX(kamera2 -> angelua);
                    }
                } else if (egoera_main == TRANSFORMAZIOA){
                    if (egoera_trans > 0) {
                        if (egoera_trans == TRASLAZIOA) {
                            mx_t = translate(0, 0, 1);
                        }else if (egoera_trans == BIRAKETA){
                            mx_t = rotateZ(-PI/18);
                        }else if (egoera_trans == TAMAINA){
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
                if (egoera_main == KAMARA) {
                    if (egoera_kamara == KAM_PERS) {
                        if (egoera_trans == TRASLAZIOA) {
                            cam_mat = translate(0, 0, -1);
                        }else if (egoera_trans == BIRAKETA){
                            cam_mat = rotateZ(PI/18);
                        }
                        nodobatuketakameraPers(cam_mat);
                    }else if (egoera_kamara == KAM_IBIL){
                        if (kamera2 -> angelua > -0.9) {
                            kamera2 -> angelua = (kamera2 -> angelua) - 0.05;
                        }else{
                            printf("Cuello partido\n");
                        }
                        view_mat = rotateX(kamera2 -> angelua);
                    }
                }else if (egoera_main == TRANSFORMAZIOA){
                    if (egoera_trans > 0) {
                        if (egoera_trans == TRASLAZIOA) {
                            mx_t = translate(0, 0, -1);
                        }else if (egoera_trans == BIRAKETA){
                            mx_t = rotateZ(PI/18);
                        }else if (egoera_trans == TAMAINA){
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
            
        case GLUT_KEY_F1:
            if (argi1egoera == ARGI1_PIZTU) {
                argi1egoera = ARGI1_ITZALI;
                printf("Lehenengo argia itzali da\n");
            } else if (argi1egoera == ARGI1_ITZALI) {
                argi1egoera = ARGI1_PIZTU;
                printf("Lehenengo argia piztu da\n");
            }
            break;
            
        case GLUT_KEY_F2:
            if (argi2egoera == ARGI2_PIZTU) {
                argi2egoera = ARGI2_ITZALI;
                printf("Bigarren argia itzali da\n");
            } else if (argi2egoera == ARGI2_ITZALI) {
                argi2egoera = ARGI2_PIZTU;
                printf("Bigarren argia piztu da\n");
            }
            break;
            
        case GLUT_KEY_F3:
            if (argi3egoera == ARGI3_PIZTU) {
                argi3egoera = ARGI3_ITZALI;
                printf("Hirugarren argia itzali da\n");
            } else if (argi3egoera == ARGI3_ITZALI) {
                argi3egoera = ARGI3_PIZTU;
                printf("Hirugarren argia piztu da\n");
            }
            break;
            
        case GLUT_KEY_F5:
            if (argi5egoera == ARGI5_PIZTU) {
                argi5egoera = ARGI5_ITZALI;
                printf("Bostgarren argia itzali da\n");
            } else if (argi5egoera == ARGI5_ITZALI) {
                argi5egoera = ARGI5_PIZTU;
                printf("Bostgarren argia piztu da\n");
            }
            break;
            
        case GLUT_KEY_F4:
            if (argi4egoera == ARGI4_PIZTU) {
                argi4egoera = ARGI4_ITZALI;
                printf("Laugarren argia itzali da\n");
            } else if (argi4egoera == ARGI4_ITZALI) {
                argi4egoera = ARGI4_PIZTU;
                printf("Laugarren argia piztu da\n");
            }
            break;
            
        case GLUT_KEY_F11:
            if (materiala == BAI) {
                materiala = EZ;
                printf("Materiala kenduko da\n");
            } else if (materiala == EZ) {
                materiala = BAI;
                printf("Materiala kargatuko da\n");
            }
            break;
        
        case GLUT_KEY_F12:
            if (objektu_bistaratzea == OBJE_FLAT) {
                objektu_bistaratzea = OBJE_SMOOTH;
                printf("Objektu bistaraketa smooth motakoa aktibatu da\n");
            } else if (objektu_bistaratzea == OBJE_SMOOTH) {
                objektu_bistaratzea = OBJE_FLAT;
                printf("Objektu bistaraketa flat motakoa aktibatu da\n");
            }
            break;
            
        default:
            break;
    }
    glutPostRedisplay();
}
