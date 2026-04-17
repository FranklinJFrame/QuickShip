#include <stdio.h>
#include "formatos.h"
#include "atajos.h"

/*
Funcion: mostrarencclientes
Argumentos: ninguno
Objetivo: imprime el encabezado de la tabla de clientes
Retorno: ninguno
*/
void mostrarencclientes()
{
    printf("\n%-8s %-28s %-18s %-10s\n", "ID", "Nombre", "Telefono", "Estado");
    imprimirlinea(66);
}

/*
Funcion: mostrarfilacliente
Argumentos: id, nombre, telefono, activo
Objetivo: imprime una fila de la tabla con los datos de un cliente
Retorno: ninguno
*/
void mostrarfilacliente(char *id, char *nombre, char *telefono, int activo)
{
    printf("%-8s %-28s %-18s %-10s\n", id, nombre, telefono, activo == 1 ? "Activo" : "Inactivo");
}

/*
Funcion: mostrarencpaquetes
Argumentos: ninguno
Objetivo: imprime el encabezado de la tabla de paquetes
Retorno: ninguno
*/
void mostrarencpaquetes()
{
    printf("\n%-8s %-28s %-8s %-8s %-10s\n", "ID", "Descripcion", "Peso", "Fragil", "Estado");
    imprimirlinea(64);
}

/*
Funcion: mostrarfilapaquete
Argumentos: id, descripcion, peso, fragil, activo
Objetivo: imprime una fila de la tabla con los datos de un paquete
Retorno: ninguno
*/
void mostrarfilapaquete(char *id, char *descripcion, float peso, int fragil, int activo)
{
    printf("%-8s %-28s %-8.2f %-8s %-10s\n", id, descripcion, peso,
           fragil == 1 ? "Si" : "No", activo == 1 ? "Activo" : "Inactivo");
}

/*
Funcion: mostrarencenvios
Argumentos: ninguno
Objetivo: imprime el encabezado de la tabla de envios
Retorno: ninguno
*/
void mostrarencenvios()
{
    printf("\n%-8s %-8s %-8s %-18s %-10s %-12s\n", "ID", "Cliente", "Paquete", "Destino", "Costo", "Estado");
    imprimirlinea(66);
}

/*
Funcion: mostrarfilaenvio
Argumentos: ids, destino, costo, estado
Objetivo: imprime una fila de la tabla con los datos de un envio
Retorno: ninguno
*/
void mostrarfilaenvio(char *idenvio, char *idcliente, char *idpaquete, char *destino, float costo, int estado)
{

    char *estadotexto;

    if(estado == 1)
        estadotexto = "En transito";
    else if
        (estado == 2) estadotexto = "Entregado";
    else
        estadotexto = "Cancelado";

    printf("%-8s %-8s %-8s %-18s %-10.2f %-12s\n", idenvio, idcliente, idpaquete, destino, costo, estadotexto);
}

/*
Funcion: mostrardetalleenvio
Argumentos: todos los campos del envio con nombre de cliente y paquete
Objetivo: muestra el detalle completo de un envio en formato vertical
Retorno: ninguno
*/
void mostrardetalleenvio(char *idenvio, char *nombrecliente, char *nombrepaquete, char *origen, char *destino, float costo, int estado)
{

    char *estadotexto;

    if(estado == 1)
        estadotexto = "En transito";
    else if
        (estado == 2) estadotexto = "Entregado";
    else
        estadotexto = "Cancelado";

    printf("\nID Envio  : %s\n", idenvio);
    printf("Cliente   : %s\n", nombrecliente);
    printf("Paquete   : %s\n", nombrepaquete);
    printf("Origen    : %s\n", origen);
    printf("Destino   : %s\n", destino);
    printf("Costo     : %.2f\n", costo);
    printf("Estado    : %s\n", estadotexto);
}
