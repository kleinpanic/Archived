#include "earth/raster.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ============================================================================
// Framebuffer management
// ============================================================================

framebuffer_t *fb_create(int width, int height) {
    framebuffer_t *fb = malloc(sizeof(framebuffer_t));
    if (!fb) return NULL;

    fb->width = width;
    fb->height = height;
    fb->stride = width;

    fb->pixels = calloc((size_t)(width * height), sizeof(uint32_t));
    fb->depth = malloc(sizeof(float) * (size_t)(width * height));

    if (!fb->pixels || !fb->depth) {
        fb_destroy(fb);
        return NULL;
    }

    fb_clear_depth(fb);
    return fb;
}

void fb_destroy(framebuffer_t *fb) {
    if (!fb) return;
    free(fb->pixels);
    free(fb->depth);
    free(fb);
}

void fb_clear(framebuffer_t *fb, uint32_t color) {
    for (int i = 0; i < fb->width * fb->height; i++) {
        fb->pixels[i] = color;
    }
}

void fb_clear_depth(framebuffer_t *fb) {
    for (int i = 0; i < fb->width * fb->height; i++) {
        fb->depth[i] = 1.0f; // Far plane
    }
}

// ============================================================================
// Pixel operations
// ============================================================================

static inline bool in_bounds(framebuffer_t *fb, int x, int y) {
    return x >= 0 && x < fb->width && y >= 0 && y < fb->height;
}

void fb_set_pixel(framebuffer_t *fb, int x, int y, uint32_t color) {
    if (!in_bounds(fb, x, y)) return;
    fb->pixels[y * fb->stride + x] = color;
}

void fb_set_pixel_depth(framebuffer_t *fb, int x, int y, float depth, uint32_t color) {
    if (!in_bounds(fb, x, y)) return;

    int idx = y * fb->stride + x;
    if (depth < fb->depth[idx]) {
        fb->depth[idx] = depth;
        fb->pixels[idx] = color;
    }
}

void fb_blend_pixel(framebuffer_t *fb, int x, int y, uint32_t color, float alpha) {
    if (!in_bounds(fb, x, y)) return;

    int idx = y * fb->stride + x;
    fb->pixels[idx] = blend_colors(fb->pixels[idx], color, alpha);
}

// ============================================================================
// Blending utilities
// ============================================================================

uint32_t blend_colors(uint32_t dst, uint32_t src, float alpha) {
    if (alpha <= 0.0f) return dst;
    if (alpha >= 1.0f) return src;

    color_t d = unpack_color(dst);
    color_t s = unpack_color(src);

    // Premultiplied alpha blending
    float a = alpha;
    float inv_a = 1.0f - a;

    uint8_t r = (uint8_t)(s.r * a + d.r * inv_a);
    uint8_t g = (uint8_t)(s.g * a + d.g * inv_a);
    uint8_t b = (uint8_t)(s.b * a + d.b * inv_a);
    uint8_t final_a = (uint8_t)(s.a * a + d.a * inv_a);

    return pack_color(r, g, b, final_a);
}

uint32_t modulate_color(uint32_t color, float intensity) {
    if (intensity <= 0.0f) return pack_color(0, 0, 0, 255);
    if (intensity >= 1.0f) return color;

    color_t c = unpack_color(color);

    return pack_color(
        (uint8_t)(c.r * intensity),
        (uint8_t)(c.g * intensity),
        (uint8_t)(c.b * intensity),
        c.a
    );
}

// ============================================================================
// Gamma correction
// ============================================================================

float srgb_to_linear(float c) {
    if (c <= 0.04045f)
        return c / 12.92f;
    return powf((c + 0.055f) / 1.055f, 2.4f);
}

float linear_to_srgb(float c) {
    if (c <= 0.0031308f)
        return c * 12.92f;
    return 1.055f * powf(c, 1.0f / 2.4f) - 0.055f;
}

// ============================================================================
// Xiaolin Wu anti-aliased line drawing
// ============================================================================

static inline float fpart(float x) {
    return x - floorf(x);
}

static inline float rfpart(float x) {
    return 1.0f - fpart(x);
}

static void plot_aa(framebuffer_t *fb, int x, int y, float depth, uint32_t color, float alpha) {
    if (!in_bounds(fb, x, y)) return;

    int idx = y * fb->stride + x;

    // Simple depth test with AA
    if (depth < fb->depth[idx] + 0.001f) { // Small epsilon for AA
        fb->pixels[idx] = blend_colors(fb->pixels[idx], color, alpha);
        if (alpha > 0.5f) {
            fb->depth[idx] = depth;
        }
    }
}

void raster_line_aa(framebuffer_t *fb, vec2_t p0, vec2_t p1,
                    float z0, float z1, uint32_t color, float thickness) {
    (void)thickness; // Reserved for future use
    float x0 = p0.x, y0 = p0.y;
    float x1 = p1.x, y1 = p1.y;

    bool steep = fabsf(y1 - y0) > fabsf(x1 - x0);

    if (steep) {
        float tmp;
        tmp = x0; x0 = y0; y0 = tmp;
        tmp = x1; x1 = y1; y1 = tmp;
    }

    if (x0 > x1) {
        float tmp;
        tmp = x0; x0 = x1; x1 = tmp;
        tmp = y0; y0 = y1; y1 = tmp;
        tmp = z0; z0 = z1; z1 = tmp;
    }

    float dx = x1 - x0;
    float dy = y1 - y0;
    float gradient = (dx == 0.0f) ? 1.0f : dy / dx;

    // Handle first endpoint
    float xend = roundf(x0);
    float yend = y0 + gradient * (xend - x0);
    float xgap = rfpart(x0 + 0.5f);
    int xpxl1 = (int)xend;
    int ypxl1 = (int)floorf(yend);

    float depth_step = (dx != 0.0f) ? (z1 - z0) / dx : 0.0f;
    float curr_depth = z0;

    if (steep) {
        plot_aa(fb, ypxl1, xpxl1, curr_depth, color, rfpart(yend) * xgap);
        plot_aa(fb, ypxl1 + 1, xpxl1, curr_depth, color, fpart(yend) * xgap);
    } else {
        plot_aa(fb, xpxl1, ypxl1, curr_depth, color, rfpart(yend) * xgap);
        plot_aa(fb, xpxl1, ypxl1 + 1, curr_depth, color, fpart(yend) * xgap);
    }

    float intery = yend + gradient;
    curr_depth += depth_step;

    // Handle second endpoint
    xend = roundf(x1);
    yend = y1 + gradient * (xend - x1);
    xgap = fpart(x1 + 0.5f);
    int xpxl2 = (int)xend;
    int ypxl2 = (int)floorf(yend);

    if (steep) {
        plot_aa(fb, ypxl2, xpxl2, z1, color, rfpart(yend) * xgap);
        plot_aa(fb, ypxl2 + 1, xpxl2, z1, color, fpart(yend) * xgap);
    } else {
        plot_aa(fb, xpxl2, ypxl2, z1, color, rfpart(yend) * xgap);
        plot_aa(fb, xpxl2, ypxl2 + 1, z1, color, fpart(yend) * xgap);
    }

    // Main loop
    for (int x = xpxl1 + 1; x < xpxl2; x++) {
        if (steep) {
            plot_aa(fb, (int)floorf(intery), x, curr_depth, color, rfpart(intery));
            plot_aa(fb, (int)floorf(intery) + 1, x, curr_depth, color, fpart(intery));
        } else {
            plot_aa(fb, x, (int)floorf(intery), curr_depth, color, rfpart(intery));
            plot_aa(fb, x, (int)floorf(intery) + 1, curr_depth, color, fpart(intery));
        }
        intery += gradient;
        curr_depth += depth_step;
    }
}

void raster_thick_line(framebuffer_t *fb, vec2_t p0, vec2_t p1,
                      float z0, float z1, uint32_t color, float thickness) {
    if (thickness <= 1.0f) {
        raster_line_aa(fb, p0, p1, z0, z1, color, thickness);
        return;
    }

    // For thick lines, draw multiple offset lines
    float dx = p1.x - p0.x;
    float dy = p1.y - p0.y;
    float len = sqrtf(dx * dx + dy * dy);

    if (len < 0.001f) return;

    float nx = -dy / len;
    float ny = dx / len;

    int steps = (int)(thickness + 0.5f);
    for (int i = -steps/2; i <= steps/2; i++) {
        float offset = (float)i * 0.5f;
        vec2_t pp0 = vec2(p0.x + nx * offset, p0.y + ny * offset);
        vec2_t pp1 = vec2(p1.x + nx * offset, p1.y + ny * offset);

        float alpha = 1.0f - fabsf((float)i) / (float)steps;
        uint32_t mod_color = modulate_color(color, alpha);

        raster_line_aa(fb, pp0, pp1, z0, z1, mod_color, 1.0f);
    }
}
