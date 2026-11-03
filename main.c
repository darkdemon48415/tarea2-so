#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config_input.h"
#include "servidor_matchmaker.h"
#include "resiliencia.h"

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Uso: %s N_jugadores M_mesas_max [config.txt]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n_jugadores = atoi(argv[1]);
    int m_mesas     = atoi(argv[2]);
    const char *archivo_config = (argc == 4) ? argv[3] : NULL;

    if (n_jugadores < 2 || m_mesas < 1) {
        fprintf(stderr, "Error: se requieren N >= 2 y M >= 1.\n");
        return EXIT_FAILURE;
    }

    resiliencia_configurar_senales();

    ConfigInput cfg;
    if (configuracion_cargar(archivo_config, &cfg) != 0) {
        fprintf(stderr, "Aviso: no se pudo abrir '%s'; se usan valores por defecto.\n", archivo_config);
    }
    srand(cfg.seed);

    printf("Iniciando Torneo Komikan Concurrente (N=%d, M=%d, Seed=%d)\n",
           n_jugadores, m_mesas, cfg.seed);

    ServidorMatchmaker srv;
    if (matchmaker_inicio(&srv, n_jugadores, m_mesas, &cfg) != 0) {
        fprintf(stderr, "Error al iniciar el servidor.\n");
        return EXIT_FAILURE;
    }
    matchmaker_finalizo(&srv);

    if (resiliencia_detencion_solicitada())
        printf("Torneo interrumpido por señal. Recursos liberados correctamente.\n");
    else
        printf("Torneo finalizado con éxito. Cero recursos huérfanos.\n");
    return EXIT_SUCCESS;
}
