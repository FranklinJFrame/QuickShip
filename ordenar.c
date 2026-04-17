#include <stdio.h>
#include <string.h>
#include "ordenar.h"

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
    while(intercambio);
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
    while(intercambio);
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
    while(intercambio);
}
