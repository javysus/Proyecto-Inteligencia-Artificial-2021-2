#ifndef ALSP_H
#define ALSP_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <signal.h>
extern bool flag;

/*Estructura para almacenar las listas*/
typedef struct Nodo Node;

/*Estructura para almacenar los aviones*/
typedef struct 
{
    int id; //Indice del avion, aviones de 0 a p
    int ei; //Tiempo mas temprano de aterrizaje
    int ti; //Tiempo ideal de aterrizaje
    int li; //Tiempo mas tardio de aterrizaje
    float gi; //Penalizacion por aterrizar antes de Ti
    float hi; //Penalizacion por aterrizar despues de Ti
    int* si; //Tiempo de separacion entre avion i y otros aviones
    int xi; //Variable, tiempo asignado al avion
    int* dominio; //Arreglo que contiene el dominio a instanciar en orden
    float mean_penalizacion; //Promedio de la penalizacion
    struct Nodo* aviones_conf; //Lista de conflictos del avion para CBJ
}avion;

/*Estructura para almacenar informacion asociada a CBJ*/
typedef struct
{
    bool fallo; //Deteccion de fallo
    bool activado; //Si salto esta activado o no
    int salto; //Variable a la que saltar en caso de fallo
    int instanciaciones;
    int saltos;
}salto_cbj;

/*Estructura para almacenar las mejores soluciones*/
typedef struct
{
    int* xi; //Instanciacion de la solucion
    float costo; //Costo total de la solucion
    bool first_sol; //Si es la primera sol encontrada
}sol;

/*Funcion para verificar si un nodo de un valor entero ya se encuentra en una lista*/
bool is_new(Node** head_ref, int new_data);

/*Funcion para agregar un nuevo elemento a una lista*/
void push(Node** head_ref, int new_data);

/*Funcion para eliminar una lista*/
void deleteList(Node** head_ref);

/*Funcion para retornar a que variable salto dado una lista de conflictos de una variable*/
int saltoLista(Node** head_ref);

/*Funcion para verificar si se cumplen las restricciones de una instanciacion*/
bool restricciones(avion *avion_inst, int pos_avion, avion *aviones, salto_cbj *salto);

/*Funcion para calcular el costo de una instanciacion*/
float calculoCosto(avion *aviones, int p);

/*Funcion para actualizar la mejor solucion*/
void mejorSolucion(avion *aviones, sol *best_sol, int p);

/*Funcion para ordenar el dominio de una variable segun su alejamiento en unidades de tiempo
del tiempo ideal de aterrizaje*/
int* ordenDominio(avion avion);

/*Funcion para ordenar el arreglo de aviones segun el orden de instanciacion de las variables,
escogido segun costo promedio entre costo por aterrizar antes y despues*/
void ordenInstanciacion(avion *aviones, int p);

/*Funcion para crear el archivo txt con la mejor solucion encontrada en su totalidad, o hasta
cierto tiempo de iteracion*/
void archivoMejorSolucion(avion *aviones, sol *best_sol, int p, char instancia[5], double time, salto_cbj *salto);

/*Funcion para liberar la memoria utilizada por arreglos y listas*/
void liberarMemoria(avion *aviones, sol *best_sol, salto_cbj *salto, int p);

/*Funcion principal que realiza backtracking con CBJ*/
void cbj(avion *aviones, int pos_avion, salto_cbj *salto, int p, sol *best_sol, clock_t start, char instancia[5]);

/*Funcion para leer el archivo que luego da comienzo a BT + CBJ*/
void lecturaArchivo(FILE* alsp, char instancia[5]);

/*Funcion para escribir la solucion luego de una señal CTRL+C*/
void eventoCtrlC(int sig);

/*Funciones que se utilizaron para imprimir*/
void printList(Node** head_ref);
void printInstancia(avion *aviones, int p);
void printInstanciacion(avion *aviones, int p);
#endif