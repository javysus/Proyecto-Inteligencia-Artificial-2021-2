#include "alsp.h"
bool flag = false;

struct Nodo{
    int variable; //Posicion de avion del conflicto
    struct Nodo *next;
};

bool is_new(Node** head_ref, int new_data){
    Node* current = *head_ref;
    Node* next;
    bool new = true;

    while(current != NULL){
        if (current->variable == new_data){
            new = false;
        }
        next = current->next;
        current = next;
    }
    return new;
}

void push(Node** head_ref, int new_data)
{
    bool new = is_new(head_ref, new_data);
    if (new){
        /* 1. allocate node */
        Node* new_node = (Node*) malloc(sizeof(Node));
    
        /* 2. put in the data  */
        new_node->variable  = new_data;
        /* 3. Make next of new node as head */
        new_node->next = (*head_ref);
    
        /* 4. move the head to point to the new node */
        (*head_ref)    = new_node;
    }
}

void deleteList(Node** head_ref)
{
   /* deref head_ref to get the real head */
   Node* current = *head_ref;
   Node* next;
 
   while (current != NULL)
   {
       next = current->next;
       free(current);
       current = next;
   }
   
   /* deref head_ref to affect the real head back
      in the caller. */
   *head_ref = NULL;

}

void printList(Node** head_ref)
{
   /* deref head_ref to get the real head */
   Node* current = *head_ref;
   Node* next;
   
   printf("[ ");
   while (current != NULL)
   {
       next = current->next;
       printf("%d ", current->variable);
       current = next;
   }
   printf("]\n");
}

int saltoLista(Node** head_ref){
    Node* current = *head_ref;
    Node* next;
    int max = -1;

    while(current != NULL){
        if (current->variable>max){
            max = current->variable; //La mas recientemente instanciada sera el avion de mayor valor/indice
        }
        next = current->next;
        current = next;
    }
    return max;
}

bool restricciones(avion *avion_inst, int pos_avion, avion *aviones, salto_cbj *salto){
    //Restriccion de separacion de tiempos
    int conflicto;
    for (int i = 0; i < pos_avion; i++){
        //Se agrega el igual, ya que si son iguales de todas formas la segunda condicion no se cumplira
        //Avion instanciado aterriza despues del avion i y restriccion no se cumple
        //Avion i aterriza despues dea avion instanciado y restriccion no se cumple
        int j = aviones[i].id;

        //Chequeos con aviones instanciados anteriormente

        if ((avion_inst->xi >= aviones[i].xi && avion_inst->xi < aviones[i].xi + avion_inst->si[j]) ||
        (avion_inst->xi <= aviones[i].xi && aviones[i].xi < avion_inst->xi + avion_inst->si[j])){
            /*Conflicto
            Se agrega variable mas antiguamente instanciada*/

            if (salto->activado){
                conflicto = i; //La primera encontrada sera la mas antigua
                push(&avion_inst->aviones_conf, conflicto);
            }

            return false; //No se cumplen restricciones
        }
    }
    return true;//Se cumplen las restricciones
}

float calculoCosto(avion *aviones, int p){
    float costo = 0;
    for (int i = 0; i < p; i++)
    {   
        if(aviones[i].xi<=aviones[i].ti){ //Aterrizaje antes del tiempo ideal de aterrizaje
            costo = costo + aviones[i].gi * (aviones[i].ti - aviones[i].xi);
        } else { //Aterrizaje despues del tiempo ideal
            costo = costo + aviones[i].hi * (aviones[i].xi - aviones[i].ti);
        }
    }

    return costo;
}
void mejorSolucion(avion *aviones, sol *best_sol, int p){
    for (int i = 0; i < p; i++)
    {
        best_sol->xi[i] = aviones[i].xi; //Se modifica instanciacion
        best_sol->costo = calculoCosto(aviones, p);
        printf("%d ",aviones[i].xi);
    }
    printf("\n");
    
}

void printInstancia(avion *aviones, int p){
    for (int i = 0; i < p; i++)
    {
        printf("%d ",aviones[i].xi);
    }
    printf("\n");
}

int* ordenDominioInicial(avion avion){
    int ei = avion.ei;
    int li = avion.li;
    int largoDominio = (li-ei)+1; //Cantidad de valores en el dominio
    int* dominio = (int *)malloc(largoDominio*sizeof(int));

    for (int i = 0; i < largoDominio; i++){
        dominio[i] = ei + i;
    }

    return dominio;
}

int* ordenDominio(avion avion){
    int ei = avion.ei;
    int li = avion.li;
    int ti = avion.ti; 
    int largoDominio = (li-ei)+1; //Cantidad de valores en el dominio
    int* dominio = (int *)malloc(largoDominio*sizeof(int));
    dominio[0] = ti;
    bool rellenarMenores = false;
    bool rellenarMayores = false;
    int pos;
    int cont = 1; //Para ir sumando
    for (int i = 1; i < largoDominio; i = i+2) //i para posicion
    {
        /*Casos limite*/
        if(avion.ti == avion.ei){
            rellenarMayores = true;
            pos = i;
            break;
        } else if(avion.ti == avion.li){
            rellenarMenores = true;
            pos = i;
            break;
        }
        if(avion.gi > avion.hi){ //Si costo por aterrizar antes es mayor, se comienza por aterrizaje despues
            dominio[i] = ti + cont;
            dominio[i+1] = ti - cont; 
        } else {
            dominio[i] = ti - cont;
            dominio[i+1] = ti + cont; 
        }
        
        
        if (ti+cont == li){ //Si se llega al maximo posible de tiempo de aterrizaje
            rellenarMenores = true;
            pos = i+2;
            cont++;
            break;
        } else if (ti -cont == ei){ //Si se llega al minimo posible de tiempo
            rellenarMayores = true;
            pos = i+2;
            cont++;
            break;
        }
        cont++;
    }

    if(rellenarMenores){
        for (int j = pos; j < largoDominio; j++){
            dominio[j] = ti - cont;
            cont++;
        }
    } else if (rellenarMayores){
        for (int j = pos; j < largoDominio; j++){
            dominio[j] = ti + cont;
            cont++;
        }
    }
    //printf("\n");
    return dominio;
}

void ordenInstanciacion(avion *aviones, int p){
    for (int i = 0; i < p; ++i){
      for (int j = i + 1; j < p; ++j){
         if (aviones[i].mean_penalizacion < aviones[j].mean_penalizacion){
            avion a = aviones[i];
            aviones[i] = aviones[j];
            aviones[j] = a;
         }
      }
   }
}

void printInstanciacion(avion *aviones, int p){
    for (int i = 0; i < p; i++)
    {
        printf("%d ",aviones[i].id);
    }
    printf("\n");
}

void archivoMejorSolucion(avion *aviones, sol *best_sol, int p, char instancia[5], double time, salto_cbj *salto){

    char archivo[20];
    strcat(strcpy(archivo, "solucion_"), instancia);
    strcat(archivo, ".txt");

    FILE *solucion = fopen(archivo, "w");

    if (best_sol->first_sol){
        fprintf(solucion, "No se alcanzo a encontrar solucion completa\n");
        fprintf(solucion, "Costo: %f\n", calculoCosto(aviones, p));
        fprintf(solucion, "Tiempo total de ejecución: %f [s]\n", time);
        fprintf(solucion, "Instanciaciones totales en la ejecución: %d\n", salto->instanciaciones);

        for (int i = 0; i < p; i++)
        {
            fprintf(solucion, "Tiempo avióń %d: %d\n", aviones[i].id+1, aviones[i].xi);
        }
    }
    else{
        fprintf(solucion, "Costo: %f\n", best_sol->costo);
        fprintf(solucion, "Tiempo total de ejecución: %f [s]\n", time);
        fprintf(solucion, "Instanciaciones totales en la ejecución: %d\n", salto->instanciaciones);

        for (int i = 0; i < p; i++)
        {
            fprintf(solucion, "Tiempo avióń %d: %d\n", aviones[i].id+1, best_sol->xi[i]);
        }

    }

     fclose(solucion);
}

void liberarMemoria(avion *aviones, sol *best_sol, salto_cbj *salto, int p){
    //Liberar memoria utilizada
    for (int i=0; i < p; i++){
        free(aviones[i].si);
        free(aviones[i].dominio);
        deleteList(&aviones[i].aviones_conf);
    }
    free(salto);
    free(best_sol->xi);
    free(best_sol);
    free(aviones);
}

void eventoCtrlC(int sig){
    flag = true;
}

void cbj(avion *aviones, int pos_avion, salto_cbj *salto, int p, sol *best_sol, clock_t start, char instancia[5]){
    bool consistente = true;
    bool fallo = false;
    clock_t actual = clock();
    double cpu_time_used;

    cpu_time_used = ((double) (actual - start))/CLOCKS_PER_SEC; //Segundos

    signal(SIGINT, eventoCtrlC);

    if(cpu_time_used > 300.00 || flag){ //Modificar segundos en los que se permitira ejecutar la instancia
        archivoMejorSolucion(aviones, best_sol, p, instancia, cpu_time_used, salto);
        liberarMemoria(aviones, best_sol, salto, p);
        exit(0);
    }
   
    if(pos_avion-1 > 0){ 
        //Chequear conflictos de instanciacion anterior
        consistente = restricciones(&aviones[pos_avion-1], pos_avion-1, aviones, salto);
    }

    //Se llega a una solucion
    if(pos_avion==p && consistente){
        
        if(best_sol->first_sol){
            best_sol->first_sol = false;
            mejorSolucion(aviones, best_sol, p);
            printf("Costo: %f\n", best_sol->costo);
            printf("Tiempo: %f [s]\n", cpu_time_used);
            printf("Instanciaciones totales: %d\n", salto->instanciaciones);
            printf("Saltos totales: %d\n", salto->saltos);
        }
        else if (calculoCosto(aviones, p) < best_sol->costo){
            mejorSolucion(aviones, best_sol, p);
            printf("Costo: %f\n", best_sol->costo);
            printf("Tiempo: %f [s]\n", cpu_time_used);
            printf("Instanciaciones totales: %d\n", salto->instanciaciones);
            printf("Saltos totales: %d\n", salto->saltos);
        }

        if (best_sol->costo == 0.0){
            //Mejor solucion posible

            archivoMejorSolucion(aviones, best_sol, p, instancia, cpu_time_used, salto);
            liberarMemoria(aviones, best_sol, salto, p);
            exit(0);
        }

        //Desactivar salto inteligente
        salto->activado = false;
        return;

    }
    
    //Si no hay inconsistencias, se sigue con la instanciacion 
    if(consistente){
        int largoDominio = (aviones[pos_avion].li - aviones[pos_avion].ei) + 1;
        for (int i = 0; i < largoDominio; i++){ 
            aviones[pos_avion].xi = aviones[pos_avion].dominio[i]; //Instanciacion
            salto->instanciaciones = salto->instanciaciones + 1;

            cbj(aviones, pos_avion+1, salto, p, best_sol, start, instancia); //Siguiente variable

            if (!salto->activado && pos_avion == 0){ //Llegada a nodo raiz
                salto->activado = true; //Se vuelve a activar salto inteligente.
            } 
            
            else if (!salto->activado) { //Reiniciar lista de conflictos para los demas nodos
                deleteList(&aviones[pos_avion].aviones_conf);
            }

            else if (salto->activado && salto->fallo) { //Si el salto esta activado y hubo un fallo
                if(pos_avion>salto->salto){
                    //Reiniciar lista de conflictos y break
                    deleteList(&aviones[pos_avion].aviones_conf);
                    return;
                } else if (pos_avion==salto->salto){
                    //Reiniciar salto y seguir con instanciacion
                    salto->fallo = false;
                }
            }
        }

        if(salto->activado && pos_avion != 0){ //Al finalizar el nodo raiz termina
            //Se intento todo: fallo
            fallo = true;
        }
    }
    
    if(fallo){
        salto->fallo = true;

        /*Se actualiza el salto CBJ*/
        salto->salto = saltoLista(&aviones[pos_avion].aviones_conf);
        deleteList(&aviones[pos_avion].aviones_conf);
        salto->saltos = salto->saltos + 1;
        
    }


}
void lecturaArchivo(FILE* alsp, char instancia[5]){
    int p;
    fscanf(alsp, "%d", &p); //Cantidad de aviones

    avion *aviones;
    aviones = (avion *)malloc(p*sizeof(avion));

    for (int i = 0; i < p; i++)
    {
        //Leer informacion
        fscanf(alsp, "%d %d %d %f %f\n", &aviones[i].ei, &aviones[i].ti, &aviones[i].li, 
        &aviones[i].gi, &aviones[i].hi);

        //El id refleja que avion es, de 0 a p-1, para luego poder comparar tiempos de separacion
        aviones[i].id = i;
        aviones[i].mean_penalizacion = (aviones[i].gi + aviones[i].hi)/2.00;

        //Inicializar dominio
        /*ordenDominioInicial para orden de mas temprano a mas tardio, ordenDominio para orden final propuesto. Escoger una.*/
        //aviones[i].dominio = ordenDominioInicial(aviones[i]);
        aviones[i].dominio = ordenDominio(aviones[i]);

        //Inicializar lista de conflictos
        aviones[i].aviones_conf = NULL;


        //Inicializar arreglo de tiempos de separacion
        aviones[i].si = (int *)malloc(p*sizeof(int));
        for (int j = 0; j < p; j++){
            fscanf(alsp, "%d", &aviones[i].si[j]);
        }
    }
    fclose(alsp);
    /*Se ordena el arreglo de aviones*/
    /*Comentar la siguiente linea para orden dado de variables, descomentar para implementacion final*/
    ordenInstanciacion(aviones, p);

    salto_cbj *salto = (salto_cbj *)malloc(sizeof(salto_cbj)); 
    sol *best_sol = (sol *)malloc(sizeof(sol)); 
    
    //Inicializar mejor solucion
    best_sol->xi = (int *)malloc(p*sizeof(int));
    best_sol->first_sol = true; //Aun no se encuentra una solucion
    best_sol->costo = 0;

    //Inicializar salto activado
    salto->activado = true; 
    salto->fallo = false;
    salto->instanciaciones = 0;
    salto->salto = 0;
    //Ejecutar CBJ
    clock_t start;
    start = clock();
    cbj(aviones, 0, salto, p, best_sol, start, instancia);

    clock_t actual = clock();
    double cpu_time_used;

    cpu_time_used = ((double) (actual - start))/CLOCKS_PER_SEC; //Segundos

    archivoMejorSolucion(aviones, best_sol, p, instancia, cpu_time_used, salto);
}
