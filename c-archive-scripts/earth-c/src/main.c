#include "earth/app.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

static void print_usage(const char *prog_name) {
    printf("Earth Renderer - Realistic 3D Wireframe Earth\n\n");
    printf("Usage: %s [options]\n\n", prog_name);
    printf("Display options:\n");
    printf("  --width <N>          Frame width (default: 800)\n");
    printf("  --height <N>         Frame height (default: 800)\n");
    printf("  --fps <N>            Target FPS (default: 60)\n");
    printf("  --fov <F>            Field of view in degrees (default: 55)\n");
    printf("\n");
    printf("Backend options:\n");
    printf("  --backend <TYPE>     Backend: ppm, sdl2, fbdev (default: ppm)\n");
    printf("  --out <PATH>         Output path (PPM backend)\n");
    printf("  --device <PATH>      Framebuffer device (fbdev backend)\n");
    printf("\n");
    printf("Geometry options:\n");
    printf("  --grid <N>           Lat/long grid divisions (default: 18)\n");
    printf("  --subdiv <N>         Sphere subdivisions (default: 4)\n");
    printf("  --seed <N>           Random seed for procedural features\n");
    printf("\n");
    printf("Render options:\n");
    printf("  --style <STYLE>      Render style: default, blueprint, neon,\n");
    printf("                       moonlight, terminal-green, noir, night\n");
    printf("  --speed <F>          Rotation speed multiplier (default: 0.8)\n");
    printf("  --thickness <F>      Line thickness (default: 1.0)\n");
    printf("  --no-bloom           Disable bloom effect\n");
    printf("  --no-stars           Disable starfield\n");
    printf("  --no-atmosphere      Disable atmosphere effects\n");
    printf("  --no-occlusion       Disable hidden line removal\n");
    printf("  --scanlines          Enable CRT scanline effect\n");
    printf("  --bloom <F>          Bloom intensity 0-1 (default: 0.4)\n");
    printf("\n");
    printf("Recording options:\n");
    printf("  --record <N>         Record N frames and exit\n");
    printf("  --demo               Demo mode (10 second loop)\n");
    printf("  --benchmark          Benchmark mode (print frame stats)\n");
    printf("\n");
    printf("Other:\n");
    printf("  --help               Show this help\n");
    printf("\n");
    printf("Examples:\n");
    printf("  %s --backend sdl2\n", prog_name);
    printf("  %s --width 1920 --height 1080 --out frame_%%05d.ppm --record 600\n", prog_name);
    printf("  %s --style neon --bloom 0.8 --speed 0.5 --backend sdl2\n", prog_name);
    printf("  %s --backend fbdev --device /dev/fb0\n", prog_name);
}

int main(int argc, char **argv) {
    config_t config;
    config_init(&config);

    static struct option long_options[] = {
        {"width", required_argument, 0, 'w'},
        {"height", required_argument, 0, 'h'},
        {"fps", required_argument, 0, 'f'},
        {"fov", required_argument, 0, 'v'},
        {"backend", required_argument, 0, 'b'},
        {"out", required_argument, 0, 'o'},
        {"device", required_argument, 0, 'd'},
        {"grid", required_argument, 0, 'g'},
        {"subdiv", required_argument, 0, 's'},
        {"seed", required_argument, 0, 'S'},
        {"style", required_argument, 0, 't'},
        {"speed", required_argument, 0, 'p'},
        {"thickness", required_argument, 0, 'T'},
        {"bloom", required_argument, 0, 'B'},
        {"no-bloom", no_argument, 0, 1001},
        {"no-stars", no_argument, 0, 1002},
        {"no-atmosphere", no_argument, 0, 1003},
        {"no-occlusion", no_argument, 0, 1004},
        {"scanlines", no_argument, 0, 1005},
        {"record", required_argument, 0, 'r'},
        {"demo", no_argument, 0, 1006},
        {"benchmark", no_argument, 0, 1007},
        {"help", no_argument, 0, '?'},
        {0, 0, 0, 0}
    };

    int opt;
    int option_index = 0;

    while ((opt = getopt_long(argc, argv, "w:h:f:v:b:o:d:g:s:S:t:p:T:B:r:?",
                              long_options, &option_index)) != -1) {
        switch (opt) {
            case 'w':
                config.width = atoi(optarg);
                break;
            case 'h':
                config.height = atoi(optarg);
                break;
            case 'f':
                config.fps = atoi(optarg);
                break;
            case 'v':
                config.fov = (float)atof(optarg);
                break;
            case 'b':
                if (strcmp(optarg, "ppm") == 0) {
                    config.backend = BACKEND_PPM;
                } else if (strcmp(optarg, "sdl2") == 0) {
#ifdef USE_SDL2
                    config.backend = BACKEND_SDL2;
#else
                    fprintf(stderr, "SDL2 backend not compiled in. Use -DUSE_SDL2.\n");
                    return 1;
#endif
                } else if (strcmp(optarg, "fbdev") == 0) {
#ifdef USE_FBDEV
                    config.backend = BACKEND_FBDEV;
#else
                    fprintf(stderr, "fbdev backend not compiled in. Use -DUSE_FBDEV.\n");
                    return 1;
#endif
                } else {
                    fprintf(stderr, "Unknown backend: %s\n", optarg);
                    return 1;
                }
                break;
            case 'o':
                strncpy(config.output_path, optarg, sizeof(config.output_path) - 1);
                break;
            case 'd':
                strncpy(config.fbdev_path, optarg, sizeof(config.fbdev_path) - 1);
                break;
            case 'g':
                config.grid_divisions = atoi(optarg);
                break;
            case 's':
                config.sphere_subdivisions = atoi(optarg);
                break;
            case 'S':
                config.seed = (uint32_t)atoi(optarg);
                break;
            case 't':
                if (strcmp(optarg, "default") == 0) {
                    config.style = STYLE_DEFAULT;
                } else if (strcmp(optarg, "blueprint") == 0) {
                    config.style = STYLE_BLUEPRINT;
                } else if (strcmp(optarg, "neon") == 0) {
                    config.style = STYLE_NEON;
                } else if (strcmp(optarg, "moonlight") == 0) {
                    config.style = STYLE_MOONLIGHT;
                } else if (strcmp(optarg, "terminal-green") == 0) {
                    config.style = STYLE_TERMINAL_GREEN;
                } else if (strcmp(optarg, "noir") == 0) {
                    config.style = STYLE_NOIR;
                } else if (strcmp(optarg, "night") == 0) {
                    config.style = STYLE_EARTH_AT_NIGHT;
                } else {
                    fprintf(stderr, "Unknown style: %s\n", optarg);
                    return 1;
                }
                break;
            case 'p':
                config.rotation_speed = (float)atof(optarg);
                break;
            case 'T':
                config.line_thickness = (float)atof(optarg);
                break;
            case 'B':
                config.bloom_intensity = (float)atof(optarg);
                break;
            case 1001:
                config.enable_bloom = false;
                break;
            case 1002:
                config.enable_stars = false;
                break;
            case 1003:
                config.enable_atmosphere = false;
                break;
            case 1004:
                config.enable_occlusion = false;
                break;
            case 1005:
                config.enable_scanlines = true;
                break;
            case 'r':
                config.record_mode = true;
                config.record_frames = atoi(optarg);
                break;
            case 1006:
                config.demo_mode = true;
                break;
            case 1007:
                config.benchmark_mode = true;
                break;
            case '?':
            default:
                print_usage(argv[0]);
                return 0;
        }
    }

    // Create and run application
    printf("Earth Renderer starting...\n");
    printf("Resolution: %dx%d @ %d FPS\n", config.width, config.height, config.fps);
    printf("Backend: ");
    switch (config.backend) {
        case BACKEND_PPM: printf("PPM\n"); break;
        case BACKEND_SDL2: printf("SDL2\n"); break;
        case BACKEND_FBDEV: printf("fbdev\n"); break;
    }

    app_t *app = app_create(&config);
    if (!app) {
        fprintf(stderr, "Failed to create application\n");
        return 1;
    }

    app_run(app);

    app_destroy(app);

    printf("Done.\n");
    return 0;
}
