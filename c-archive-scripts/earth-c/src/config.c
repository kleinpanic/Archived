#include "earth/config.h"
#include <string.h>

void config_init(config_t *cfg) {
    cfg->width = DEFAULT_WIDTH;
    cfg->height = DEFAULT_HEIGHT;
    cfg->fps = DEFAULT_FPS;
    cfg->backend = BACKEND_PPM;

    strcpy(cfg->output_path, "output.ppm");
    strcpy(cfg->fbdev_path, "/dev/fb0");

    cfg->grid_divisions = DEFAULT_GRID_DIVISIONS;
    cfg->sphere_subdivisions = DEFAULT_SPHERE_SUBDIVISIONS;

    cfg->fov = DEFAULT_FOV;
    cfg->rotation_speed = DEFAULT_ROTATION_SPEED;
    cfg->camera_distance = 3.0f;

    cfg->style = STYLE_DEFAULT;
    cfg->line_thickness = 1.0f;

    cfg->enable_stars = true;
    cfg->enable_bloom = true;
    cfg->enable_atmosphere = true;
    cfg->enable_occlusion = true;
    cfg->enable_scanlines = false;
    cfg->bloom_intensity = 0.4f;

    cfg->record_mode = false;
    cfg->record_frames = 0;
    cfg->record_duration = 0.0f;
    cfg->demo_mode = false;
    cfg->benchmark_mode = false;

    cfg->seed = 12345;
}
