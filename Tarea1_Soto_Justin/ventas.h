#ifndef VENTAS_H
#define VENTAS_H

#define MAX_DETALLE 50

typedef struct {
    char codigo_producto[20];
    char nombre_producto[100];
    int cantidad;
    float costo;
    float precio_venta;
} DetalleVenta;

/* Lee el ultimo numero de factura registrado en ventas.txt y retorna ese
   numero + 1. Si el archivo no existe o esta vacio, retorna 1. */
int obtener_siguiente_factura(const char *archivo);

/* Agrega al final de ventas.txt una linea por cada producto vendido en
   la factura indicada. */
int registrar_venta(const char *archivo, int num_factura, DetalleVenta detalle[],
                     int total_items, const char *fecha);

#endif
