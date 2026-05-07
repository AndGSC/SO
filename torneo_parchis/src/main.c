#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "config.h"
#include "tipos.h"
#include "memoria_compartida.h"
#include "sincronizacion.h"
#include "tablero.h"
#include "estadisticas.h"
#include "jugador.h"
#include "arbitro.h"
#include "ipc_pipes.h"
#include "ipc_colas.h"
#include "ipc_sockets.h"
#include "colores.h"

static void crear_procesos_jugadores(EstadoJuego *estado, pid_t pids[])
{
    for (int i = 0; i < NUM_JUGADORES; i++) {
        pids[i] = fork();

        if (pids[i] < 0) {
            perror("Error al crear proceso jugador con fork");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0) {
            cerrar_pipes_no_usados(i);

            inicializar_socket_jugador(i);

            ejecutar_jugador(i, estado);

            enviar_estadisticas_pipe(i, estado->estadisticas[i]);

            cerrar_sockets();

            exit(EXIT_SUCCESS);
        }

        printf("Proceso jugador %s creado con PID: %d\n",
               nombre_jugador(i),
               pids[i]);
    }
}

static void despertar_jugadores_bloqueados(EstadoJuego *estado)
{
    for (int i = 0; i < NUM_JUGADORES; i++) {
        if (estado->ganador != SIN_GANADOR && i == estado->ganador) {
            continue;
        }

        enviar_turno_socket(i);
    }
}

static void esperar_procesos_jugadores(pid_t pids[])
{
    for (int i = 0; i < NUM_JUGADORES; i++) {
        if (waitpid(pids[i], NULL, 0) == -1) {
            perror("Error al esperar proceso jugador");
            exit(EXIT_FAILURE);
        }
    }
}

int main(void)
{
    EstadoJuego *estado;
    pid_t pids[NUM_JUGADORES];

    printf("Iniciando torneo de parchis...\n");

    estado = crear_memoria_compartida();

    inicializar_sincronizacion(estado);
    inicializar_tablero(estado);
    inicializar_estadisticas(estado);

    crear_pipes();
    crear_cola_mensajes();
    inicializar_sockets_arbitro();

    crear_procesos_jugadores(estado, pids);

    ejecutar_arbitro(estado);

    despertar_jugadores_bloqueados(estado);

    recibir_estadisticas_pipe(estado);

    esperar_procesos_jugadores(pids);

    imprimir_estadisticas_finales(estado);

    cerrar_sockets();
    eliminar_cola_mensajes();
    destruir_sincronizacion(estado);
    liberar_memoria_compartida(estado);

    printf("\nTorneo finalizado correctamente.\n");

    return EXIT_SUCCESS;
}
