#ifndef PRODUCTOS_H
#define PRODUCTOS_H

#define MAX_PRODUCTOS 500

typedef struct {
    char codigo[20];
    char nombre[100];
    int cantidad;
    float costo;
    float precio_venta;
} Producto;

/* Carga productos.txt en el arreglo. Retorna cantidad de productos leidos. */
int cargar_productos(const char *archivo, Producto productos[], int max);

/* Busca un producto por codigo. Retorna el indice, o -1 si no existe. */
int buscar_producto(Producto productos[], int total, const char *codigo);

/* Reescribe productos.txt completo con el arreglo actualizado (tras una venta). */
int guardar_productos(const char *archivo, Producto productos[], int total);

#endif
