#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "juez_simulacion.h"

static void dormir_ms(int ms) {
    struct timespec ts = { ms / 1000, (long)(ms % 1000) * 1000000L };
    nanosleep(&ts, NULL);
}

static void *hilo_mesa(void *arg) {
    MesaJuego *mesa = (MesaJuego *)arg;

    unsigned int semilla = (unsigned int)(mesa->cfg.seed + mesa->id * 7919);
    int rango = mesa->cfg.delay_max_ms - mesa->cfg.delay_min_ms + 1;
    int demora = mesa->cfg.delay_min_ms + (int)(rand_r(&semilla) % (unsigned)rango);

    printf("[Mesa %2d] Comienza partida (puma en %d,%d)\n",
           mesa->id, mesa->cfg.puma_init_x, mesa->cfg.puma_init_y);
    dormir_ms(demora);
    const char *ganador = (rand_r(&semilla) % 2) ? "Puma" : "Perros";
    printf("[Mesa %2d] Fin tras %d ms. Gana: %s\n", mesa->id, demora, ganador);

    sem_t *sem = mesa->sem_mesas;
    free(mesa);
    sem_post(sem);
    return NULL;
}

int mesa_inicio(MesaJuego *mesa, int id, const ConfigInput *cfg, sem_t *sem_mesas) {
    mesa->id = id;
    mesa->cfg = *cfg;
    mesa->sem_mesas = sem_mesas;

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_t tid;
    int rc = pthread_create(&tid, &attr, hilo_mesa, mesa);
    pthread_attr_destroy(&attr);
    if (rc != 0) {
        fprintf(stderr, "pthread_create (mesa): error %d\n", rc);
        return -1;
    }
    return 0;
}
