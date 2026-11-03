#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "servidor_matchmaker.h"
#include "juez_simulacion.h"
#include "resiliencia.h"

int matchmaker_inicio(ServidorMatchmaker *srv, int n, int m, ConfigInput *cfg) {
    srv->total_jugadores = n;
    srv->mesas_max = m;
    srv->cfg = *cfg;
    srv->ejecutando = 1;

    pthread_mutex_init(&srv->mutex_cola, NULL);

    if (sem_init(&srv->sem_mesas_disponibles, 0, m) != 0) {
        perror("sem_init");
        pthread_mutex_destroy(&srv->mutex_cola);
        return -1;
    }

    pthread_t tid_match;
    if (pthread_create(&tid_match, NULL, hilo_matchmaker, srv) != 0) {
        perror("pthread_create");
        return -1;
    }
    pthread_join(tid_match, NULL);
    return 0;
}

void* hilo_matchmaker(void *arg) {
    ServidorMatchmaker *srv = (ServidorMatchmaker *)arg;
    int parejas_totales = srv->total_jugadores / 2;

    for (int i = 0; i < parejas_totales && srv->ejecutando; i++) {
        if (resiliencia_detencion_solicitada()) {
            srv->ejecutando = 0;
            break;
        }

        sem_wait(&srv->sem_mesas_disponibles);
        MesaJuego *mesa = malloc(sizeof *mesa);
        if (!mesa) { sem_post(&srv->sem_mesas_disponibles); break; }
        if (mesa_inicio(mesa, i + 1, &srv->cfg, &srv->sem_mesas_disponibles) != 0) {
            free(mesa);
            sem_post(&srv->sem_mesas_disponibles);
        }
    }

    for (int k = 0; k < srv->mesas_max; k++)
        sem_wait(&srv->sem_mesas_disponibles);
    for (int k = 0; k < srv->mesas_max; k++)
        sem_post(&srv->sem_mesas_disponibles);

    return NULL;
}

void matchmaker_finalizo(ServidorMatchmaker *srv) {
    srv->ejecutando = 0;
    pthread_mutex_destroy(&srv->mutex_cola);
    sem_destroy(&srv->sem_mesas_disponibles);
}
