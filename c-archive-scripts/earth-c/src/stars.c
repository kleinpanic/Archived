#include "earth/stars.h"
#include "earth/math.h"
#include <stdlib.h>
#include <math.h>

// Simple LCG for star generation
static uint32_t star_seed = 0;

static float star_randf(void) {
    star_seed = star_seed * 1664525u + 1013904223u;
    return (float)(star_seed >> 8) / 16777216.0f;
}

starfield_t *starfield_create(int count, uint32_t seed) {
    starfield_t *stars = malloc(sizeof(starfield_t));
    if (!stars) return NULL;

    stars->count = count;
    stars->positions_x = malloc(sizeof(float) * count);
    stars->positions_y = malloc(sizeof(float) * count);
    stars->brightness = malloc(sizeof(float) * count);

    if (!stars->positions_x || !stars->positions_y || !stars->brightness) {
        starfield_destroy(stars);
        return NULL;
    }

    // Generate stars in NDC space [-1, 1]
    star_seed = seed;
    for (int i = 0; i < count; i++) {
        stars->positions_x[i] = star_randf() * 2.0f - 1.0f;
        stars->positions_y[i] = star_randf() * 2.0f - 1.0f;

        // Varied brightness (power distribution for more dim stars)
        float b = star_randf();
        stars->brightness[i] = b * b * b; // Cubic for more faint stars
    }

    return stars;
}

void starfield_destroy(starfield_t *stars) {
    if (!stars) return;
    free(stars->positions_x);
    free(stars->positions_y);
    free(stars->brightness);
    free(stars);
}

void starfield_render(starfield_t *stars, framebuffer_t *fb, float time, render_style_t style) {
    (void)time; // For future twinkling effect

    for (int i = 0; i < stars->count; i++) {
        // Convert NDC to screen space
        int x = (int)((stars->positions_x[i] + 1.0f) * 0.5f * fb->width);
        int y = (int)((1.0f - stars->positions_y[i]) * 0.5f * fb->height);

        if (x < 0 || x >= fb->width || y < 0 || y >= fb->height) continue;

        float brightness = stars->brightness[i];

        // Optional subtle twinkle
        // brightness *= 0.8f + 0.2f * sinf(time * 0.5f + (float)i * 0.1f);

        uint8_t intensity = float_to_byte(brightness);

        uint32_t color;
        switch (style) {
            case STYLE_BLUEPRINT:
                color = pack_color(intensity/3, intensity/2, intensity, 255);
                break;
            case STYLE_NEON:
                color = pack_color(intensity, intensity/4, intensity, 255);
                break;
            case STYLE_TERMINAL_GREEN:
                color = pack_color(0, intensity, intensity/4, 255);
                break;
            case STYLE_EARTH_AT_NIGHT:
                // Warm white stars
                color = pack_color(intensity, (uint8_t)(intensity * 0.9f), (uint8_t)(intensity * 0.7f), 255);
                break;
            default:
                // White stars
                color = pack_color(intensity, intensity, intensity, 255);
                break;
        }

        // Draw star (small 2x2 or 3x3 cluster for brighter stars)
        fb_set_pixel(fb, x, y, color);

        if (brightness > 0.5f) {
            // Brighter stars get a halo
            uint32_t dim_color = modulate_color(color, 0.3f);
            fb_blend_pixel(fb, x-1, y, dim_color, 0.3f);
            fb_blend_pixel(fb, x+1, y, dim_color, 0.3f);
            fb_blend_pixel(fb, x, y-1, dim_color, 0.3f);
            fb_blend_pixel(fb, x, y+1, dim_color, 0.3f);
        }
    }
}
