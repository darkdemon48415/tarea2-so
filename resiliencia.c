#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <string.h>
#include "resiliencia.h"

static volatile sig_atomic_t detener = 0;

static void manejador(int sig) {
    (void)sig;
    detener = 1;
}

void resiliencia_configurar_senales(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = manejador;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT,  &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}

int resiliencia_detencion_solicitada(void) {
    return detener != 0;
}
