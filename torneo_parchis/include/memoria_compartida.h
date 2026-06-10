#ifndef MEMORIA_COMPARTIDA_H
#define MEMORIA_COMPARTIDA_H

#include "tipos.h"

EstadoJuego *crear_memoria_compartida(void);
void liberar_memoria_compartida(EstadoJuego *estado);

#endif
