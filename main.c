#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "atajos.h"
#include "validaciones.h"
#include "formatos.h"



typedef struct
{

    char id[6];
    char nombre[50];
    char telefono[20];
    int activo; // 1 activo, 0 inactivo

}CLIENTE;

typedef struct
{

    char idpaquete[6];
    char descripcion[100];
    float peso;
    int fragil;  // 1 si, 0 no
    int activo;  // 1 activo, 0 inactivo

}PAQUETE;

typedef struct
{

    char idenvio[6];
    char idcliente[6];
    char idpaquete[6];
    char origen[50];
    char destino[50];
    float costo;
    int estado;  // 1 en transito, 2 entregado, 0 cancelado

}ENVIO;

typedef struct
{

    float valorporlibra;

}TARIFA;


typedef struct nodocliente
{

    CLIENTE cliente;
    struct nodocliente *sig;
    struct nodocliente *ant;

}NODOCLIENTE;

typedef struct nodopaquete
{

    PAQUETE paquete;
    struct nodopaquete *sig;
    struct nodopaquete *ant;

}NODOPAQUETE;

typedef struct nodoenvio
{

    ENVIO envio;
    struct nodoenvio *sig;
    struct nodoenvio *ant;

}NODOENVIO;

//Pongo los includes restantes aqui abajo porque estos usan las estructuras en sus prototipos y asi no me dan error
#include "archivos.h"
#include "reportes.h"


void insertarcliente(NODOCLIENTE **cabeza, CLIENTE cliente);
void insertarpaquete(NODOPAQUETE **cabeza, PAQUETE paquete);
void insertarenvio(NODOENVIO **cabeza, ENVIO envio);
NODOCLIENTE *buscarcliente(NODOCLIENTE *cabeza, char *id);
NODOPAQUETE *buscarpaquete(NODOPAQUETE *cabeza, char *id);
NODOENVIO *buscarenvio(NODOENVIO *cabeza, char *id);
void borrarcliente(NODOCLIENTE **cabeza, char *id);
void borrarpaquete(NODOPAQUETE **cabeza, char *id);
int idclientedisponible(NODOCLIENTE *cabeza, char *id);
int idpaquetedisponible(NODOPAQUETE *cabeza, char *id);
int idenviodisponible(NODOENVIO *cabeza, char *id);
int clientetieneenvios(NODOENVIO *cabeza, char *idcliente);
int paquetetieneenvios(NODOENVIO *cabeza, char *idpaquete);
void menuprincipal(NODOCLIENTE **cabezaclientes, NODOPAQUETE **cabezapaquetes, NODOENVIO **cabezaenvios, TARIFA *tarifa);
void menuclientes(NODOCLIENTE **cabezaclientes, NODOENVIO *cabezaenvios);
void menupaquetes(NODOPAQUETE **cabezapaquetes, NODOENVIO *cabezaenvios);
void menuenvios(NODOENVIO **cabezaenvios, NODOCLIENTE *cabezaclientes, NODOPAQUETE *cabezapaquetes, TARIFA tarifa);
void menureportes(NODOENVIO *cabezaenvios, NODOCLIENTE *cabezaclientes, NODOPAQUETE *cabezapaquetes);
void menutarifa(TARIFA *tarifa);
void crearcliente(NODOCLIENTE **cabezaclientes);
void listarclientes(NODOCLIENTE *cabezaclientes);
void modificarcliente(NODOCLIENTE *cabezaclientes);
void eliminarcliente(NODOCLIENTE **cabezaclientes, NODOENVIO *cabezaenvios);
void crearpaquete(NODOPAQUETE **cabezapaquetes);
void listarpaquetes(NODOPAQUETE *cabezapaquetes);
void modificarpaquete(NODOPAQUETE *cabezapaquetes);
void eliminarpaquete(NODOPAQUETE **cabezapaquetes, NODOENVIO *cabezaenvios);
void crearenvio(NODOENVIO **cabezaenvios, NODOCLIENTE *cabezaclientes, NODOPAQUETE *cabezapaquetes, TARIFA tarifa);
void listarenvios(NODOENVIO *cabezaenvios);
void verdetalleenvio(NODOENVIO *cabezaenvios, NODOCLIENTE *cabezaclientes, NODOPAQUETE *cabezapaquetes);
void cambiarestado(NODOENVIO *cabezaenvios);
void repenviosdestino(NODOENVIO *cabezaenvios, NODOCLIENTE *cabezaclientes, NODOPAQUETE *cabezapaquetes);
void repclientemasenvios(NODOENVIO *cabezaenvios, NODOCLIENTE *cabezaclientes);
void reptotalingresos(NODOENVIO *cabezaenvios);
void repentregados(NODOENVIO *cabezaenvios);
void repentransito(NODOENVIO *cabezaenvios);
void repcancelados(NODOENVIO *cabezaenvios, NODOCLIENTE *cabezaclientes);
void ordenarclientes(NODOCLIENTE **cabeza);
void ordenarpaquetes(NODOPAQUETE **cabeza);
void ordenarenvios(NODOENVIO **cabeza);


int main()
{

    NODOCLIENTE *cabezaclientes;
    NODOPAQUETE *cabezapaquetes;
    NODOENVIO   *cabezaenvios;
    TARIFA tarifa;

    cabezaclientes = NULL;
    cabezapaquetes = NULL;
    cabezaenvios   = NULL;

    cargarclientes(&cabezaclientes);
    cargarpaquetes(&cabezapaquetes);
    cargarenvios(&cabezaenvios);

    // si noexiste o falla el archivo que tiene las tarifas se le va a poner una por defecto que es 200
    if(!cargartarifa(&tarifa))
    {
        tarifa.valorporlibra = 200;
    }

    menuprincipal(&cabezaclientes, &cabezapaquetes, &cabezaenvios, &tarifa);

    guardarclientes(cabezaclientes);
    guardarpaquetes(cabezapaquetes);
    guardarenvios(cabezaenvios);
    guardartarifa(tarifa);

    return 0;
}


/*
Funcion: guardarclientes
Argumentos: NODOCLIENTE *cabeza
Objetivo: recorre la lista y escribe cada cliente en el archivo binario
Retorno: ninguno
*/
void guardarclientes(NODOCLIENTE *cabeza)
{

    FILE *archivo;
    NODOCLIENTE *indice;

    archivo = fopen("clientes.dat", "wb");
    if(archivo == NULL)
    {
        printf("Error guardando clientes\n");
        return;
    }

    for(indice = cabeza; indice; indice = indice->sig)
    {
        fwrite(&indice->cliente, sizeof(CLIENTE), 1, archivo);
    }

    fclose(archivo);
}

/*
Funcion: cargarclientes
Argumentos: NODOCLIENTE **cabeza
Objetivo: lee el archivo binario e inserta cada cliente en la lista
Retorno: ninguno
*/
void cargarclientes(NODOCLIENTE **cabeza)
{

    FILE *archivo;
    CLIENTE clienteleido;

    archivo = fopen("clientes.dat", "rb");
    if(archivo == NULL)
    {
        return;
    }

    while(fread(&clienteleido, sizeof(CLIENTE), 1, archivo) == 1)
    {
        insertarcliente(cabeza, clienteleido);
    }

    fclose(archivo);
}

/*
Funcion: guardarpaquetes
Argumentos: NODOPAQUETE *cabeza
Objetivo: recorre la lista y escribe cada paquete en el archivo binario
Retorno: ninguno
*/
void guardarpaquetes(NODOPAQUETE *cabeza)
{

    FILE *archivo;
    NODOPAQUETE *indice;

    archivo = fopen("paquetes.dat", "wb");
    if(archivo == NULL)
    {
        printf("Error guardando paquetes\n");
        return;
    }

    for(indice = cabeza; indice; indice = indice->sig)
    {
        fwrite(&indice->paquete, sizeof(PAQUETE), 1, archivo);
    }

    fclose(archivo);
}

/*
Funcion: cargarpaquetes
Argumentos: NODOPAQUETE **cabeza
Objetivo: lee el archivo binario e inserta cada paquete en la lista
Retorno: ninguno
*/
void cargarpaquetes(NODOPAQUETE **cabeza)
{

    FILE *archivo;
    PAQUETE paqueteleido;

    archivo = fopen("paquetes.dat", "rb");
    if(archivo == NULL)
    {
        return;
    }

    while(fread(&paqueteleido, sizeof(PAQUETE), 1, archivo) == 1)
    {
        insertarpaquete(cabeza, paqueteleido);
    }

    fclose(archivo);
}

/*
Funcion: guardarenvios
Argumentos: NODOENVIO *cabeza
Objetivo: recorre la lista y escribe cada envio en el archivo binario
Retorno: ninguno
*/
void guardarenvios(NODOENVIO *cabeza)
{

    FILE *archivo;
    NODOENVIO *indice;

    archivo = fopen("envios.dat", "wb");
    if(archivo == NULL)
    {
        printf("Error guardando envios\n");
        return;
    }

    for(indice = cabeza; indice; indice = indice->sig)
    {
        fwrite(&indice->envio, sizeof(ENVIO), 1, archivo);
    }

    fclose(archivo);
}

/*
Funcion: cargarenvios
Argumentos: NODOENVIO **cabeza
Objetivo: lee el archivo binario e inserta cada envio en la lista
Retorno: ninguno
*/
void cargarenvios(NODOENVIO **cabeza)
{

    FILE *archivo;
    ENVIO envioleido;

    archivo = fopen("envios.dat", "rb");
    if(archivo == NULL)
    {
        return;
    }

    while(fread(&envioleido, sizeof(ENVIO), 1, archivo) == 1)
    {
        insertarenvio(cabeza, envioleido);
    }

    fclose(archivo);
}

/*
Funcion: guardartarifa
Argumentos: TARIFA tarifa
Objetivo: guarda la tarifa actual en su propio archivo binario
Retorno: ninguno
*/
void guardartarifa(TARIFA tarifa)
{

    FILE *archivo;

    archivo = fopen("tarifa.dat", "wb");
    if(archivo == NULL)
    {
        return;
    }

    fwrite(&tarifa, sizeof(TARIFA), 1, archivo);
    fclose(archivo);
}

/*
Funcion: cargartarifa
Argumentos: TARIFA *tarifa
Objetivo: carga la tarifa desde el archivo, retorna 0 si no existe
Retorno: int - 1 si cargo bien, 0 si no existe el archivo
*/
int cargartarifa(TARIFA *tarifa)
{

    FILE *archivo;

    archivo = fopen("tarifa.dat", "rb");
    if(archivo == NULL)
    {
        return 0;
    }

    fread(tarifa, sizeof(TARIFA), 1, archivo);
    fclose(archivo);
    return 1;
}


/*
Funcion: insertarcliente
Argumentos: NODOCLIENTE **cabeza, CLIENTE cliente
Objetivo: inserta un cliente al final de la lista enlazada
Retorno: ninguno
*/
void insertarcliente(NODOCLIENTE **cabeza, CLIENTE cliente)
{

    NODOCLIENTE *nuevo;
    NODOCLIENTE *indice;

    nuevo = (NODOCLIENTE *) malloc(sizeof(NODOCLIENTE));
    nuevo->cliente = cliente;
    nuevo->sig = NULL;
    nuevo->ant = NULL;

    if(*cabeza == NULL)
    {
        *cabeza = nuevo;
        return;
    }


    for(indice = *cabeza; indice->sig; indice = indice->sig);
    indice->sig = nuevo;
    nuevo->ant = indice;
}

/*
Funcion: insertarpaquete
Argumentos: NODOPAQUETE **cabeza, PAQUETE paquete
Objetivo: inserta un paquete al final de la lista enlazada
Retorno: ninguno
*/
void insertarpaquete(NODOPAQUETE **cabeza, PAQUETE paquete)
{

    NODOPAQUETE *nuevo;
    NODOPAQUETE *indice;

    nuevo = (NODOPAQUETE *) malloc(sizeof(NODOPAQUETE));
    nuevo->paquete = paquete;
    nuevo->sig = NULL;
    nuevo->ant = NULL;

    if(*cabeza == NULL)
    {
        *cabeza = nuevo;
        return;
    }

    for(indice = *cabeza; indice->sig; indice = indice->sig);
    indice->sig = nuevo;
    nuevo->ant = indice;
}

/*
Funcion: insertarenvio
Argumentos: NODOENVIO **cabeza, ENVIO envio
Objetivo: inserta un envio al final de la lista enlazada
Retorno: ninguno
*/
void insertarenvio(NODOENVIO **cabeza, ENVIO envio)
{

    NODOENVIO *nuevo;
    NODOENVIO *indice;

    nuevo = (NODOENVIO *) malloc(sizeof(NODOENVIO));
    nuevo->envio = envio;
    nuevo->sig = NULL;
    nuevo->ant = NULL;

    if(*cabeza == NULL)
    {
        *cabeza = nuevo;
        return;
    }

    for(indice = *cabeza; indice->sig; indice = indice->sig);
    indice->sig = nuevo;
    nuevo->ant = indice;
}

/*
Funcion: buscarcliente
Argumentos: NODOCLIENTE *cabeza, char *id
Objetivo: busca un cliente por su id y retorna el nodo si lo encuentra
Retorno: NODOCLIENTE* - el nodo encontrado o NULL
*/
NODOCLIENTE *buscarcliente(NODOCLIENTE *cabeza, char *id)
{

    NODOCLIENTE *indice;

    for(indice = cabeza; indice; indice = indice->sig)
    {
        if(strcmp(indice->cliente.id, id) == 0)
        {
            return indice;
        }
    }
    return NULL;
}

/*
Funcion: buscarpaquete
Argumentos: NODOPAQUETE *cabeza, char *id
Objetivo: busca un paquete por su id y retorna el nodo si lo encuentra
Retorno: NODOPAQUETE* - el nodo encontrado o NULL
*/
NODOPAQUETE *buscarpaquete(NODOPAQUETE *cabeza, char *id)
{

    NODOPAQUETE *indice;

    for(indice = cabeza; indice; indice = indice->sig)
    {
        if(strcmp(indice->paquete.idpaquete, id) == 0)
        {
            return indice;
        }
    }
    return NULL;
}

/*
Funcion: buscarenvio
Argumentos: NODOENVIO *cabeza, char *id
Objetivo: busca un envio por su id y retorna el nodo si lo encuentra
Retorno: NODOENVIO* - el nodo encontrado o NULL
*/
NODOENVIO *buscarenvio(NODOENVIO *cabeza, char *id)
{

    NODOENVIO *indice;

    for(indice = cabeza; indice; indice = indice->sig)
    {
        if(strcmp(indice->envio.idenvio, id) == 0)
        {
            return indice;
        }
    }
    return NULL;
}

/*
Funcion: borrarcliente
Argumentos: NODOCLIENTE **cabeza, char *id
Objetivo: elimina fisicamente el nodo del cliente de la lista
Retorno: ninguno
*/
void borrarcliente(NODOCLIENTE **cabeza, char *id)
{

    NODOCLIENTE *nodo;

    nodo = buscarcliente(*cabeza, id);
    if(nodo == NULL)
    {
        return;
    }


    if(nodo->ant != NULL)
    {
        nodo->ant->sig = nodo->sig;
    }
    else
    {
        *cabeza = nodo->sig;
    }

    if(nodo->sig != NULL)
    {
        nodo->sig->ant = nodo->ant;
    }

    free(nodo);
}

/*
Funcion: borrarpaquete
Argumentos: NODOPAQUETE **cabeza, char *id
Objetivo: elimina fisicamente el nodo del paquete de la lista
Retorno: ninguno
*/
void borrarpaquete(NODOPAQUETE **cabeza, char *id)
{

    NODOPAQUETE *nodo;

    nodo = buscarpaquete(*cabeza, id);
    if(nodo == NULL)
    {
        return;
    }

    if(nodo->ant != NULL)
    {
        nodo->ant->sig = nodo->sig;
    }
    else
    {
        *cabeza = nodo->sig;
    }

    if(nodo->sig != NULL)
    {
        nodo->sig->ant = nodo->ant;
    }

    free(nodo);
}

/*
Funcion: idclientedisponible
Argumentos: NODOCLIENTE *cabeza, char *id
Objetivo: verifica que el id no exista ya y que tenga un tamanio valido
Retorno: int - 1 si esta disponible, 0 si no
*/
int idclientedisponible(NODOCLIENTE *cabeza, char *id)
{

    if(strlen(id) == 0 || strlen(id) > 5)
    {
        return 0;
    }

    return buscarcliente(cabeza, id) == NULL;
}

/*
Funcion: idpaquetedisponible
Argumentos: NODOPAQUETE *cabeza, char *id
Objetivo: verifica que el id no exista ya y que tenga un tamanio valido
Retorno: int - 1 si esta disponible, 0 si no
*/
int idpaquetedisponible(NODOPAQUETE *cabeza, char *id)
{

    if(strlen(id) == 0 || strlen(id) > 5)
    {
        return 0;
    }

    return buscarpaquete(cabeza, id) == NULL;
}

/*
Funcion: idenviodisponible
Argumentos: NODOENVIO *cabeza, char *id
Objetivo: verifica que el id no exista ya y que tenga un tamanio valido
Retorno: int - 1 si esta disponible, 0 si no
*/
int idenviodisponible(NODOENVIO *cabeza, char *id)
{

    if(strlen(id) == 0 || strlen(id) > 5)
    {
        return 0;
    }

    return buscarenvio(cabeza, id) == NULL;
}

/*
Funcion: clientetieneenvios
Argumentos: NODOENVIO *cabeza, char *idcliente
Objetivo: revisa si algun envio tiene ese idcliente asociado
Retorno: int - 1 si tiene envios, 0 si no
*/
int clientetieneenvios(NODOENVIO *cabeza, char *idcliente)
{

    NODOENVIO *indice;

    for(indice = cabeza; indice; indice = indice->sig)
    {
        if(strcmp(indice->envio.idcliente, idcliente) == 0)
        {
            return 1;
        }
    }
    return 0;
}

/*
Funcion: paquetetieneenvios
Argumentos: NODOENVIO *cabeza, char *idpaquete
Objetivo: revisa si algun envio tiene ese idpaquete asociado
Retorno: int - 1 si tiene envios, 0 si no
*/
int paquetetieneenvios(NODOENVIO *cabeza, char *idpaquete)
{

    NODOENVIO *indice;

    for(indice = cabeza; indice; indice = indice->sig)
    {
        if(strcmp(indice->envio.idpaquete, idpaquete) == 0)
        {
            return 1;
        }
    }
    return 0;
}


/*
Funcion: menuprincipal
Argumentos: punteros a las tres listas y a la tarifa
Objetivo: muestra el menu principal y redirige a cada submenu
Retorno: ninguno
*/
void menuprincipal(NODOCLIENTE **cabezaclientes, NODOPAQUETE **cabezapaquetes, NODOENVIO **cabezaenvios, TARIFA *tarifa)
{

    int opcion;

    do
    {
        limpiarpantalla();
        imprimirtitulo("QuickShip");
        printf("1. Clientes\n");
        printf("2. Paquetes\n");
        printf("3. Env%cos\n", 161);
        printf("4. Reportes\n");
        printf("5. Configurar tarifa\n");
        printf("0. Salir\n");
        printf("\nSeleccione una opci%cn: ", 162);
        scanf("%d", &opcion);
        limpiarbuffer();

        if(opcion == 1)
        {
            menuclientes(cabezaclientes, *cabezaenvios);
        }
        else if(opcion == 2)
        {
            menupaquetes(cabezapaquetes, *cabezaenvios);
        }
        else if(opcion == 3)
        {
            menuenvios(cabezaenvios, *cabezaclientes, *cabezapaquetes, *tarifa);
        }
        else if(opcion == 4)
        {
            menureportes(*cabezaenvios, *cabezaclientes, *cabezapaquetes);
        }
        else if(opcion == 5)
        {
            menutarifa(tarifa);
        }
    }
    while(opcion != 0);
}

/*
Funcion: menuclientes
Argumentos: NODOCLIENTE **cabezaclientes, NODOENVIO *cabezaenvios
Objetivo: submenu de clientes con las 4 operaciones del CRUD
Retorno: ninguno
*/
void menuclientes(NODOCLIENTE **cabezaclientes, NODOENVIO *cabezaenvios)
{

    int opcion;

    do
    {
        limpiarpantalla();
        imprimirtitulo("MENU DE  CLIENTES");
        printf("1. Crear cliente\n");
        printf("2. Listar clientes\n");
        printf("3. Modificar cliente\n");
        printf("4. Eliminar / Inactivar cliente\n");
        printf("0. Volver\n");
        printf("\nSeleccione una opci%cn: ", 162);
        scanf("%d", &opcion);
        limpiarbuffer();

        if(opcion == 1)
        {
            limpiarpantalla();
            crearcliente(cabezaclientes);
        }
        else if(opcion == 2)
        {
            limpiarpantalla();
            listarclientes(*cabezaclientes);
            pausar();
        }
        else if(opcion == 3)
        {
            limpiarpantalla();
            modificarcliente(*cabezaclientes);
            pausar();
        }
        else if(opcion == 4)
        {
            limpiarpantalla();
            eliminarcliente(cabezaclientes, cabezaenvios);
            pausar();
        }
    }
    while(opcion != 0);
}

/*
Funcion: menupaquetes
Argumentos: NODOPAQUETE **cabezapaquetes, NODOENVIO *cabezaenvios
Objetivo: submenu de paquetes con las 4 operaciones del CRUD
Retorno: ninguno
*/
void menupaquetes(NODOPAQUETE **cabezapaquetes, NODOENVIO *cabezaenvios)
{

    int opcion;

    do
    {
        limpiarpantalla();
        imprimirtitulo("MENU DE PAQUETES");
        printf("1. Crear paquete\n");
        printf("2. Listar paquetes\n");
        printf("3. Modificar paquete\n");
        printf("4. Eliminar / Inactivar paquete\n");
        printf("0. Volver\n");
        printf("\nSeleccione una opci%cn: ", 162);
        scanf("%d", &opcion);
        limpiarbuffer();

        if(opcion == 1)
        {
            limpiarpantalla();
            crearpaquete(cabezapaquetes);
        }
        else if(opcion == 2)
        {
            limpiarpantalla();
            listarpaquetes(*cabezapaquetes);
            pausar();
        }
        else if(opcion == 3)
        {
            limpiarpantalla();
            modificarpaquete(*cabezapaquetes);
            pausar();
        }
        else if(opcion == 4)
        {
            limpiarpantalla();
            eliminarpaquete(cabezapaquetes, cabezaenvios);
            pausar();
        }
    }
    while(opcion != 0);
}

/*
Funcion: menuenvios
Argumentos: puntero a lista de envios, listas de clientes y paquetes, tarifa
Objetivo: submenu de envios con crear, listar, detalle y cambio de estado
Retorno: ninguno
*/
void menuenvios(NODOENVIO **cabezaenvios, NODOCLIENTE *cabezaclientes, NODOPAQUETE *cabezapaquetes, TARIFA tarifa)
{

    int opcion;

    do
    {
        limpiarpantalla();
        imprimirtitulo("MENU DE ENVIOS");
        printf("1. Crear env%co\n", 161);
        printf("2. Listar env%cos\n", 161);
        printf("3. Ver detalle de env%co\n", 161);
        printf("4. Cambiar estado de env%co\n", 161);
        printf("0. Volver\n");
        printf("\nSeleccione una opci%cn: ", 162);
        scanf("%d", &opcion);
        limpiarbuffer();

        if(opcion == 1)
        {
            limpiarpantalla();
            crearenvio(cabezaenvios, cabezaclientes, cabezapaquetes, tarifa);
        }
        else if(opcion == 2)
        {
            limpiarpantalla();
            listarenvios(*cabezaenvios);
            pausar();
        }
        else if(opcion == 3)
        {
            limpiarpantalla();
            verdetalleenvio(*cabezaenvios, cabezaclientes, cabezapaquetes);
            pausar();
        }
        else if(opcion == 4)
        {
            limpiarpantalla();
            cambiarestado(*cabezaenvios);
            pausar();
        }
    }
    while(opcion != 0);
}

/*
Funcion: menureportes
Argumentos: las tres listas
Objetivo: submenu de reportes con las 6 opciones pedidas en el enunciado
Retorno: ninguno
*/
void menureportes(NODOENVIO *cabezaenvios, NODOCLIENTE *cabezaclientes, NODOPAQUETE *cabezapaquetes)
{

    int opcion;

    do
    {
        limpiarpantalla();
        imprimirtitulo("MENU REPORTES");
        printf("1. Env%cos por destino\n", 161);
        printf("2. Cliente con m%cs env%cos\n", 160, 161);
        printf("3. Total generado\n");
        printf("4. Env%cos entregados\n", 161);
        printf("5. Env%cos en tr%cnsito\n", 161, 160);
        printf("6. Env%cos cancelados\n", 161);
        printf("0. Volver\n");
        printf("\nSeleccione una opci%cn: ", 162);
        scanf("%d", &opcion);
        limpiarbuffer();

        limpiarpantalla();

        if(opcion == 1)
        {
            repenviosdestino(cabezaenvios, cabezaclientes, cabezapaquetes);
            pausar();
        }
        else if(opcion == 2)
        {
            repclientemasenvios(cabezaenvios, cabezaclientes);
            pausar();
        }
        else if(opcion == 3)
        {
            reptotalingresos(cabezaenvios);
            pausar();
        }
        else if(opcion == 4)
        {
            repentregados(cabezaenvios);
            pausar();
        }
        else if(opcion == 5)
        {
            repentransito(cabezaenvios);
            pausar();
        }
        else if(opcion == 6)
        {
            repcancelados(cabezaenvios, cabezaclientes);
            pausar();
        }
    }
    while(opcion != 0);
}

/*
Funcion: menutarifa
Argumentos: TARIFA *tarifa
Objetivo: permite cambiar el valor de la tarifa y guarda si el usuario confirma
Retorno: ninguno
*/
void menutarifa(TARIFA *tarifa)
{

    char confirmacion;

    limpiarpantalla();
    imprimirtitulo("CONFIGURAR TARIFA");
    printf("Tarifa actual: %.2f por libra\n", tarifa->valorporlibra);
    printf("Nueva tarifa: ");
    scanf("%f", &tarifa->valorporlibra);
    limpiarbuffer();

    if(tarifa->valorporlibra <= 0)
    {
        printf("La tarifa tiene que ser mayor a 0\n");
        tarifa->valorporlibra = 100.0;
    }
    else
    {
        printf("Guardar cambios? (S/N): ");
        scanf(" %c", &confirmacion);
        limpiarbuffer();

        if(confirmacion == 'S' || confirmacion == 's')
        {
            guardartarifa(*tarifa);
            printf("Tarifa actualizada\n");
        }
        else
        {
            printf("Cancelado\n");
        }
    }
    pausar();
}


/*
Funcion: crearcliente
Argumentos: NODOCLIENTE **cabezaclientes
Objetivo: pide los datos del cliente, los valida y lo inserta en la lista
Retorno: ninguno
*/
void crearcliente(NODOCLIENTE **cabezaclientes)
{

    CLIENTE nuevocliente;
    char entrada[100];

    imprimirtitulo("Crear Cliente");

    do
    {
        printf("Ingrese ID (X=cancelar): ");
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0';

        if(esCancelar(entrada))
        {
            printf("Cancelado\n");
            pausar();
            return;
        }

        if(!idclientedisponible(*cabezaclientes, entrada))
        {
            printf("ID invalido o ya existe\n");
        }
        else
        {
            break;
        }
    }
    while(1);
    strcpy(nuevocliente.id, entrada);

    do
    {
        printf("Ingrese nombre (X=cancelar): ");
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0';

        if(esCancelar(entrada))
        {
            printf("Cancelado\n");
            pausar();
            return;
        }

        if(!sololetras(entrada))
        {
            printf("Solo letras, espacios y guiones\n");
        }
        else
        {
            break;
        }
    }
    while(1);
    strcpy(nuevocliente.nombre, entrada);

    do
    {
        printf("Ingrese telefono (X=cancelar): ");
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0';

        if(esCancelar(entrada))
        {
            printf("Cancelado\n");
            pausar();
            return;
        }

        if(!validartelefono(entrada))
        {
            printf("Solo numeros, espacios y guiones\n");
        }
        else
        {
            break;
        }
    }
    while(1);
    strcpy(nuevocliente.telefono, entrada);

    nuevocliente.activo = 1;
    insertarcliente(cabezaclientes, nuevocliente);
    guardarclientes(*cabezaclientes);
    printf("\nCliente creado exitosamente\n");
    pausar();
}

/*
Funcion: listarclientes
Argumentos: NODOCLIENTE *cabezaclientes
Objetivo: muestra todos los clientes de la lista en formato de tabla
Retorno: ninguno
*/
void listarclientes(NODOCLIENTE *cabezaclientes)
{

    NODOCLIENTE *indice;

    imprimirtitulo("Listar Clientes");
    ordenarclientes(&cabezaclientes);
    mostrarencclientes();

    if(cabezaclientes == NULL)
    {
        printf("No hay clientes registrados\n");
        return;
    }

    for(indice = cabezaclientes; indice; indice = indice->sig)
    {
        mostrarfilacliente(indice->cliente.id, indice->cliente.nombre,
                           indice->cliente.telefono, indice->cliente.activo);
    }
}

/*
Funcion: modificarcliente
Argumentos: NODOCLIENTE *cabezaclientes
Objetivo: busca un cliente por id y permite editar nombre, telefono o estado
Retorno: ninguno
*/
void modificarcliente(NODOCLIENTE *cabezaclientes)
{

    NODOCLIENTE *clienteencontrado;
    char idbuscado[6];
    char nuevovalor[100];
    char confirmacion;
    int opcion;

    imprimirtitulo("Modificar Cliente");
    printf("Ingrese ID del cliente (X=cancelar): ");
    fgets(idbuscado, sizeof(idbuscado), stdin);
    idbuscado[strcspn(idbuscado, "\n")] = '\0';

    if(esCancelar(idbuscado))
    {
        printf("Cancelado\n");
        return;
    }

    clienteencontrado = buscarcliente(cabezaclientes, idbuscado);
    if(clienteencontrado == NULL)
    {
        printf("No existe ese cliente\n");
        return;
    }

    // muestra los datos actuales antes de modificar
    printf("\nNombre actual  : %s\n", clienteencontrado->cliente.nombre);
    printf("Telefono actual: %s\n", clienteencontrado->cliente.telefono);

    if(clienteencontrado->cliente.activo == 1)
    {
        printf("Estado actual  : Activo\n");
    }
    else
    {
        printf("Estado actual  : Inactivo\n");
    }

    printf("\n1. Nombre  2. Telefono  3. Estado  0. Cancelar\n");
    printf("Seleccione una opci%cn: ", 162);
    scanf("%d", &opcion);
    limpiarbuffer();

    if(opcion == 0)
    {
        printf("Cancelado\n");
        return;
    }

    if(opcion == 1)
    {
        printf("Nuevo nombre (X=cancelar): ");
        fgets(nuevovalor, sizeof(nuevovalor), stdin);
        nuevovalor[strcspn(nuevovalor, "\n")] = '\0';

        if(esCancelar(nuevovalor))
        {
            printf("Cancelado\n");
            return;
        }

        if(!sololetras(nuevovalor))
        {
            printf("Solo letras, espacios y guiones\n");
            return;
        }

        printf("Desea guardar cambios? (S/N): ");
        scanf(" %c", &confirmacion);
        limpiarbuffer();

        if(confirmacion == 'S' || confirmacion == 's')
        {
            strcpy(clienteencontrado->cliente.nombre, nuevovalor);
            guardarclientes(cabezaclientes);
            printf("Cliente modificado correctamente\n");
        }
        else
        {
            printf("Cancelado\n");
        }
    }
    else if(opcion == 2)
    {
        printf("Nuevo telefono (X=cancelar): ");
        fgets(nuevovalor, sizeof(nuevovalor), stdin);
        nuevovalor[strcspn(nuevovalor, "\n")] = '\0';

        if(esCancelar(nuevovalor))
        {
            printf("Cancelado\n");
            return;
        }

        if(!validartelefono(nuevovalor))
        {
            printf("Solo numeros, espacios y guiones\n");
            return;
        }

        printf("Desea guardar cambios? (S/N): ");
        scanf(" %c", &confirmacion);
        limpiarbuffer();

        if(confirmacion == 'S' || confirmacion == 's')
        {
            strcpy(clienteencontrado->cliente.telefono, nuevovalor);
            guardarclientes(cabezaclientes);
            printf("Cliente modificado correctamente\n");
        }
        else
        {
            printf("Cancelado\n");
        }
    }
    else if(opcion == 3)
    {
        int nuevoestado;

        printf("Nuevo estado (1=Activo 0=Inactivo) (X=cancelar): ");
        char entradaEstado[10];
        fgets(entradaEstado, sizeof(entradaEstado), stdin);
        entradaEstado[strcspn(entradaEstado, "\n")] = '\0';

        if(esCancelar(entradaEstado))
        {
            printf("Cancelado\n");
            return;
        }

        nuevoestado = atoi(entradaEstado); //basicamente uso atoi para convertir el string ingresado a entero para compararlo

        if(nuevoestado != 0 && nuevoestado != 1)
        {
            printf("Estado invalido\n");
            return;
        }

        printf("Desea guardar cambios? (S/N): ");
        scanf(" %c", &confirmacion);
        limpiarbuffer();

        if(confirmacion == 'S' || confirmacion == 's')
        {
            clienteencontrado->cliente.activo = nuevoestado;
            guardarclientes(cabezaclientes);
            printf("Cliente modificado correctamente\n");
        }
        else
        {
            printf("Cancelado\n");
        }
    }
}

/*
Funcion: eliminarcliente
Argumentos: NODOCLIENTE **cabezaclientes, NODOENVIO *cabezaenvios
Objetivo: elimina fisicamente si no tiene envios, sino lo marca como inactivo
Retorno: ninguno
*/
void eliminarcliente(NODOCLIENTE **cabezaclientes, NODOENVIO *cabezaenvios)
{

    NODOCLIENTE *clienteencontrado;
    char idbuscado[6];
    char confirmacion;

    imprimirtitulo("Eliminar / Inactivar Cliente");
    printf("Ingrese ID del cliente (X=cancelar): ");
    fgets(idbuscado, sizeof(idbuscado), stdin);
    idbuscado[strcspn(idbuscado, "\n")] = '\0';

    if(esCancelar(idbuscado))
    {
        printf("Cancelado\n");
        return;
    }

    clienteencontrado = buscarcliente(*cabezaclientes, idbuscado);
    if(clienteencontrado == NULL)
    {
        printf("Cliente no encontrado\n");
        return;
    }

    if(clientetieneenvios(cabezaenvios, idbuscado))
    {
        // no se puede borrar si tiene envios, se ofrece inactivar
        printf("El cliente tiene ordenes asociadas\n");
        printf("No puede eliminarse\n");
        printf("Desea marcarlo como inactivo? (S/N): ");
        scanf(" %c", &confirmacion);
        limpiarbuffer();

        if(confirmacion == 'S' || confirmacion == 's')
        {
            clienteencontrado->cliente.activo = 0;
            guardarclientes(*cabezaclientes);
            printf("Cliente inactivado correctamente\n");
        }
        else
        {
            printf("Operacion cancelada\n");
        }
    }
    else
    {
        printf("Seguro que desea eliminar este cliente? (S/N): ");
        scanf(" %c", &confirmacion);
        limpiarbuffer();

        if(confirmacion == 'S' || confirmacion == 's')
        {
            borrarcliente(cabezaclientes, idbuscado);
            guardarclientes(*cabezaclientes);
            printf("Cliente eliminado\n");
        }
        else
        {
            printf("Cancelado\n");
        }
    }
}


/*
Funcion: crearpaquete
Argumentos: NODOPAQUETE **cabezapaquetes
Objetivo: pide los datos del paquete, los valida y lo inserta en la lista
Retorno: ninguno
*/
void crearpaquete(NODOPAQUETE **cabezapaquetes)
{

    PAQUETE nuevopaquete;
    char entrada[100];

    imprimirtitulo("Crear Paquete");

    do
    {
        printf("Ingrese ID (X=cancelar): ");
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0';

        if(esCancelar(entrada))
        {
            printf("Cancelado\n");
            pausar();
            return;
        }

        if(!idpaquetedisponible(*cabezapaquetes, entrada))
        {
            printf("ID invalido o ya existe\n");
        }
        else
        {
            break;
        }
    }
    while(1);
    strcpy(nuevopaquete.idpaquete, entrada);

    do
    {
        printf("Ingrese descripcion (X=cancelar): ");
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0';

        if(esCancelar(entrada))
        {
            printf("Cancelado\n");
            pausar();
            return;
        }

        if(!sololetras(entrada))
        {
            printf("Solo letras, espacios y guiones\n");
        }
        else
        {
            break;
        }
    }
    while(1);
    strcpy(nuevopaquete.descripcion, entrada);

    do
    {
        printf("Ingrese peso (0=cancelar): ");
        scanf("%f", &nuevopaquete.peso);
        limpiarbuffer();

        if(nuevopaquete.peso == 0)
        {
            printf("Cancelado\n");
            pausar();
            return;
        }

        if(!validarpeso(nuevopaquete.peso))
        {
            printf("El peso tiene que ser mayor a 0\n");
        }
        else
        {
            break;
        }
    }
    while(1);

    printf("Es fragil? (1=Si 0=No): ");
    scanf("%d", &nuevopaquete.fragil);
    limpiarbuffer();
    nuevopaquete.activo = 1;

    insertarpaquete(cabezapaquetes, nuevopaquete);
    guardarpaquetes(*cabezapaquetes);
    printf("\nPaquete registrado exitosamente\n");
    pausar();
}

/*
Funcion: listarpaquetes
Argumentos: NODOPAQUETE *cabezapaquetes
Objetivo: muestra todos los paquetes en formato de tabla
Retorno: ninguno
*/
void listarpaquetes(NODOPAQUETE *cabezapaquetes)
{

    NODOPAQUETE *indice;

    imprimirtitulo("Listar Paquetes");
    ordenarpaquetes(&cabezapaquetes);
    mostrarencpaquetes();

    if(cabezapaquetes == NULL)
    {
        printf("No hay paquetes registrados\n");
        return;
    }

    for(indice = cabezapaquetes; indice; indice = indice->sig)
    {
        mostrarfilapaquete(indice->paquete.idpaquete, indice->paquete.descripcion,
                           indice->paquete.peso, indice->paquete.fragil, indice->paquete.activo);
    }
}

/*
Funcion: modificarpaquete
Argumentos: NODOPAQUETE *cabezapaquetes
Objetivo: busca un paquete por id y permite editar descripcion, peso o estado
Retorno: ninguno
*/
void modificarpaquete(NODOPAQUETE *cabezapaquetes)
{

    NODOPAQUETE *paqueteencontrado;
    char idbuscado[6];
    char nuevovalor[100];
    char confirmacion;
    int opcion;

    imprimirtitulo("Modificar Paquete");
    printf("Ingrese ID del paquete (X=cancelar): ");
    fgets(idbuscado, sizeof(idbuscado), stdin);
    idbuscado[strcspn(idbuscado, "\n")] = '\0';

    if(esCancelar(idbuscado))
    {
        printf("Cancelado\n");
        return;
    }

    paqueteencontrado = buscarpaquete(cabezapaquetes, idbuscado);
    if(paqueteencontrado == NULL)
    {
        printf("Paquete no encontrado\n");
        return;
    }


    printf("\nDescripcion actual: %s\n", paqueteencontrado->paquete.descripcion);
    printf("Peso actual       : %.2f\n", paqueteencontrado->paquete.peso);

    if(paqueteencontrado->paquete.activo == 1)
    {
        printf("Estado actual     : Activo\n");
    }
    else
    {
        printf("Estado actual     : Inactivo\n");
    }

    printf("\n1. descripcion  2. peso  3. estado  0. cancelar\n");
    printf("seleccione una opci%cn: ", 162);
    scanf("%d", &opcion);
    limpiarbuffer();

    if(opcion == 0)
    {
        printf("cancelado\n");
        return;
    }

    if(opcion == 1)
    {
        printf("nueva descripcion (X=cancelar): ");
        fgets(nuevovalor, sizeof(nuevovalor), stdin);
        nuevovalor[strcspn(nuevovalor, "\n")] = '\0';

        if(esCancelar(nuevovalor))
        {
            printf("cancelado\n");
            return;
        }

        if(!sololetras(nuevovalor))
        {
            printf("solo letras, espacios y guiones\n");
            return;
        }

        printf("desea guardar cambios? (S/N): ");
        scanf(" %c", &confirmacion);
        limpiarbuffer();

        if(confirmacion == 'S' || confirmacion == 's')
        {
            strcpy(paqueteencontrado->paquete.descripcion, nuevovalor);
            guardarpaquetes(cabezapaquetes);
            printf("paquete modificado correctamente\n");
        }
        else
        {
            printf("cancelado\n");
        }
    }
    else if(opcion == 2)
    {
        float nuevopeso;

        printf("nnuevo peso (0=cancelar): ");
        scanf("%f", &nuevopeso);
        limpiarbuffer();

        if(nuevopeso == 0)
        {
            printf("cancelado\n");
            return;
        }

        if(!validarpeso(nuevopeso))
        {
            printf("peso invalido\n");
            return;
        }

        printf("desea guardar cambios? (S/N): ");
        scanf(" %c", &confirmacion);
        limpiarbuffer();

        if(confirmacion == 'S' || confirmacion == 's')
        {
            paqueteencontrado->paquete.peso = nuevopeso;
            guardarpaquetes(cabezapaquetes);
            printf("paquete modificado correctamente\n");
        }
        else
        {
            printf("cancelado\n");
        }
    }
    else if(opcion == 3)
    {
        int nuevoestado;

        printf("nuevo estado (1=Activo 0=Inactivo) (X=cancelar): ");
        char entradaEstado[10];
        fgets(entradaEstado, sizeof(entradaEstado), stdin);
        entradaEstado[strcspn(entradaEstado, "\n")] = '\0';

        if(esCancelar(entradaEstado))
        {
            printf("Cancelado\n");
            return;
        }

        nuevoestado = atoi(entradaEstado); //basicamente uso atoi para convertir el string ingresado a entero para compararlo

        if(nuevoestado != 0 && nuevoestado != 1)
        {
            printf("Estado invalido\n");
            return;
        }

        printf("Desea guardar cambios? (S/N): ");
        scanf(" %c", &confirmacion);
        limpiarbuffer();

        if(confirmacion == 'S' || confirmacion == 's')
        {
            paqueteencontrado->paquete.activo = nuevoestado;
            guardarpaquetes(cabezapaquetes);
            printf("Paquete modificado correctamente\n");
        }
        else
        {
            printf("Cancelado\n");
        }
    }
}

/*
Funcion: eliminarpaquete
Argumentos: NODOPAQUETE **cabezapaquetes, NODOENVIO *cabezaenvios
Objetivo: elimina fisicamente si no tiene envios, sino lo marca como inactivo
Retorno: ninguno
*/
void eliminarpaquete(NODOPAQUETE **cabezapaquetes, NODOENVIO *cabezaenvios)
{

    NODOPAQUETE *paqueteencontrado;
    char idbuscado[6];
    char confirmacion;

    imprimirtitulo("Eliminar / Inactivar Paquete");
    printf("Ingrese ID del paquete (X=cancelar): ");
    fgets(idbuscado, sizeof(idbuscado), stdin);
    idbuscado[strcspn(idbuscado, "\n")] = '\0';

    if(esCancelar(idbuscado))
    {
        printf("Cancelado\n");
        return;
    }

    paqueteencontrado = buscarpaquete(*cabezapaquetes, idbuscado);
    if(paqueteencontrado == NULL)
    {
        printf("Paquete no encontrado\n");
        return;
    }

    if(paquetetieneenvios(cabezaenvios, idbuscado))
    {
        printf("El paquete tiene envios asociados\n");
        printf("No puede eliminarse\n");
        printf("Desea inactivarlo? (S/N): ");
        scanf(" %c", &confirmacion);
        limpiarbuffer();

        if(confirmacion == 'S' || confirmacion == 's')
        {
            paqueteencontrado->paquete.activo = 0;
            guardarpaquetes(*cabezapaquetes);
            printf("Paquete inactivado correctamente\n");
        }
        else
        {
            printf("Cancelado\n");
        }
    }
    else
    {
        printf("Seguro que desea eliminar? (S/N): ");
        scanf(" %c", &confirmacion);
        limpiarbuffer();

        if(confirmacion == 'S' || confirmacion == 's')
        {
            borrarpaquete(cabezapaquetes, idbuscado);
            guardarpaquetes(*cabezapaquetes);
            printf("Paquete eliminado\n");
        }
        else
        {
            printf("Cancelado\n");
        }
    }
}


/*
Funcion: crearenvio
Argumentos: puntero a lista de envios, listas de clientes y paquetes, tarifa
Objetivo: registra un nuevo envio, valida cliente y paquete, calcula el costo
Retorno: ninguno
*/
void crearenvio(NODOENVIO **cabezaenvios, NODOCLIENTE *cabezaclientes, NODOPAQUETE *cabezapaquetes, TARIFA tarifa)
{

    ENVIO nuevoenvio;
    NODOCLIENTE *clienteencontrado;
    NODOPAQUETE *paqueteencontrado;
    char entrada[100];

    imprimirtitulo("Crear Envio");

    do
    {
        printf("Ingrese ID de envio (X=cancelar): ");
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0';

        if(esCancelar(entrada))
        {
            printf("Cancelado\n");
            pausar();
            return;
        }

        if(!idenviodisponible(*cabezaenvios, entrada))
        {
            printf("ID invalido o ya existe\n");
        }
        else
        {
            break;
        }
    }
    while(1);
    strcpy(nuevoenvio.idenvio, entrada);

    do
    {
        printf("Ingrese ID cliente (X=cancelar): ");
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0';

        if(esCancelar(entrada))
        {
            printf("Cancelado\n");
            pausar();
            return;
        }

        clienteencontrado = buscarcliente(cabezaclientes, entrada);

        if(clienteencontrado == NULL)
        {
            printf("Cliente no existe\n");
        }
        else if(clienteencontrado->cliente.activo == 0)
        {
            printf("El cliente esta inactivo\n");
        }
        else
        {
            break;
        }
    }
    while(1);
    strcpy(nuevoenvio.idcliente, entrada);

    do
    {
        printf("Ingrese ID paquete (X=cancelar): ");
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0';

        if(esCancelar(entrada))
        {
            printf("Cancelado\n");
            pausar();
            return;
        }

        paqueteencontrado = buscarpaquete(cabezapaquetes, entrada);

        if(paqueteencontrado == NULL)
        {
            printf("Paquete no existe\n");
        }
        else if(paqueteencontrado->paquete.activo == 0)
        {
            printf("El paquete esta inactivo\n");
        }
        else
        {
            break;
        }
    }
    while(1);
    strcpy(nuevoenvio.idpaquete, entrada);

    do
    {
        printf("Ingrese origen (X=cancelar): ");
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0';

        if(esCancelar(entrada))
        {
            printf("Cancelado\n");
            pausar();
            return;
        }

        if(!sololetras(entrada))
        {
            printf("Solo letras, espacios y guiones\n");
        }
        else
        {
            break;
        }
    }
    while(1);
    strcpy(nuevoenvio.origen, entrada);

    do
    {
        printf("Ingrese destino (X=cancelar): ");
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0';

        if(esCancelar(entrada))
        {
            printf("Cancelado\n");
            pausar();
            return;
        }

        if(!sololetras(entrada))
        {
            printf("Solo letras, espacios y guiones\n");
        }
        else
        {
            break;
        }
    }
    while(1);
    strcpy(nuevoenvio.destino, entrada);


    printf("Peso del paquete: %.2f\n", paqueteencontrado->paquete.peso);
    printf("Tarifa por libra: %.2f\n", tarifa.valorporlibra);
    nuevoenvio.costo = paqueteencontrado->paquete.peso * tarifa.valorporlibra;
    printf("Costo calculado : %.2f\n", nuevoenvio.costo);

    nuevoenvio.estado = 1;
    insertarenvio(cabezaenvios, nuevoenvio);
    guardarenvios(*cabezaenvios);
    printf("\nEnvio registrado exitosamente\n");
    printf("Estado: En transito\n");
    pausar();
}

/*
Funcion: listarenvios
Argumentos: NODOENVIO *cabezaenvios
Objetivo: muestra todos los envios en formato de tabla
Retorno: ninguno
*/
void listarenvios(NODOENVIO *cabezaenvios)
{

    NODOENVIO *indice;

    imprimirtitulo("Listar Envios");
    ordenarenvios(&cabezaenvios);
    mostrarencenvios();

    if(cabezaenvios == NULL)
    {
        printf("No hay envios registrados\n");
        return;
    }

    for(indice = cabezaenvios; indice; indice = indice->sig)
    {
        mostrarfilaenvio(indice->envio.idenvio, indice->envio.idcliente, indice->envio.idpaquete,
                         indice->envio.destino, indice->envio.costo, indice->envio.estado);
    }
}

/*
Funcion: verdetalleenvio
Argumentos: las tres listas
Objetivo: muestra todos los campos de un envio buscado por id
Retorno: ninguno
*/
void verdetalleenvio(NODOENVIO *cabezaenvios, NODOCLIENTE *cabezaclientes, NODOPAQUETE *cabezapaquetes)
{

    NODOENVIO *envioencontrado;
    NODOCLIENTE *clienteencontrado;
    NODOPAQUETE *paqueteencontrado;
    char idbuscado[6];

    imprimirtitulo("Detalle de Envio");
    printf("Ingrese ID de envio (X=cancelar): ");
    fgets(idbuscado, sizeof(idbuscado), stdin);
    idbuscado[strcspn(idbuscado, "\n")] = '\0';

    if(esCancelar(idbuscado))
    {
        printf("Cancelado\n");
        return;
    }

    envioencontrado = buscarenvio(cabezaenvios, idbuscado);
    if(envioencontrado == NULL)
    {
        printf("Envio no encontrado\n");
        return;
    }

    clienteencontrado = buscarcliente(cabezaclientes, envioencontrado->envio.idcliente);
    paqueteencontrado = buscarpaquete(cabezapaquetes, envioencontrado->envio.idpaquete);

    // si el cliente o paquete fue borrado de la lista se muestra el id directamente
    if(clienteencontrado != NULL && paqueteencontrado != NULL)
    {
        mostrardetalleenvio(envioencontrado->envio.idenvio,
                            clienteencontrado->cliente.nombre,
                            paqueteencontrado->paquete.descripcion,
                            envioencontrado->envio.origen,
                            envioencontrado->envio.destino,
                            envioencontrado->envio.costo,
                            envioencontrado->envio.estado);
    }
    else
    {
        mostrardetalleenvio(envioencontrado->envio.idenvio,
                            envioencontrado->envio.idcliente,
                            envioencontrado->envio.idpaquete,
                            envioencontrado->envio.origen,
                            envioencontrado->envio.destino,
                            envioencontrado->envio.costo,
                            envioencontrado->envio.estado);
    }
}

/*
Funcion: cambiarestado
Argumentos: NODOENVIO *cabezaenvios
Objetivo: cambia el estado de un envio, respetando las reglas del sistema
Retorno: ninguno
*/
void cambiarestado(NODOENVIO *cabezaenvios)
{

    NODOENVIO *envioencontrado;
    char idbuscado[6];
    int opcion;

    imprimirtitulo("Cambiar Estado de Envio");
    printf("Ingrese ID de envio (X=cancelar): ");
    fgets(idbuscado, sizeof(idbuscado), stdin);
    idbuscado[strcspn(idbuscado, "\n")] = '\0';

    if(esCancelar(idbuscado))
    {
        printf("Cancelado\n");
        return;
    }

    envioencontrado = buscarenvio(cabezaenvios, idbuscado);
    if(envioencontrado == NULL)
    {
        printf("Envio no encontrado\n");
        return;
    }

    // un envio entregado o cancelado no se puede cambiar
    if(envioencontrado->envio.estado == 2)
    {
        printf("Este envio ya fue entregado, no se puede cambiar\n");
        return;
    }

    if(envioencontrado->envio.estado == 0)
    {
        printf("Este envio ya esta cancelado, no se puede cambiar\n");
        return;
    }

    printf("Estado actual: En transito\n");
    printf("1. Marcar como entregado\n");
    printf("2. Cancelar envio\n");
    printf("0. Cancelar operacion\n");
    printf("Seleccione: ");
    scanf("%d", &opcion);
    limpiarbuffer();

    if(opcion == 0)
    {
        printf("Operacion cancelada\n");
        return;
    }
    else if(opcion == 1)
    {
        envioencontrado->envio.estado = 2;
        guardarenvios(cabezaenvios);
        printf("Envio marcado como entregado correctamente\n");
    }
    else if(opcion == 2)
    {
        envioencontrado->envio.estado = 0;
        guardarenvios(cabezaenvios);
        printf("Envio cancelado\n");
    }
    else
    {
        printf("Opcion invalida\n");
    }
}


/*
Funcion: repenviosdestino
Argumentos: las tres listas
Objetivo: filtra y muestra los envios que van a un destino especifico
Retorno: ninguno
*/
void repenviosdestino(NODOENVIO *cabezaenvios, NODOCLIENTE *cabezaclientes, NODOPAQUETE *cabezapaquetes)
{

    NODOENVIO *indice;
    NODOCLIENTE *clienteencontrado;
    NODOPAQUETE *paqueteencontrado;
    char destinobuscado[50];
    float totalingresados = 0;
    int totalenvios = 0;

    imprimirtitulo("Envios por Destino");
    printf("Ingrese destino (X=cancelar): ");
    fgets(destinobuscado, sizeof(destinobuscado), stdin);
    destinobuscado[strcspn(destinobuscado, "\n")] = '\0';

    if(esCancelar(destinobuscado))
    {
        printf("Cancelado\n");
        return;
    }

    printf("\n--- Env%cos hacia %s ---\n\n", 161, destinobuscado);
    printf("%-8s %-25s %-25s %-10s\n", "ID", "Cliente", "Paquete", "Costo");
    imprimirlinea(70);

    for(indice = cabezaenvios; indice; indice = indice->sig)
    {
        if(strcasecmp(indice->envio.destino, destinobuscado) == 0)
        {
            clienteencontrado = buscarcliente(cabezaclientes, indice->envio.idcliente);
            paqueteencontrado = buscarpaquete(cabezapaquetes, indice->envio.idpaquete);

            if(clienteencontrado != NULL && paqueteencontrado != NULL)
            {
                printf("%-8s %-25s %-25s %-10.2f\n",
                    indice->envio.idenvio,
                    clienteencontrado->cliente.nombre,
                    paqueteencontrado->paquete.descripcion,
                    indice->envio.costo);
            }
            else
            {
                printf("%-8s %-25s %-25s %-10.2f\n",
                    indice->envio.idenvio,
                    indice->envio.idcliente,
                    indice->envio.idpaquete,
                    indice->envio.costo);
            }

            totalingresados += indice->envio.costo;
            totalenvios++;
        }
    }

    if(totalenvios == 0)
    {
        printf("No hay envios a ese destino\n");
    }
    else
    {
        printf("\nTotal generado hacia este destino: %.2f\n", totalingresados);
    }
}

/*
Funcion: repclientemasenvios
Argumentos: NODOENVIO *cabezaenvios, NODOCLIENTE *cabezaclientes
Objetivo: cuenta los envios de cada cliente y muestra cual tiene mas
Retorno: ninguno
*/
void repclientemasenvios(NODOENVIO *cabezaenvios, NODOCLIENTE *cabezaclientes)
{

    NODOCLIENTE *indice;
    NODOENVIO *indiceenvio;
    int cantidadenvios;
    int maxcantidad = 0;
    char cliente_masenvios[50];

    imprimirtitulo("Clientes con mas Envios");
    printf("%-30s %-10s\n", "Cliente", "Cantidad");
    imprimirlinea(42);

    strcpy(cliente_masenvios, "ninguno");

    for(indice = cabezaclientes; indice; indice = indice->sig)
    {
        cantidadenvios = 0;

        for(indiceenvio = cabezaenvios; indiceenvio; indiceenvio = indiceenvio->sig)
        {
            if(strcmp(indiceenvio->envio.idcliente, indice->cliente.id) == 0)
            {
                cantidadenvios++;
            }
        }

        if(cantidadenvios > 0)
        {
            printf("%-30s %-10d\n", indice->cliente.nombre, cantidadenvios);

            if(cantidadenvios > maxcantidad)
            {
                maxcantidad = cantidadenvios;
                strcpy(cliente_masenvios, indice->cliente.nombre);
            }
        }
    }

    if(maxcantidad > 0)
    {
        printf("\nCliente con mas envios: %s (%d)\n", cliente_masenvios, maxcantidad);
    }
    else
    {
        printf("No hay envios registrados\n");
    }
}

/*
Funcion: reptotalingresos
Argumentos: NODOENVIO *cabezaenvios
Objetivo: suma el costo de todos los envios NO CANCELADOS (estado 1 o 2) y muestra el total
Retorno: ninguno
*/
void reptotalingresos(NODOENVIO *cabezaenvios)
{

    NODOENVIO *indice;
    float totalingresos = 0;
    int totalenvios = 0;

    imprimirtitulo("Total General Generado");

    for(indice = cabezaenvios; indice; indice = indice->sig)
    {
        // SOLO se cuentan los envios NO CANCELADOS (estado 1 o 2)
        if(indice->envio.estado != 0)
        {
            totalingresos += indice->envio.costo;
            totalenvios++;
        }
    }

    printf("Total de envios: %d\n", totalenvios);
    printf("Total ingresos  : RD$ %.2f\n", totalingresos);
}

/*
Funcion: repentregados
Argumentos: NODOENVIO *cabezaenvios
Objetivo: lista solo los envios con estado 2 (entregado)
Retorno: ninguno
*/
void repentregados(NODOENVIO *cabezaenvios)
{

    NODOENVIO *indice;
    int totalentregados = 0;

    imprimirtitulo("Envios Entregados");
    printf("%-8s %-20s %-10s\n", "ID", "Destino", "Costo");
    imprimirlinea(40);

    for(indice = cabezaenvios; indice; indice = indice->sig)
    {
        if(indice->envio.estado == 2)
        {
            printf("%-8s %-20s %-10.2f\n", indice->envio.idenvio, indice->envio.destino, indice->envio.costo);
            totalentregados++;
        }
    }

    printf("\nTotal entregados: %d\n", totalentregados);
}

/*
Funcion: repentransito
Argumentos: NODOENVIO *cabezaenvios
Objetivo: lista solo los envios con estado 1 (en transito)
Retorno: ninguno
*/
void repentransito(NODOENVIO *cabezaenvios)
{

    NODOENVIO *indice;
    int totalentransito = 0;

    imprimirtitulo("Envios en Transito");
    printf("%-8s %-20s %-10s\n", "ID", "Destino", "Costo");
    imprimirlinea(40);

    for(indice = cabezaenvios; indice; indice = indice->sig)
    {
        if(indice->envio.estado == 1)
        {
            printf("%-8s %-20s %-10.2f\n", indice->envio.idenvio, indice->envio.destino, indice->envio.costo);
            totalentransito++;
        }
    }

    printf("\nTotal en transito: %d\n", totalentransito);
}

/*
Funcion: repcancelados
Argumentos: NODOENVIO *cabezaenvios, NODOCLIENTE *cabezaclientes
Objetivo: lista solo los envios con estado 0 (cancelado) junto al nombre del cliente
Retorno: ninguno
*/
void repcancelados(NODOENVIO *cabezaenvios, NODOCLIENTE *cabezaclientes)
{

    NODOENVIO *indice;
    NODOCLIENTE *clienteencontrado;
    int totalcancelados = 0;

    imprimirtitulo("Envios Cancelados");
    printf("%-8s %-20s %-25s\n", "ID", "Destino", "Cliente");
    imprimirlinea(55);

    for(indice = cabezaenvios; indice; indice = indice->sig)
    {
        if(indice->envio.estado == 0)
        {
            clienteencontrado = buscarcliente(cabezaclientes, indice->envio.idcliente);

            if(clienteencontrado != NULL)
            {
                printf("%-8s %-20s %-25s\n",
                    indice->envio.idenvio,
                    indice->envio.destino,
                    clienteencontrado->cliente.nombre);
            }
            else
            {
                printf("%-8s %-20s %-25s\n",
                    indice->envio.idenvio,
                    indice->envio.destino,
                    indice->envio.idcliente);
            }

            totalcancelados++;
        }
    }

    printf("\nTotal cancelados: %d\n", totalcancelados);
}

/*
Funcion: ordenarclientes
Argumentos: NODOCLIENTE **cabeza
Objetivo: ordena la lista de clientes por ID usando bubble sort,
          intercambia el contenido de los nodos sin mover los punteros
Retorno: ninguno
*/
void ordenarclientes(NODOCLIENTE **cabeza)
{

    NODOCLIENTE *indice;
    NODOCLIENTE *ultimo;
    CLIENTE temp;
    int intercambio;

    if(*cabeza == NULL)
    {
        return;
    }

    ultimo = NULL;

    do
    {
        intercambio = 0;

        for(indice = *cabeza; indice->sig != ultimo; indice = indice->sig)
        {
            if(strcmp(indice->cliente.id, indice->sig->cliente.id) > 0)
            {
                temp = indice->cliente;
                indice->cliente = indice->sig->cliente;
                indice->sig->cliente = temp;
                intercambio = 1;
            }
        }

        ultimo = indice;
    }
    while(intercambio == 1);
}

/*
Funcion: ordenarpaquetes
Argumentos: NODOPAQUETE **cabeza
Objetivo: ordena la lista de paquetes por ID usando bubble sort
Retorno: ninguno
*/
void ordenarpaquetes(NODOPAQUETE **cabeza)
{

    NODOPAQUETE *indice;
    NODOPAQUETE *ultimo;
    PAQUETE temp;
    int intercambio;

    if(*cabeza == NULL)
    {
        return;
    }

    ultimo = NULL;

    do
    {
        intercambio = 0;

        for(indice = *cabeza; indice->sig != ultimo; indice = indice->sig)
        {
            if(strcmp(indice->paquete.idpaquete, indice->sig->paquete.idpaquete) > 0)
            {
                temp = indice->paquete;
                indice->paquete = indice->sig->paquete;
                indice->sig->paquete = temp;
                intercambio = 1;
            }
        }

        ultimo = indice;
    }
    while(intercambio == 1);
}

/*
Funcion: ordenarenvios
Argumentos: NODOENVIO **cabeza
Objetivo: ordena la lista de envios por ID usando bubble sort
Retorno: ninguno
*/
void ordenarenvios(NODOENVIO **cabeza)
{

    NODOENVIO *indice;
    NODOENVIO *ultimo;
    ENVIO temp;
    int intercambio;

    if(*cabeza == NULL)
    {
        return;
    }

    ultimo = NULL;

    do
    {
        intercambio = 0;

        for(indice = *cabeza; indice->sig != ultimo; indice = indice->sig)
        {
            if(strcmp(indice->envio.idenvio, indice->sig->envio.idenvio) > 0)
            {
                temp = indice->envio;
                indice->envio = indice->sig->envio;
                indice->sig->envio = temp;
                intercambio = 1;
            }
        }

        ultimo = indice;
    }
    while(intercambio == 1);
}
