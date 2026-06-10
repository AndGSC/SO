#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "memoria_compartida.h"

/*
 * Crea la memoria compartida donde se almacenará el EstadoJuego.
 *
 * Se usa mmap() con:
 *   MAP_SHARED    (la memoria será compartida entre procesos)
 *   MAP_ANONYMOUS (no se asocia a ningún archivo físico)
 */
EstadoJuego *crear_memoria_compartida(void)
{
    EstadoJuego *estado;

    estado = mmap(
        NULL,
        sizeof(EstadoJuego),
        PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1,
        0
    );

    if (estado == MAP_FAILED) {
        perror("Error al crear la memoria compartida con mmap");
        exit(EXIT_FAILURE);
    }

    return estado;
}

void liberar_memoria_compartida(EstadoJuego *estado)
{
    if (estado == NULL) { return; }

    if (munmap(estado, sizeof(EstadoJuego)) == -1) {
        perror("Error al liberar la memoria compartida con munmap");
        exit(EXIT_FAILURE);
    }
}
