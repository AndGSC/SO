#ifndef IPC_COLAS_H
#define IPC_COLAS_H

#include "tipos.h"

#define EVENTO_MOVIMIENTO 1
#define EVENTO_CAPTURA 2
#define EVENTO_META 3
#define EVENTO_BLOQUEO 4

#define TAM_DESCRIPCION_EVENTO 128

typedef struct {
    long tipo_mensaje;
    int tipo_evento;
    int jugador;
    int ficha;
    char descripcion[TAM_DESCRIPCION_EVENTO];
} MensajeEvento;

void crear_cola_mensajes(void);

void enviar_evento(int tipo_evento, int jugador, int ficha, const char *descripcion);

void recibir_eventos_pendientes(void);

void eliminar_cola_mensajes(void);

#endif
