#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include "biblioteca.h"

#define PUERTO_SERVIDOR 8080
#define ARCHIVO_CIFRADO "archivos/archivo_cifrado.txt"
#define ARCHIVO_DESCIFRADO "archivos/archivo_descifrado.txt"
#define BUFFER_SIZE 1024*2*2*2

void guardar_archivo(int socket_cliente) {
    FILE *archivo = fopen(ARCHIVO_CIFRADO, "w");
    if (!archivo) {
        perror("No se pudo crear archivo de salida");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes;

    while ((bytes = recv(socket_cliente, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes, archivo);
    }

    fclose(archivo);
    printf("Archivo recibido correctamente.\n");
}

void ejecutar_mpi() {
    printf("Ejecutando procesamiento distribuido con MPI...\n");
    int status = system("mpirun -np 3 ./nodo");
    if (status != 0) {
        fprintf(stderr, "Error ejecutando nodos MPI.\n");
        exit(EXIT_FAILURE);
    }
}

void escribir_resultado() {
    FILE *f = fopen(ARCHIVO_DESCIFRADO, "r");
    if (!f) {
        perror("No se pudo abrir el archivo descifrado");
        return;
    }

    char palabra[100];
    int cantidad;

    fscanf(f, "%s %d", palabra, &cantidad);
    fclose(f);

    // Simulación de biblioteca de control de hardware
    printf("Palabra más repetida: %s (%d veces)\n", palabra, cantidad);
    // llamar_biblioteca_escribir(palabra, cantidad); ← función real a implementar
}

int main() {
    int sockfd, socket_cliente;
    struct sockaddr_in servidor, cliente;
    socklen_t cliente_len = sizeof(cliente);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creando socket");
        exit(EXIT_FAILURE);
    }

    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(PUERTO_SERVIDOR);
    servidor.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&servidor, sizeof(servidor)) < 0) {
        perror("Error en bind");
        exit(EXIT_FAILURE);
    }

    listen(sockfd, 1);
    printf("Servidor esperando conexión...\n");

    socket_cliente = accept(sockfd, (struct sockaddr *)&cliente, &cliente_len);
    if (socket_cliente < 0) {
        perror("Error aceptando conexión");
        exit(EXIT_FAILURE);
    }

    printf("Cliente conectado. Recibiendo archivo...\n");
    guardar_archivo(socket_cliente);
    close(socket_cliente);
    close(sockfd);

    ejecutar_mpi();
    escribir_resultado();

    //inicializar_mano("/dev/ttyUSB0");
    //mover_derecha();
    //bajar_dedo();
    //cerrar_mano();

    return 0;
}
