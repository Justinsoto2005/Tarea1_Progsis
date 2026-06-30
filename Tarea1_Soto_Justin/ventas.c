#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ventas.h"

int obtener_siguiente_factura(const char *archivo) {
    FILE *fp = fopen(archivo, "r");
    if (fp == NULL) {
        /* No existe el archivo todavia: empezamos en la factura 1 */
        return 1;
    }

    char linea[300];
    int ultimo = 0;

    while (fgets(linea, sizeof(linea), fp) != NULL) {
        linea[strcspn(linea, "\n")] = '\0';
        if (strlen(linea) == 0) continue;

        char copia[300];
        strncpy(copia, linea, sizeof(copia) - 1);
        copia[sizeof(copia) - 1] = '\0';

        char *token = strtok(copia, ",");
        if (token != NULL) {
            int num = atoi(token);
            if (num > ultimo) ultimo = num;
        }
    }

    fclose(fp);
    return ultimo + 1;
}

int registrar_venta(const char *archivo, int num_factura, DetalleVenta detalle[],
                     int total_items, const char *fecha) {
    FILE *fp = fopen(archivo, "a");
    if (fp == NULL) {
        printf("Error: no se pudo abrir %s para escritura\n", archivo);
        return 0;
    }

    for (int i = 0; i < total_items; i++) {
        fprintf(fp, "%d,%s,%s,%d,%.2f,%.2f,%s\n",
                num_factura,
                detalle[i].codigo_producto,
                detalle[i].nombre_producto,
                detalle[i].cantidad,
                detalle[i].costo,
                detalle[i].precio_venta,
                fecha);
    }

    fclose(fp);
    return 1;
}
