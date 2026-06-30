#include <stdio.h>
#include <string.h>
#include <time.h>
#include "usuarios.h"
#include "productos.h"
#include "ventas.h"

#define ARCHIVO_USUARIOS  "vendedores.txt"
#define ARCHIVO_PRODUCTOS "productos.txt"
#define ARCHIVO_VENTAS    "ventas.txt"

/* Devuelve la fecha actual en formato aaaa-mm-dd */
void obtener_fecha_actual(char *buffer, int tam) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buffer, tam, "%Y-%m-%d", tm_info);
}

int main(void) {
    Usuario usuarios[MAX_USUARIOS];
    int total_usuarios = cargar_usuarios(ARCHIVO_USUARIOS, usuarios, MAX_USUARIOS);

    if (total_usuarios == 0) {
        printf("No se pudieron cargar los usuarios. Saliendo.\n");
        return 1;
    }

    /* ---------- LOGIN (maximo 3 intentos) ---------- */
    int sesion_idx = -1;
    int intentos = 0;
    char user[50], clave[50];

    while (intentos < 3) {
        printf("Usuario: ");
        scanf("%49s", user);
        printf("Clave: ");
        scanf("%49s", clave);

        sesion_idx = validar_login(usuarios, total_usuarios, user, clave);

        if (sesion_idx != -1) {
            break;
        }

        intentos++;
        printf("Usuario o clave incorrectos. Intento %d de 3.\n", intentos);
    }

    if (sesion_idx == -1) {
        printf("Numero maximo de intentos alcanzado. Cerrando el sistema.\n");
        return 1;
    }

    printf("\nBienvenido, %s!\n\n", usuarios[sesion_idx].nombre);

    /* ---------- CARGAR PRODUCTOS ---------- */
    Producto productos[MAX_PRODUCTOS];
    int total_productos = cargar_productos(ARCHIVO_PRODUCTOS, productos, MAX_PRODUCTOS);

    if (total_productos == 0) {
        printf("No se pudieron cargar los productos. Saliendo.\n");
        return 1;
    }

    /* ---------- REGISTRO DE VENTAS (loop) ---------- */
    DetalleVenta detalle[MAX_DETALLE];
    int total_items = 0;
    char respuesta;

    do {
        char codigo[20];
        printf("Codigo del producto: ");
        scanf("%19s", codigo);

        int idx = buscar_producto(productos, total_productos, codigo);
        if (idx == -1) {
            printf("Producto no encontrado. Intente de nuevo.\n");
            continue;
        }

        int cantidad;
        printf("Cantidad a vender (disponible: %d): ", productos[idx].cantidad);
        scanf("%d", &cantidad);

        if (cantidad <= 0) {
            printf("Cantidad invalida.\n");
            continue;
        }

        if (cantidad > productos[idx].cantidad) {
            printf("Error: no hay suficiente inventario. Disponible: %d\n",
                   productos[idx].cantidad);
            continue;
        }

        if (total_items >= MAX_DETALLE) {
            printf("Se alcanzo el limite de items por factura.\n");
            break;
        }

        /* Guardamos el item en el detalle de la venta */
        strcpy(detalle[total_items].codigo_producto, productos[idx].codigo);
        strcpy(detalle[total_items].nombre_producto, productos[idx].nombre);
        detalle[total_items].cantidad = cantidad;
        detalle[total_items].costo = productos[idx].costo;
        detalle[total_items].precio_venta = productos[idx].precio_venta;
        total_items++;

        /* Reservamos el inventario en memoria (se confirma al final) */
        productos[idx].cantidad -= cantidad;

        printf("Producto agregado a la venta.\n");
        printf("¿Desea agregar otro producto? (s/n): ");
        scanf(" %c", &respuesta);

    } while ((respuesta == 's' || respuesta == 'S') && total_items < MAX_DETALLE);

    if (total_items == 0) {
        printf("No se registraron productos. Finalizando sin generar venta.\n");
        return 0;
    }

    /* ---------- MOSTRAR FACTURA ---------- */
    int num_factura = obtener_siguiente_factura(ARCHIVO_VENTAS);
    char fecha[15];
    obtener_fecha_actual(fecha, sizeof(fecha));

    printf("\n========== FACTURA ==========\n");
    printf("Numero de factura: %d\n", num_factura);
    printf("Fecha: %s\n", fecha);
    printf("Vendedor: %s\n", usuarios[sesion_idx].nombre);
    printf("------------------------------\n");
    printf("%-10s %-20s %5s %10s %10s\n", "Codigo", "Producto", "Cant", "P.Unit", "Total");

    float total_factura = 0.0f;
    for (int i = 0; i < total_items; i++) {
        float subtotal = detalle[i].cantidad * detalle[i].precio_venta;
        total_factura += subtotal;
        printf("%-10s %-20s %5d %10.2f %10.2f\n",
               detalle[i].codigo_producto,
               detalle[i].nombre_producto,
               detalle[i].cantidad,
               detalle[i].precio_venta,
               subtotal);
    }

    printf("------------------------------\n");
    printf("TOTAL: %.2f\n", total_factura);
    printf("==============================\n\n");

    /* ---------- CONFIRMACION FINAL ---------- */
    printf("¿Confirma la venta? (s/n): ");
    scanf(" %c", &respuesta);

    if (respuesta == 's' || respuesta == 'S') {
        guardar_productos(ARCHIVO_PRODUCTOS, productos, total_productos);
        registrar_venta(ARCHIVO_VENTAS, num_factura, detalle, total_items, fecha);
        printf("Venta registrada con exito. Factura #%d\n", num_factura);
    } else {
        printf("Venta cancelada. No se modifico el inventario.\n");
    }

    return 0;
}
