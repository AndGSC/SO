#include "colores.h"

const char *nombre_jugador(int id_jugador)
{
    switch (id_jugador) {
        case ROJO:
            return "Rojo";

        case VERDE:
            return "Verde";

        case AZUL:
            return "Azul";

        case AMARILLO:
            return "Amarillo";

        default:
            return "Desconocido";
    }
}

/*
 * Devuelve un símbolo de un solo carácter para representar
 * al jugador en la salida del tablero.
 */
char simbolo_jugador(int id_jugador)
{
    switch (id_jugador) {
        case ROJO:
            return 'R';

        case VERDE:
            return 'V';

        case AZUL:
            return 'A';

        case AMARILLO:
            return 'M';

        default:
            return '?';
    }
}
