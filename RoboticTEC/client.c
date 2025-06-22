#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define PUERTO_SERVIDOR 8080
#define CLAVE_CIFRADO 3  // Desplazamiento para cifrado César

void cifrar_archivo(const char *archivo_entrada, const char *archivo_salida) {
    FILE *fin = fopen(archivo_entrada, "r");
    FILE *fout = fopen(archivo_salida, "w");

    if (!fin || !fout) {
        perror("Error abriendo archivos");
        exit(EXIT_FAILURE);
    }

    int c;
    while ((c = fgetc(fin)) != EOF) {
        if (c >= 32 && c <= 126) {
            c = ((c - 32 + CLAVE_CIFRADO) % 95) + 32;
        }
        fputc(c, fout);
    }

    fclose(fin);
    fclose(fout);
}

void enviar_archivo(const char *archivo_cifrado, const char *ip_servidor) {
    int sockfd;
    struct sockaddr_in servidor;
    char buffer[BUFFER_SIZE];
    FILE *archivo = fopen(archivo_cifrado, "r");

    if (!archivo) {
        perror("Error abriendo archivo cifrado");
        exit(EXIT_FAILURE);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creando socket");
        exit(EXIT_FAILURE);
    }

    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(PUERTO_SERVIDOR);
    servidor.sin_addr.s_addr = inet_addr(ip_servidor);

    if (connect(sockfd, (struct sockaddr *)&servidor, sizeof(servidor)) < 0) {
        perror("Error conectando al servidor");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, BUFFER_SIZE, archivo)) {
        send(sockfd, buffer, strlen(buffer), 0);
    }

    printf("Archivo cifrado enviado correctamente.\n");
    fclose(archivo);
    close(sockfd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <archivo_entrada.txt> <ip_servidor>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *archivo_entrada = argv[1];
    const char *archivo_salida = "archivo_cifrado.txt";
    const char *ip_servidor = argv[2];

    cifrar_archivo(archivo_entrada, archivo_salida);
    enviar_archivo(archivo_salida, ip_servidor);

    return 0;
}
