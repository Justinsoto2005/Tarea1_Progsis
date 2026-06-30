#include <stdio.h>
#include <string.h>
#include "usuarios.h"

int cargar_usuarios(const char *archivo, Usuario usuarios[], int max) {
    FILE *fp = fopen(archivo, "r");
    if (fp == NULL) {
        printf("Error: no se pudo abrir %s\n", archivo);
        return 0;
    }

    int total = 0;
    char linea[300];

    while (fgets(linea, sizeof(linea), fp) != NULL && total < max) {
        /* Quitar el salto de linea final, si existe */
        linea[strcspn(linea, "\n")] = '\0';

        /* Formato: Usuario,clave,Nombre */
        char *token = strtok(linea, ",");
        if (token == NULL) continue;
        strncpy(usuarios[total].usuario, token, sizeof(usuarios[total].usuario) - 1);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strncpy(usuarios[total].clave, token, sizeof(usuarios[total].clave) - 1);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strncpy(usuarios[total].nombre, token, sizeof(usuarios[total].nombre) - 1);

        total++;
    }

    fclose(fp);
    return total;
}

int validar_login(Usuario usuarios[], int total, const char *user, const char *clave) {
    for (int i = 0; i < total; i++) {
        if (strcmp(usuarios[i].usuario, user) == 0 &&
            strcmp(usuarios[i].clave, clave) == 0) {
            return i;
        }
    }
    return -1;
}
