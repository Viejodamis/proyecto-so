#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <getopt.h>

static void uso(const char *prog) {
    fprintf(stderr, "Uso: %s -s <nombreAgente> -a <archivoCSV> -p <fifoServidor>\n", prog);
}

int main(int argc, char *argv[]) {
    const char *nombre = NULL;
    const char *csv = NULL;
    const char *fifo = NULL;

    int opt;
    while ((opt = getopt(argc, argv, "s:a:p:")) != -1) {
        switch (opt) {
            case 's': nombre = optarg; break;
            case 'a': csv    = optarg; break;
            case 'p': fifo   = optarg; break;
            default: uso(argv[0]); return EXIT_FAILURE;
        }
    }

    if (!nombre || !csv || !fifo) {
        uso(argv[0]);
        return EXIT_FAILURE;
    }

    printf("[CLI] Agente: %s | CSV: %s | FIFO: %s\n", nombre, csv, fifo);
    fflush(stdout);

    FILE *fcsv = fopen(csv, "r");
    if (!fcsv) {
        perror("[CLI] fopen CSV");
        return EXIT_FAILURE;
    }

    int fd = open(fifo, O_WRONLY);
    if (fd == -1) {
        perror("[CLI] open FIFO");
        fclose(fcsv);
        return EXIT_FAILURE;
    }

    FILE *out = fdopen(fd, "w");
    if (!out) {
        perror("[CLI] fdopen");
        close(fd);
        fclose(fcsv);
        return EXIT_FAILURE;
    }

    printf("[CLI] Agente listo para conectarse ✅\n");
    fflush(stdout);

    char linea[256];
    int i = 0;
    while (fgets(linea, sizeof(linea), fcsv)) {
        linea[strcspn(linea, "\r\n")] = '\0';
        if (linea[0] == '\0') continue;

        fprintf(out, "%s %s\n", nombre, linea);
        fflush(out);
        i++;
        sleep(1);
    }

    printf("[CLI] %s finalizó (%d lecturas).\n", nombre, i);
    fflush(stdout);

    fclose(out);
    fclose(fcsv);
    return 0;
}
