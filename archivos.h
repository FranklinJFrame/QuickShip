#ifndef ARCHIVOS_H
#define ARCHIVOS_H


void guardarclientes(NODOCLIENTE *cabeza);
void cargarclientes(NODOCLIENTE **cabeza);

void guardarpaquetes(NODOPAQUETE *cabeza);
void cargarpaquetes(NODOPAQUETE **cabeza);

void guardarenvios(NODOENVIO *cabeza);
void cargarenvios(NODOENVIO **cabeza);

void guardartarifa(TARIFA tarifa);
int  cargartarifa(TARIFA *tarifa);

#endif
