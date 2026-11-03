#ifndef SERVIDOR_MATCHMAKER_H
#define SERVIDOR_MATCHMAKER_H

#include <pthread.h>
#include <semaphore.h>
#include "config_input.h"

typedef struct {
    int total_jugadores;
    int mesas_max;
    ConfigInput cfg;
    
    sem_t sem_mesas_disponibles;
    pthread_mutex_t mutex_cola;
    volatile int ejecutando;
} ServidorMatchmaker;

int matchmaker_inicio(ServidorMatchmaker *srv, int n, int m, ConfigInput *cfg);
void* hilo_matchmaker(void *arg);
void matchmaker_finalizo(ServidorMatchmaker *srv);

#endif
