#ifndef USUARIOS_H
#define USUARIOS_H

#define MAX_USUARIOS 100

typedef struct {
    char usuario[50];
    char clave[50];
    char nombre[100];
} Usuario;

/* Carga el archivo vendedores.txt en el arreglo 'usuarios'.
   Retorna la cantidad de usuarios leidos. */
int cargar_usuarios(const char *archivo, Usuario usuarios[], int max);

/* Busca usuario+clave en el arreglo. Retorna el indice si lo encuentra,
   -1 si no existe o la clave no coincide. */
int validar_login(Usuario usuarios[], int total, const char *user, const char *clave);

#endif
