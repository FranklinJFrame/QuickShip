#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "atajos.h"

/*
Funcion: limpiarpantalla
Argumentos: ninguno
Objetivo: limpia la consola
Retorno: ninguno
*/
void limpiarpantalla()
{
    system("cls");
}

/*
Funcion: pausar
Argumentos: ninguno
Objetivo: espera que el usuario presione enter para continuar
Retorno: ninguno
*/
void pausar()
{
    printf("\nPresione Enter para continuar...");
    getchar();
}

/*
Funcion: limpiarbuffer
Argumentos: ninguno
Objetivo: limpia lo que queda en el buffer despues de un scanf
Retorno: ninguno
*/
void limpiarbuffer()
{
    int caracter;
    while((caracter = getchar()) != '\n' && caracter != EOF);
}

/*
Funcion: imprimirlinea
Argumentos: int n
Objetivo: imprime una linea de guiones de longitud n
Retorno: ninguno
*/
void imprimirlinea(int n)
{
    int indice;
    for(indice = 0; indice < n; indice++)
        printf("-");
    printf("\n");
}

/*
Funcion: imprimirtitulo
Argumentos: char *titulo
Objetivo: imprime el titulo de cada seccion
Retorno: ninguno
*/
void imprimirtitulo(char *titulo)
{
    printf("\n--- %s ---\n", titulo);
}
