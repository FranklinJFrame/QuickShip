# 📦 QuickShip — Sistema de Gestión de Envíos Courier

Sistema de gestión de envíos tipo Courier desarrollado en C, utilizando listas doblemente enlazadas como estructura principal de datos. Permite administrar clientes, paquetes y envíos con persistencia en archivos binarios.

---

## 🚀 Características

- CRUD completo de **clientes**, **paquetes** y **envíos**
- Listas **doblemente enlazadas** para almacenamiento en memoria
- Persistencia de datos mediante **archivos binarios** (`.dat`)
- **6 reportes** operativos del sistema
- Validaciones de entrada y relaciones entre entidades
- Ordenamiento por ID en todos los listados
- Tarifa configurable por libra

---

## 🗂️ Estructura del proyecto

```
QuickShip/
├── main.c           # Lógica principal, menús, CRUD y reportes
├── atajos.h         # Funciones auxiliares (limpiar pantalla, pausar, etc.)
├── validaciones.h   # Validaciones de entrada (letras, teléfono, peso)
├── formatos.h       # Funciones de visualización y formato de tablas
├── archivos.h       # Prototipo de funciones de archivos binarios
├── reportes.h       # Prototipo de funciones de reportes
├── clientes.dat     # Datos persistentes de clientes (generado en ejecución)
├── paquetes.dat     # Datos persistentes de paquetes (generado en ejecución)
├── envios.dat       # Datos persistentes de envíos (generado en ejecución)
└── tarifa.dat       # Tarifa actual por libra (generado en ejecución)
```

---

## 🏗️ Estructuras principales

```c
typedef struct {
    char id[6];
    char nombre[50];
    char telefono[20];
    int activo;         // 1 = activo, 0 = inactivo
} CLIENTE;

typedef struct {
    char idpaquete[6];
    char descripcion[100];
    float peso;
    int fragil;         // 1 = sí, 0 = no
    int activo;         // 1 = activo, 0 = inactivo
} PAQUETE;

typedef struct {
    char idenvio[6];
    char idcliente[6];
    char idpaquete[6];
    char origen[50];
    char destino[50];
    float costo;
    int estado;         // 1 = en tránsito, 2 = entregado, 0 = cancelado
} ENVIO;
```

---

## ⚙️ Compilación y ejecución

### Requisitos
- Compilador GCC (o compatible con C99)
- Sistema operativo: Windows / Linux / macOS

### Compilar
```bash
gcc main.c -o quickship
```

### Ejecutar
```bash
./quickship
```
> En Windows: `quickship.exe`

---

## 🖥️ Menú principal

```
--- MENÚ PRINCIPAL ---
1. Clientes
2. Paquetes
3. Envíos
4. Reportes
5. Configurar tarifa
0. Salir
```

---

## ✅ Validaciones implementadas

| Campo | Regla |
|---|---|
| ID | Único, máximo 5 caracteres, no vacío |
| Nombre / Descripción / Origen / Destino | Solo letras, espacios y guiones |
| Teléfono | Solo números, espacios y guiones |
| Peso | Mayor a 0 |
| Costo | Calculado automáticamente: `peso × tarifa` |

---

## 📋 Reglas del sistema

- Un **cliente** o **paquete** con envíos asociados **no puede eliminarse físicamente** → se marca como inactivo
- Un **envío** no se elimina → solo puede cambiar su estado a cancelado
- No se puede crear un envío con un cliente o paquete **inactivo**
- Un envío **entregado** no puede cancelarse, y uno **cancelado** no puede marcarse como entregado

---

## 📊 Reportes disponibles

1. Envíos filtrados por destino
2. Cliente con más envíos
3. Total de ingresos generados (excluye cancelados)
4. Envíos entregados
5. Envíos en tránsito
6. Envíos cancelados

---

## 💾 Persistencia

Los datos se cargan automáticamente al iniciar el programa y se guardan al salir:

| Archivo | Contenido |
|---|---|
| `clientes.dat` | Lista de clientes |
| `paquetes.dat` | Lista de paquetes |
| `envios.dat` | Lista de envíos |
| `tarifa.dat` | Tarifa configurada por libra |

---

## 🎓 Información académica

Proyecto final de la asignatura **Fundamentos de Programación (ICC-103-TP)**  
Pontificia Universidad Católica Madre y Maestra (PUCMM)  
Ciclo 1910 — Enero 2026  
Profesora: Damarys A. Germosén M.
