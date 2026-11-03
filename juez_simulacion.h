#ifndef JUEZ_SIMULACION_H
#define JUEZ_SIMULACION_H

#include <pthread.h>
#include <semaphore.h>
#include "config_input.h"

typedef struct {
    int id;
    ConfigInput cfg;
    sem_t *sem_mesas;
} MesaJuego;

int mesa_inicio(MesaJuego *mesa, int id, const ConfigInput *cfg, sem_t *sem_mesas);

#endif
