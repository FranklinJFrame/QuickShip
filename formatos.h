#ifndef FORMATOS_H
#define FORMATOS_H

void mostrarencclientes();
void mostrarfilacliente(char *id, char *nombre, char *telefono, int activo);

void mostrarencpaquetes();
void mostrarfilapaquete(char *id, char *descripcion, float peso, int fragil, int activo);

void mostrarencenvios();
void mostrarfilaenvio(char *idenvio, char *idcliente, char *idpaquete, char *destino, float costo, int estado);
void mostrardetalleenvio(char *idenvio, char *nombrecliente, char *nombrepaquete, char *origen, char *destino, float costo, int estado);

#endif
