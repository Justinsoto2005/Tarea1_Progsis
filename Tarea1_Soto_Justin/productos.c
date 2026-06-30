#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "productos.h"

int cargar_productos(const char *archivo, Producto productos[], int max) {
    FILE *fp = fopen(archivo, "r");
    if (fp == NULL) {
        printf("Error: no se pudo abrir %s\n", archivo);
        return 0;
    }

    int total = 0;
    char linea[300];

    while (fgets(linea, sizeof(linea), fp) != NULL && total < max) {
        linea[strcspn(linea, "\n")] = '\0';
        if (strlen(linea) == 0) continue;

        /* Formato: Codigo_producto,Nombre_producto,cantidad,costo,precio_venta */
        char *token = strtok(linea, ",");
        if (token == NULL) continue;
        strncpy(productos[total].codigo, token, sizeof(productos[total].codigo) - 1);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strncpy(productos[total].nombre, token, sizeof(productos[total].nombre) - 1);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        productos[total].cantidad = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        productos[total].costo = atof(token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        productos[total].precio_venta = atof(token);

        total++;
    }

    fclose(fp);
    return total;
}

int buscar_producto(Producto productos[], int total, const char *codigo) {
    for (int i = 0; i < total; i++) {
        if (strcmp(productos[i].codigo, codigo) == 0) {
            return i;
        }
    }
    return -1;
}

int guardar_productos(const char *archivo, Producto productos[], int total) {
    FILE *fp = fopen(archivo, "w");
    if (fp == NULL) {
        printf("Error: no se pudo abrir %s para escritura\n", archivo);
        return 0;
    }

    for (int i = 0; i < total; i++) {
        fprintf(fp, "%s,%s,%d,%.2f,%.2f\n",
                productos[i].codigo,
                productos[i].nombre,
                productos[i].cantidad,
                productos[i].costo,
                productos[i].precio_venta);
    }

    fclose(fp);
    return 1;
}
