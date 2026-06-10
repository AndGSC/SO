#ifndef IPC_SOCKETS_H
#define IPC_SOCKETS_H

#include "tipos.h"

#define PUERTO_BASE 5000
#define TAM_BUFFER_SOCKET 128

void inicializar_sockets_arbitro(void);
void inicializar_socket_jugador(int id_jugador);
void enviar_turno_socket(int id_jugador);
void esperar_turno_socket(int id_jugador);
void enviar_respuesta_socket(int id_jugador, const char *mensaje);
void recibir_respuesta_socket(int id_jugador);
void cerrar_sockets(void);

#endif
