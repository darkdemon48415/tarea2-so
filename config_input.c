#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config_input.h"

int configuracion_cargar(const char *archivo, ConfigInput *cfg) {
    cfg->seed = 42;
    cfg->delay_min_ms = 50;
    cfg->delay_max_ms = 300;
    cfg->puma_init_x = 2;
    cfg->puma_init_y = 2;

    if (!archivo) return 0;
    FILE *f = fopen(archivo, "r");
    if (!f) return -1;

    char key[64];
    while (fscanf(f, "%63s", key) == 1) {
        if (strcmp(key, "seed") == 0) fscanf(f, "%d", &cfg->seed);
        else if (strcmp(key, "delay_min_ms") == 0) fscanf(f, "%d", &cfg->delay_min_ms);
        else if (strcmp(key, "delay_max_ms") == 0) fscanf(f, "%d", &cfg->delay_max_ms);
        else if (strcmp(key, "puma_init") == 0) fscanf(f, "%d %d", &cfg->puma_init_x, &cfg->puma_init_y);
    }
    fclose(f);

    if (cfg->delay_min_ms < 0) cfg->delay_min_ms = 0;
    if (cfg->delay_max_ms < cfg->delay_min_ms) cfg->delay_max_ms = cfg->delay_min_ms;
    return 0;
}
