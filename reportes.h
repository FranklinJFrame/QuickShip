#ifndef REPORTES_H
#define REPORTES_H


void repenviosdestino(NODOENVIO *cabeza, NODOCLIENTE *cabezaclientes, NODOPAQUETE *cabezapaquetes);
void repclientemasenvios(NODOENVIO *cabeza, NODOCLIENTE *cabezaclientes);
void reptotalingresos(NODOENVIO *cabeza);
void repentregados(NODOENVIO *cabeza);
void repentransito(NODOENVIO *cabeza);
void repcancelados(NODOENVIO *cabeza, NODOCLIENTE *cabezaclientes);

#endif
