# Torneo de Parchís — Sistemas Operativos

Simulador de una partida de Parchís en C sobre Linux donde los jugadores son
procesos y cada ficha es un hilo, compitiendo por recursos compartidos.

## Arquitectura

- **Procesos (`fork`)**: cada color (Rojo, Verde, Azul, Amarillo) es un proceso independiente.
- **Hilos (`pthread`)**: cada proceso jugador crea 4 hilos ficha que viven toda la partida y compiten por reclamar el movimiento de cada turno.
- **Memoria compartida (`mmap`)**: el tablero completo (casillas, posiciones, mutex y semáforos) es visible para todos los procesos.
- **Sincronización**: un mutex por casilla (`PTHREAD_PROCESS_SHARED`), semáforos para la zona de meta y los pasillos estrechos, y variables de condición para coordinar los hilos ficha.
- **IPC**: sockets TCP para los turnos (árbitro → jugadores), cola de mensajes System V para los eventos del juego (capturas, metas) y pipes para las estadísticas finales padre-hijo.
- **Scheduler**: el proceso padre (árbitro) asigna turnos con Round Robin.

## Compilación y ejecución

```bash
cd torneo_parchis
make        # compila con gcc -Wall -Wextra -pthread
make run    # compila y ejecuta
make clean  # elimina el binario
```

Requiere Linux (usa `fork`, `mmap`, pthreads, semáforos POSIX, colas System V y sockets).

## Entregables

- `torneo_parchis/` — código fuente en C y `Makefile`.
- `torneo_parchis/informe.docx` — informe técnico (diagrama de procesos e hilos, prevención de deadlock, justificación del scheduler y mecanismos de comunicación).
- `torneo_parchis/diagrama_procesos.txt` — diagrama en texto plano.
