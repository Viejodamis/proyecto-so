#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define FIFO_PATH "/tmp/pipe_principal"

// Función para asegurar que el FIFO exista
static void asegurar_fifo(const char *path) {
    struct stat st;
    if (stat(path, &st) == -1) {
        if (mkfifo(path, 0666) == -1) {
            perror("[SRV] mkfifo");
            exit(EXIT_FAILURE);
        }
    } else if (!S_ISFIFO(st.st_mode)) {
        fprintf(stderr, "[SRV] %s existe pero no es FIFO.\n", path);
        exit(EXIT_FAILURE);
    }
    chmod(path, 0666);
}

int main(void) {
    printf("[SRV] Configuración cargada:\n");
    printf("      Horario: 7 a 19 | Seg/hora: 2 | Aforo: 50 | FIFO: %s\n", FIFO_PATH);

    asegurar_fifo(FIFO_PATH);

    // Abrimos el FIFO en modo lectura
    int fd_r = open(FIFO_PATH, O_RDONLY);
    if (fd_r == -1) {
        perror("[SRV] open read");
        exit(EXIT_FAILURE);
    }

    // Mantiene abierto un descriptor de escritura para evitar EOF
    int fd_w_dummy = open(FIFO_PATH, O_WRONLY | O_NONBLOCK);
    if (fd_w_dummy == -1) {
        // No es crítico si falla
    }

    FILE *in = fdopen(fd_r, "r");
    if (!in) {
        perror("[SRV] fdopen");
        exit(EXIT_FAILURE);
    }

    printf("[SRV] Servidor inicializado correctamente ✅\n");
    printf("[SRV] Esperando datos de los agentes...\n");

    char linea[512];
    while (1) {
        if (fgets(linea, sizeof(linea), in) != NULL) {
            linea[strcspn(linea, "\r\n")] = '\0';
            if (linea[0] != '\0') {
                printf("[SRV] Recibido: %s\n", linea);
                fflush(stdout);
            }
        } else {
            clearerr(in);
            usleep(100000); // evita saturar la CPU
        }
    }

    if (fd_w_dummy != -1) close(fd_w_dummy);
    fclose(in);
    return 0;
}
