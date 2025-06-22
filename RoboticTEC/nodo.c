#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CLAVE_CIFRADO 3
#define MAX_PALABRA 100
#define MAX_TEXTO 100000
#define OVERLAP 16  // margen extra para evitar cortar palabras

// Descifra un carácter (algoritmo César inverso)
char descifrar_char(char c) {
    if (c >= 32 && c <= 126)
        return ((c - 32 - CLAVE_CIFRADO + 95) % 95) + 32;
    return c;
}

// Normaliza una palabra (convierte a minúscula)
void normalizar(char *palabra) {
    for (int i = 0; palabra[i]; ++i)
        palabra[i] = tolower(palabra[i]);
}

// Estructura para contar palabras
typedef struct {
    char palabra[MAX_PALABRA];
    int conteo;
} PalabraConteo;

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char *texto_total = NULL;
    int tam_texto = 0;

    if (rank == 0) {
        FILE *f = fopen("archivo_cifrado.txt", "r");
        if (!f) {
            perror("No se pudo abrir archivo_cifrado.txt");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        texto_total = malloc(MAX_TEXTO);
        tam_texto = fread(texto_total, 1, MAX_TEXTO - 1, f);
        texto_total[tam_texto] = '\0';
        fclose(f);
    }

    // Difundir tamaño del texto
    MPI_Bcast(&tam_texto, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Difundir el texto completo
    if (rank != 0) {
        texto_total = malloc(MAX_TEXTO);
    }
    MPI_Bcast(texto_total, tam_texto + 1, MPI_CHAR, 0, MPI_COMM_WORLD);

    // Calcular segmento a procesar
    int start = rank * (tam_texto / size);
    int end = (rank == size - 1) ? tam_texto : (rank + 1) * (tam_texto / size) + OVERLAP;
    if (end > tam_texto) end = tam_texto;
    int tam_local = end - start;

    char *bloque_local = malloc(tam_local + 1);
    memcpy(bloque_local, texto_total + start, tam_local);
    bloque_local[tam_local] = '\0';

    // Descifrar el bloque
    for (int i = 0; i < tam_local; ++i)
        bloque_local[i] = descifrar_char(bloque_local[i]);

    // Tokenizar palabras
    char *token = strtok(bloque_local, " \t\n\r.,;:!?()[]{}<>\"\'");
    PalabraConteo conteos_locales[1000];
    int n_local = 0;

    while (token != NULL) {
        normalizar(token);
        int encontrado = 0;
        for (int i = 0; i < n_local; ++i) {
            if (strcmp(conteos_locales[i].palabra, token) == 0) {
                conteos_locales[i].conteo++;
                encontrado = 1;
                break;
            }
        }
        if (!encontrado && n_local < 1000) {
            strcpy(conteos_locales[n_local].palabra, token);
            conteos_locales[n_local].conteo = 1;
            n_local++;
        }
        token = strtok(NULL, " \t\n\r.,;:!?()[]{}<>\"\'");
    }

    // Serializar conteos
    char buffer_envio[MAX_TEXTO];
    buffer_envio[0] = '\0';
    for (int i = 0; i < n_local; ++i) {
        char linea[150];
        sprintf(linea, "%s %d\n", conteos_locales[i].palabra, conteos_locales[i].conteo);
        strcat(buffer_envio, linea);
    }

    // Enviar tamaño del buffer
    int tam_envio = strlen(buffer_envio) + 1;
    int *tamanios = NULL;
    if (rank == 0) tamanios = malloc(size * sizeof(int));
    MPI_Gather(&tam_envio, 1, MPI_INT, tamanios, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Enviar conteos serializados al root
    char *recibidos = NULL;
    if (rank == 0) {
        int total = 0;
        for (int i = 0; i < size; ++i) total += tamanios[i];
        recibidos = malloc(total);
    }

    MPI_Gather(buffer_envio, tam_envio, MPI_CHAR, recibidos, tam_envio, MPI_CHAR, 0, MPI_COMM_WORLD);

    // Procesamiento final en root
    if (rank == 0) {
        PalabraConteo final[2000];
        int n_final = 0;

        char *linea = strtok(recibidos, "\n");
        while (linea) {
            char palabra[MAX_PALABRA];
            int cantidad;
            sscanf(linea, "%s %d", palabra, &cantidad);

            int encontrado = 0;
            for (int i = 0; i < n_final; ++i) {
                if (strcmp(final[i].palabra, palabra) == 0) {
                    final[i].conteo += cantidad;
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                strcpy(final[n_final].palabra, palabra);
                final[n_final].conteo = cantidad;
                n_final++;
            }

            linea = strtok(NULL, "\n");
        }

        // Encontrar la más frecuente
        int max = 0;
        char palabra_max[MAX_PALABRA];
        for (int i = 0; i < n_final; ++i) {
            if (final[i].conteo > max) {
                max = final[i].conteo;
                strcpy(palabra_max, final[i].palabra);
            }
        }

        // Guardar resultado
        FILE *out = fopen("archivo_descifrado.txt", "w");
        if (out) {
            fprintf(out, "%s %d\n", palabra_max, max);
            fclose(out);
            printf("Resultado guardado: %s (%d veces)\n", palabra_max, max);
        } else {
            perror("No se pudo guardar resultado");
        }

        free(recibidos);
        free(tamanios);
    }

    free(bloque_local);
    if (texto_total) free(texto_total);

    MPI_Finalize();
    return 0;
}
