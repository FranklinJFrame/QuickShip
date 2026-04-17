#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "validaciones.h"

/*
Funcion: sololetras
Argumentos: char *texto
Objetivo: verifica que el texto solo tenga letras, espacios y guiones
Retorno: int - 1 si es valido, 0 si no
*/
int sololetras(char *texto)
{

    int indice;

    if(strlen(texto) == 0) return 0;

    for(indice = 0; texto[indice]; indice++)
    {
        if(!isalpha((char)texto[indice]) && texto[indice] != ' ' && texto[indice] != '-')
            return 0;
    }
    return 1;
}

/*
Funcion: solonumeros
Argumentos: char *texto
Objetivo: verifica que el texto solo tenga numeros, espacios y guiones
Retorno: int - 1 si es valido, 0 si no
*/
int solonumeros(char *texto)
{

    int indice;

    if(strlen(texto) == 0) return 0;

    for(indice = 0; texto[indice]; indice++)
    {
        if(!isdigit((char)texto[indice]) && texto[indice] != ' ' && texto[indice] != '-')
            return 0;
    }
    return 1;
}

/*
Funcion: validartelefono
Argumentos: char *telefono
Objetivo: verifica que el telefono no este vacio y solo tenga numeros y guiones
Retorno: int - 1 si es valido, 0 si no
*/
int validartelefono(char *telefono)
{

    if(strlen(telefono) == 0) return 0;
    return solonumeros(telefono);
}

/*
Funcion: validarpeso
Argumentos: float peso
Objetivo: el peso tiene que ser mayor a cero
Retorno: int - 1 si es valido, 0 si no
*/
int validarpeso(float peso)
{
    return peso > 0;
}

/*
Funcion: validarcosto
Argumentos: float costo
Objetivo: el costo tiene que ser mayor a cero
Retorno: int - 1 si es valido, 0 si no
*/
int validarcosto(float costo)
{
    return costo > 0;
}

/*
Funcion: esCancelar
Argumentos: char *entrada
Objetivo: verifica si la entrada es 'X' o 'x' para cancelar la operacion
Retorno: int - 1 si es cancelar, 0 si no
*/
int esCancelar(char *entrada)
{
    return (*entrada == 'X' || *entrada == 'x');
}
