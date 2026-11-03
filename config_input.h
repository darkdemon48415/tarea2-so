#ifndef CONFIG_INPUT_H
#define CONFIG_INPUT_H

typedef struct {
    int seed;
    int delay_min_ms;
    int delay_max_ms;
    int puma_init_x;
    int puma_init_y;
} ConfigInput;

int configuracion_cargar(const char *archivo, ConfigInput *cfg);

#endif
