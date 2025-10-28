#ifndef COMUN_H
#define COMUN_H

#define HORA_MIN 7
#define HORA_MAX 19

#define MAX_NOMBRE 64
#define MAX_PATH   256
#define FIFO_DEF   "/tmp/pipe_principal"

// Estados de respuesta (para mensajes servidor-agente)
typedef enum {
    RESP_OK = 0,
    RESP_REPROGRAMADA,
    RESP_EXTEMPORANEA,
    RESP_NEGADA
} estado_resp_t;

#endif
