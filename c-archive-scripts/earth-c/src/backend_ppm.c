#include "earth/backend.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

typedef struct {
    int frame_number;
    char output_path[512];
    bool single_file;
} ppm_backend_data_t;

static void ppm_init(backend_t *backend, config_t *config) {
    ppm_backend_data_t *data = malloc(sizeof(ppm_backend_data_t));
    data->frame_number = 0;
    data->single_file = false;

    // Copy output path
    strncpy(data->output_path, config->output_path, sizeof(data->output_path) - 1);
    data->output_path[sizeof(data->output_path) - 1] = '\0';

    // Check if output path contains format specifier
    if (strstr(data->output_path, "%") == NULL) {
        data->single_file = true;
    } else {
        // Create output directory if needed
        char dir_path[512];
        strncpy(dir_path, data->output_path, sizeof(dir_path) - 1);

        // Find last '/'
        char *last_slash = strrchr(dir_path, '/');
        if (last_slash) {
            *last_slash = '\0';
            mkdir(dir_path, 0755);
        }
    }

    backend->user_data = data;
}

static void write_ppm(const char *filename, framebuffer_t *fb) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        fprintf(stderr, "Failed to open %s: %s\n", filename, strerror(errno));
        return;
    }

    // Write PPM header
    fprintf(f, "P6\n%d %d\n255\n", fb->width, fb->height);

    // Write pixel data (RGB only, drop alpha)
    for (int y = 0; y < fb->height; y++) {
        for (int x = 0; x < fb->width; x++) {
            uint32_t pixel = fb->pixels[y * fb->stride + x];
            uint8_t rgb[3] = {
                (uint8_t)(pixel & 0xFF),         // R
                (uint8_t)((pixel >> 8) & 0xFF),  // G
                (uint8_t)((pixel >> 16) & 0xFF)  // B
            };
            fwrite(rgb, 1, 3, f);
        }
    }

    fclose(f);
}

static void ppm_present(backend_t *backend, framebuffer_t *fb) {
    ppm_backend_data_t *data = (ppm_backend_data_t *)backend->user_data;

    char filename[1024];
    if (data->single_file) {
        strncpy(filename, data->output_path, sizeof(filename) - 1);
    } else {
        snprintf(filename, sizeof(filename), data->output_path, data->frame_number);
    }

    write_ppm(filename, fb);

    if (!data->single_file) {
        printf("Wrote frame %05d to %s\n", data->frame_number, filename);
    }

    data->frame_number++;
}

static bool ppm_should_quit(backend_t *backend) {
    (void)backend;
    return false; // Never quits on its own (controlled by app)
}

static void ppm_shutdown(backend_t *backend) {
    ppm_backend_data_t *data = (ppm_backend_data_t *)backend->user_data;
    if (data->single_file) {
        printf("Wrote final frame to %s\n", data->output_path);
    } else {
        printf("Wrote %d frames\n", data->frame_number);
    }
    free(data);
}

backend_t *backend_ppm_create(void) {
    backend_t *backend = malloc(sizeof(backend_t));
    backend->init = ppm_init;
    backend->present = ppm_present;
    backend->should_quit = ppm_should_quit;
    backend->shutdown = ppm_shutdown;
    backend->user_data = NULL;
    return backend;
}
