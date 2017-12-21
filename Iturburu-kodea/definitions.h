#ifndef DEFINITIONS_H
#define DEFINITIONS_H

//#include <GL/gl.h> //LINUX
#include <openGL/gl.h> //macOS

/** DEFINITIONS **/

#define KG_WINDOW_TITLE                     "KbG Praktika - Ugaitz Alonso eta Oihan Arroyo"
#define KG_WINDOW_WIDTH                     800
#define KG_WINDOW_HEIGHT                    600
#define KG_WINDOW_X                         50
#define KG_WINDOW_Y                         50

#define KG_MSSG_SELECT_FILE                 "Idatz ezazu fitxategiaren path-a: "
#define KG_MSSG_FILENOTFOUND                "Fitxategi hori ez da existitzen!!"
#define KG_MSSG_INVALIDFILE                 "Arazoren bat egon da fitxategiarekin ..."
#define KG_MSSG_EMPTYFILE                   "Fitxategia utsik dago"
#define KG_MSSG_FILEREAD                    "Fitxategiaren irakurketa buruta"
#define KG_MSSG_TABFILE                     "Ez dago objekturik tabulatzeko"
#define KG_MSSG_SUPRFILE                    "Ez dago objekturik ezabatzeko"
#define KG_MSSG_INFO                        "Objektuaren informazioa:"

#define KG_MSS_UP_EMPTY                     "Ez dago objekturik aurkitu GORA tekla sakatzean"
#define KG_MSS_DOWN_EMPTY                   "Ez dago objekturik aurkitu BEHERA tekla sakatzean"
#define KG_MSS_LEFT_EMPTY                   "Ez dago objekturik aurkitu EZKERRA tekla sakatzean"
#define KG_MSS_RIGHT_EMPTY                  "Ez dago objekturik aurkitu ESKUINA tekla sakatzean"
#define KG_MSSG_PAGE_UP_EMPTY               "Ez dago objekturik aurkitu AVPAG tekla sakatzean"
#define KG_MSSG_PAGE_DOWN_EMPTY             "Ez dago objekturik aurkitu REPAG tekla sakatzean"

#define KG_MSSG_TRANS                       "Traslazioa aktibatu da"
#define KG_MSSG_BIRAKETA                    "Biraketa aktibatu da"
#define KG_MSSG_TAMAINA                     "Tamaina aldaketa aktibatu da"

#define KG_MSSG_LOKALA                      "Aldaketa lokala aktibatu da"
#define KG_MSSG_GLOBALA                     "Aldaketa globala aktibatu da"
#define KG_MSS_OPTION_EMPTY                 "Ez dago aldaketa aukerarik aktibatuta. Hiru daude: Traslazioa, biraketa eta tamaina aldaketa"

#define KG_MSSG_KAMERA                      "Kamerari aldaketak aplikatzeko moduan zaude"
#define KG_MSSG_KAM_ORTO                    "Kamera ortografikoa aplikatu da"
#define KG_MSSG_KAM_OBJ_MOTA                "Perspektiba kamera aplikatu da"
#define KG_MSSG_KAM_IBIL                    "Kamera ibiltaria aplikatu da"
#define KG_MSSG_KAM_EMPTY                   "Ez dago objekturik kamara aldatzeko"

#define KG_MSSG_TRANSFORM                   "Objektuari aldaketak aplikatzeko moduan zaude"
#define KG_MSSG_TRANSFORM_EMPTY             "Ez duzu objektu bati aldaketak aplikatzeko tekla zapaldu ('o' edo 'O')"

#define KG_MSS_OPTION_MAIN_EMPTY            "Ez dago aukera nagusirik aukeratuta. Bi daude: Objektuari aplikatzeko (O) eta kamarari aplikatzeko (K)"

#define KG_STEP_MOVE                        5.0f
#define KG_STEP_ROTATE                      10.0f
#define KG_STEP_ZOOM                        0.75
#define KG_STEP_CAMERA_ANGLE                5.0f

#define KG_ORTHO_X_MIN_INIT                -5
#define KG_ORTHO_X_MAX_INIT                 5
#define KG_ORTHO_Y_MIN_INIT                -5
#define KG_ORTHO_Y_MAX_INIT                 5
#define KG_ORTHO_Z_MIN_INIT                -100
#define KG_ORTHO_Z_MAX_INIT                 10000

#define KG_COL_BACK_R                       0.00f
#define KG_COL_BACK_G                       0.00f
#define KG_COL_BACK_B                       0.00f
#define KG_COL_BACK_A                       0.50f

#define KG_COL_SELECTED_R                   1.00f
#define KG_COL_SELECTED_G                   0.00f
#define KG_COL_SELECTED_B                   0.00f

#define KG_COL_NONSELECTED_R                1.00f
#define KG_COL_NONSELECTED_G                1.00f
#define KG_COL_NONSELECTED_B                1.00f

#define KG_COL_X_AXIS_R                     0.0f
#define KG_COL_X_AXIS_G                     1.0f
#define KG_COL_X_AXIS_B                     0.0f

#define KG_COL_Y_AXIS_R                     0.0f
#define KG_COL_Y_AXIS_G                     1.0f
#define KG_COL_Y_AXIS_B                     0.0f

#define KG_COL_Z_AXIS_R                     0.0f
#define KG_COL_Z_AXIS_G                     1.0f
#define KG_COL_Z_AXIS_B                     0.0f

#define KG_MAX_DOUBLE                       100000000.0

#define TRASLAZIOA                          1
#define BIRAKETA                            2
#define TAMAINA                             3

#define LOKALA                              4
#define GLOBALA                             5

#define KAM_ORTO                            6
#define KAM_PERS                            7
#define KAM_IBIL                            8

#define TRANSFORMAZIOA                      9
#define KAMARA                              10

#define ARG_BONBILLA                        11
#define ARG_EGUZKIA                         12
#define ARG_FOKUA                           13

#define ARG_1                               14
#define ARG_2                               15
#define ARG_3                               16

#define ARGIA                               17

#define ARGIA_DESGAITU                      18
#define ARGIA_GAITU                         19

#define ARGI_ITZALI                         20
#define ARGI_PIZTU                          21

#define OBJE_FLAT                           22
#define OBJE_SMOOTH                         23

#define BAI                                 24
#define EZ                                  25

#define PI                                  3.1415926

/** STRUCTURES **/

/****************************
 * Structure to store the   *
 * coordinates of 3D points *
 ****************************/
typedef struct {
    GLdouble x, y, z;
} point3;

/*****************************
 * Structure to store the    *
 * coordinates of 3D vectors *
 *****************************/
typedef struct {
    GLdouble x, y, z;
} vector3;

/****************************
 * Structure to store the   *
 * colors in RGB mode       *
 ****************************/
typedef struct {
    GLfloat r, g, b;
} color3;

/****************************
 * Structure to store       *
 * objects' vertices         *
 ****************************/
typedef struct {
    point3 coord;                       /* coordinates,x, y, z */
    GLint num_faces;                    /* number of faces that share this vertex */
} vertex;

/****************************
 * Structure to store       *
 * objects' faces or        *
 * polygons                 *
 ****************************/
typedef struct {
    GLint num_vertices;                 /* number of vertices in the face */
    GLint *vertex_table;                /* table with the index of each vertex */
} face;

struct argia{
    GLfloat kokapena[4];
    GLfloat norabidea[4];
    struct argia *next;
};

typedef struct argia argia;

typedef struct material{
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
    
}material;

/****************************
 * 	     Pila           *
 ****************************/
struct elementua{
    GLdouble *matrizea;
    struct elementua *atzera;
    struct elementua *aurrera;
};

typedef struct elementua elementua;

/****************************
 * Structure to store a     *
 * pile of 3D objects       *
 ****************************/
struct object3d{
    GLint num_vertices;                 /* number of vertices in the object*/
    vertex *vertex_table;               /* table of vertices */
    GLint num_faces;                    /* number of faces in the object */
    face *face_table;                   /* table of faces */
    GLdouble *matrizea;
    elementua *pila;
    point3 min;                         /* coordinates' lower bounds */
    point3 max;                         /* coordinates' bigger bounds */
    struct material material;
    struct object3d *next;              /* next element in the pile of objects */
    vector3 *norm_taula;
};

typedef struct object3d object3d;

struct camera3d {
    GLdouble *eye;
    GLdouble *center;
    GLdouble *up;
    elementua *pila;
    double angelua;
};

typedef struct camera3d camera3d;

#endif // DEFINITIONS_H
