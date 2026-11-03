# Tarea 2: Torneo de Komikan Concurrente

## Integrantes
* Eylan Acuña
* Eric Fernandez

## Descripción de Funciones Implementadas
El sistema consiste en un simulador multihilo robusto escrito en C (estándar C11) para gestionar partidas concurrentes del juego Komikan, estructurado bajo los siguientes componentes:

1. **Orquestador Principal (`main.c`)**:
   * **Control de Argumentos**: Valida estrictamente los parámetros de entrada por línea de comandos ($N$ jugadores, $M$ mesas máximas y el archivo de configuración opcional).
   * **Carga Dinámica de Parámetros (`config_input.c` / `config_input.h`)**: Implementa un analizador que lee de forma secuencial (`fscanf`) las variables operacionales (`seed`, `delay_min_ms`, `delay_max_ms`, `puma_init`) desde un archivo de texto, aplicando valores seguros por defecto en caso de error.

2. **Servidor Matchmaker (`servidor_matchmaker.c` / `servidor_matchmaker.h`)**:
   * **Control de Concurrencia**: Administra el emparejamiento de jugadores y el flujo de partidas utilizando semáforos POSIX (`sem_t`) para limitar de manera estricta el máximo de $M$ mesas activas simultáneamente.
   * **Sincronización Desacoplada**: Ejecuta la lógica del emparejamiento en un hilo independiente del sistema principal, previniendo condiciones de bloqueo mutuo (*deadlocks*) y garantizando una recolección asíncrona segura de los hilos de las mesas (`pthread_join`).

3. **Juez de Simulación (`juez_simulacion.c` / `juez_simulacion.h`)**:
   * **Rutina de Mesas en Segundo Plano**: Lanza cada mesa de juego como un hilo independiente en modo desasociado (*detached thread*), simulando la duración de las partidas mediante retardos variables controlados con `nanosleep`.
   * **Generación Aleatoria Segura**: Utiliza `rand_r` con semillas únicas calculadas a partir de la `seed` base y el ID de la mesa para simular de forma concurrente y determinista las victorias del Puma o de los Perros.

4. **Gestión de Tablero (`tablero.c`)**:
   * **Lógica de Estado**: Centraliza las operaciones, la inicialización de posiciones y las reglas esenciales de movimiento de las fichas dentro de la cuadrícula del Komikan.

5. **Módulo de Resiliencia (`resiliencia.c` / `resiliencia.h`)**:
   * **Manejo Seguro de Señales**: Captura de forma limpia las señales `SIGINT` (Ctrl+C) y `SIGTERM` implementando la API moderna de `sigaction`.
   * **Finalización Controlada**: Al interrumpirse el torneo, el sistema detiene la creación de nuevas partidas y sincroniza la destrucción de variables de condición, exclusión mutua (`pthread_mutex_destroy`) y semáforos, garantizando un cierre con cero recursos huérfanos.

## Instrucciones de Compilación
El binario compilado requiere dos argumentos numéricos obligatorios y permite un archivo de configuración opcional:

```bash
./komikan <N_jugadores> <M_mesas_max> [config.txt]
```

### Ejemplo de uso:
Para ejecutar una simulación con **10 jugadores**, un tope de **3 mesas simultáneas** en paralelo y leyendo la configuración externa:
```bash
./komikan 10 3 config.txt
```
