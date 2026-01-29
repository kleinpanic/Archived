#define _POSIX_C_SOURCE 199309L
#include "earth/app.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

double get_time_seconds(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
}

app_t *app_create(config_t *config) {
    app_t *app = malloc(sizeof(app_t));
    if (!app) return NULL;

    app->config = *config;

    // Create framebuffer
    app->fb = fb_create(config->width, config->height);
    if (!app->fb) {
        free(app);
        return NULL;
    }

    // Create backend
    switch (config->backend) {
        case BACKEND_PPM:
            app->backend = backend_ppm_create();
            break;

#ifdef USE_SDL2
        case BACKEND_SDL2:
            app->backend = backend_sdl2_create();
            break;
#endif

#ifdef USE_FBDEV
        case BACKEND_FBDEV:
            app->backend = backend_fbdev_create();
            break;
#endif

        default:
            fprintf(stderr, "Backend not available\n");
            fb_destroy(app->fb);
            free(app);
            return NULL;
    }

    app->backend->init(app->backend, &app->config);

    // Create render context
    app->render_ctx = render_create(app->fb, &app->config);
    if (!app->render_ctx) {
        app->backend->shutdown(app->backend);
        fb_destroy(app->fb);
        free(app->backend);
        free(app);
        return NULL;
    }

    // Create geometry
    app->geometry = geom_create(MAX_LINES, MAX_VERTICES);
    if (!app->geometry) {
        render_destroy(app->render_ctx);
        app->backend->shutdown(app->backend);
        fb_destroy(app->fb);
        free(app->backend);
        free(app);
        return NULL;
    }

    // Generate Earth geometry
    geom_add_lat_long_grid(app->geometry, 1.0f,
                          config->grid_divisions, config->grid_divisions * 2);

    if (config->style != STYLE_EARTH_AT_NIGHT) {
        geom_add_icosphere(app->geometry, 1.0f, config->sphere_subdivisions);
    }

    geom_add_procedural_coastlines(app->geometry, 1.0f, config->seed, 10);

    printf("Generated %d lines\n", app->geometry->line_count);

    // Create starfield
    if (config->enable_stars) {
        app->stars = starfield_create(500, config->seed + 1);
        if (!app->stars) {
            fprintf(stderr, "Warning: Failed to create starfield\n");
        }
    } else {
        app->stars = NULL;
    }

    // Initialize timing
    app->last_time = get_time_seconds();
    app->accumulator = 0.0;
    app->frame_count = 0;
    app->total_frame_time = 0.0;
    app->max_frame_time = 0.0;

    return app;
}

void app_destroy(app_t *app) {
    if (!app) return;

    // Print stats if benchmark mode
    if (app->config.benchmark_mode && app->frame_count > 0) {
        double avg_frame_time = app->total_frame_time / app->frame_count;
        double avg_fps = 1.0 / avg_frame_time;
        printf("\n=== Benchmark Results ===\n");
        printf("Frames rendered: %d\n", app->frame_count);
        printf("Avg frame time: %.2f ms\n", avg_frame_time * 1000.0);
        printf("Avg FPS: %.1f\n", avg_fps);
        printf("Max frame time: %.2f ms\n", app->max_frame_time * 1000.0);
        printf("Min FPS: %.1f\n", 1.0 / app->max_frame_time);
    }

    starfield_destroy(app->stars);
    geom_destroy(app->geometry);
    render_destroy(app->render_ctx);
    app->backend->shutdown(app->backend);
    free(app->backend);
    fb_destroy(app->fb);
    free(app);
}

void app_run(app_t *app) {
    double frame_time = 1.0 / (double)app->config.fps;

    while (true) {
        double current_time = get_time_seconds();
        double dt = current_time - app->last_time;
        app->last_time = current_time;

        app->accumulator += dt;

        // Fixed timestep update
        if (app->accumulator >= frame_time) {
            double frame_start = get_time_seconds();

            // Render frame (with optional starfield)
            render_frame(app->render_ctx, app->geometry, app->stars, (float)frame_time);

            // Present
            app->backend->present(app->backend, app->fb);

            double frame_end = get_time_seconds();
            double elapsed = frame_end - frame_start;

            app->total_frame_time += elapsed;
            if (elapsed > app->max_frame_time) {
                app->max_frame_time = elapsed;
            }

            app->frame_count++;
            app->accumulator -= frame_time;

            // Print progress for benchmark mode
            if (app->config.benchmark_mode && app->frame_count % 60 == 0) {
                double avg = app->total_frame_time / app->frame_count;
                printf("Frame %d: %.2f ms avg, %.1f FPS\n",
                       app->frame_count, avg * 1000.0, 1.0 / avg);
            }

            // Check quit conditions
            if (app->backend->should_quit(app->backend)) {
                break;
            }

            if (app->config.record_mode && app->config.record_frames > 0) {
                if (app->frame_count >= app->config.record_frames) {
                    break;
                }
            }

            if (app->config.demo_mode && app->frame_count >= 600) {
                break; // 10 seconds at 60 FPS
            }
        }

        // Sleep to maintain target FPS (if not recording)
        if (!app->config.record_mode && !app->config.benchmark_mode) {
            double sleep_time = frame_time - (get_time_seconds() - current_time);
            if (sleep_time > 0.001) {
                struct timespec ts;
                ts.tv_sec = 0;
                ts.tv_nsec = (long)(sleep_time * 1000000000.0);
                nanosleep(&ts, NULL);
            }
        }
    }
}
