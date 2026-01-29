#include "earth/render.h"
#include "earth/stars.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ============================================================================
// Context management
// ============================================================================

render_context_t *render_create(framebuffer_t *fb, config_t *config) {
    render_context_t *ctx = malloc(sizeof(render_context_t));
    if (!ctx) return NULL;

    ctx->fb = fb;
    ctx->config = config;
    ctx->time = 0.0f;
    ctx->rotation_angle = 0.0f;

    ctx->projected_lines = malloc(sizeof(projected_line_t) * MAX_LINES);
    ctx->projected_count = 0;
    ctx->projected_capacity = MAX_LINES;

    if (!ctx->projected_lines) {
        free(ctx);
        return NULL;
    }

    // Initialize light direction (from top-right-front)
    ctx->light_dir = vec3_normalize(vec3(0.5f, 0.7f, -1.0f));

    render_update_camera(ctx);

    return ctx;
}

void render_destroy(render_context_t *ctx) {
    if (!ctx) return;
    free(ctx->projected_lines);
    free(ctx);
}

// ============================================================================
// Camera setup
// ============================================================================

void render_update_camera(render_context_t *ctx) {
    float aspect = (float)ctx->config->width / (float)ctx->config->height;

    // Position camera
    float cam_dist = 3.0f;
    ctx->camera_pos = vec3(0.0f, 0.3f, cam_dist);

    // View matrix
    ctx->view_matrix = mat4_look_at(
        ctx->camera_pos,
        vec3(0, 0, 0),
        vec3(0, 1, 0)
    );

    // Projection matrix
    ctx->proj_matrix = mat4_perspective(ctx->config->fov, aspect, 0.1f, 100.0f);

    // Combined matrix
    ctx->view_proj_matrix = mat4_multiply(ctx->proj_matrix, ctx->view_matrix);
}

// ============================================================================
// Projection and visibility
// ============================================================================

static bool project_point(render_context_t *ctx, vec3_t world_pos,
                         vec2_t *screen_pos, float *depth) {
    // Transform to clip space
    vec4_t clip = mat4_mul_vec4(ctx->view_proj_matrix,
                                vec4(world_pos.x, world_pos.y, world_pos.z, 1.0f));

    // Perspective divide
    if (fabsf(clip.w) < 0.0001f) return false;

    float inv_w = 1.0f / clip.w;
    vec3_t ndc = vec3(clip.x * inv_w, clip.y * inv_w, clip.z * inv_w);

    // Depth for depth buffer
    *depth = (ndc.z + 1.0f) * 0.5f; // Map [-1,1] to [0,1]

    // NDC to screen space
    screen_pos->x = (ndc.x + 1.0f) * 0.5f * ctx->fb->width;
    screen_pos->y = (1.0f - ndc.y) * 0.5f * ctx->fb->height; // Flip Y

    return clip.w > 0.0f; // In front of camera
}

static float compute_lighting(render_context_t *ctx, vec3_t world_pos, vec3_t normal) {
    // Directional light
    float diffuse = clamp_f(vec3_dot(normal, ctx->light_dir), 0.0f, 1.0f);

    // Ambient
    float ambient = 0.3f;

    // Rim lighting for atmosphere effect
    vec3_t view_dir = vec3_normalize(vec3_sub(ctx->camera_pos, world_pos));
    float rim = 1.0f - fabsf(vec3_dot(normal, view_dir));
    rim = powf(rim, 3.0f) * 0.4f;

    return clamp_f(ambient + diffuse * 0.7f + rim, 0.0f, 1.0f);
}

void render_project_geometry(render_context_t *ctx, geometry_t *geom) {
    ctx->projected_count = 0;

    for (int i = 0; i < geom->line_count && ctx->projected_count < ctx->projected_capacity; i++) {
        line_segment_t *line = &geom->lines[i];

        // Check backface culling (simple sphere test)
        vec3_t mid = vec3_scale(vec3_add(line->start, line->end), 0.5f);
        vec3_t normal = vec3_normalize(mid);
        vec3_t view_dir = vec3_normalize(vec3_sub(ctx->camera_pos, mid));

        // Skip back-facing lines if occlusion is enabled
        if (ctx->config->enable_occlusion) {
            if (vec3_dot(normal, view_dir) < -0.1f) {
                continue; // Back-facing
            }
        }

        // Project endpoints
        vec2_t p0, p1;
        float z0 = 0.0f, z1 = 0.0f;

        bool v0 = project_point(ctx, line->start, &p0, &z0);
        bool v1 = project_point(ctx, line->end, &p1, &z1);

        if (!v0 || !v1) continue; // Behind camera

        // Compute lighting
        vec3_t n0 = vec3_normalize(line->start);
        vec3_t n1 = vec3_normalize(line->end);
        float intensity = (compute_lighting(ctx, line->start, n0) +
                          compute_lighting(ctx, line->end, n1)) * 0.5f;

        // Add to projected list
        projected_line_t *proj = &ctx->projected_lines[ctx->projected_count++];
        proj->p0 = p0;
        proj->p1 = p1;
        proj->z0 = z0;
        proj->z1 = z1;
        proj->intensity = intensity;
        proj->visible = true;
    }
}

// ============================================================================
// Render style colors
// ============================================================================

static uint32_t get_style_color(config_t *cfg, float intensity) {
    uint8_t i = float_to_byte(intensity);

    switch (cfg->style) {
        case STYLE_BLUEPRINT:
            return pack_color(i/3, i/2, i, 255);

        case STYLE_NEON:
            return pack_color(i, i/4, i, 255);

        case STYLE_MOONLIGHT:
            return pack_color(i/2, i/2, i, 255);

        case STYLE_TERMINAL_GREEN:
            return pack_color(0, i, i/4, 255);

        case STYLE_NOIR:
            return pack_color(i, i, i, 255);

        case STYLE_EARTH_AT_NIGHT: {
            // Warm city lights
            uint8_t r = float_to_byte(intensity * 1.0f);
            uint8_t g = float_to_byte(intensity * 0.8f);
            uint8_t b = float_to_byte(intensity * 0.3f);
            return pack_color(r, g, b, 255);
        }

        default: // STYLE_DEFAULT
            return pack_color(i, i, i, 255);
    }
}

void render_draw_lines(render_context_t *ctx) {
    float thickness = ctx->config->line_thickness;

    for (int i = 0; i < ctx->projected_count; i++) {
        projected_line_t *line = &ctx->projected_lines[i];
        if (!line->visible) continue;

        uint32_t color = get_style_color(ctx->config, line->intensity);

        if (thickness <= 1.5f) {
            raster_line_aa(ctx->fb, line->p0, line->p1,
                          line->z0, line->z1, color, thickness);
        } else {
            raster_thick_line(ctx->fb, line->p0, line->p1,
                             line->z0, line->z1, color, thickness);
        }
    }
}

// ============================================================================
// Post-processing
// ============================================================================

void render_apply_bloom(framebuffer_t *fb, float intensity) {
    if (intensity < 0.01f) return;

    // Simple box blur for bloom effect
    uint32_t *temp = malloc(sizeof(uint32_t) * fb->width * fb->height);
    if (!temp) return;

    memcpy(temp, fb->pixels, sizeof(uint32_t) * fb->width * fb->height);

    int radius = 3;
    for (int y = radius; y < fb->height - radius; y++) {
        for (int x = radius; x < fb->width - radius; x++) {
            int r_sum = 0, g_sum = 0, b_sum = 0;
            int count = 0;

            // Sample neighborhood
            for (int dy = -radius; dy <= radius; dy++) {
                for (int dx = -radius; dx <= radius; dx++) {
                    color_t c = unpack_color(temp[(y + dy) * fb->stride + (x + dx)]);
                    r_sum += c.r;
                    g_sum += c.g;
                    b_sum += c.b;
                    count++;
                }
            }

            uint8_t r_avg = (uint8_t)(r_sum / count);
            uint8_t g_avg = (uint8_t)(g_sum / count);
            uint8_t b_avg = (uint8_t)(b_sum / count);

            // Blend bloom with original
            color_t orig = unpack_color(temp[y * fb->stride + x]);

            uint8_t r_final = (uint8_t)(orig.r + r_avg * intensity * 0.3f);
            uint8_t g_final = (uint8_t)(orig.g + g_avg * intensity * 0.3f);
            uint8_t b_final = (uint8_t)(orig.b + b_avg * intensity * 0.3f);

            fb->pixels[y * fb->stride + x] = pack_color(r_final, g_final, b_final, 255);
        }
    }

    free(temp);
}

void render_apply_vignette(framebuffer_t *fb, float strength) {
    if (strength < 0.01f) return;

    float cx = fb->width * 0.5f;
    float cy = fb->height * 0.5f;
    float max_dist = sqrtf(cx * cx + cy * cy);

    for (int y = 0; y < fb->height; y++) {
        for (int x = 0; x < fb->width; x++) {
            float dx = x - cx;
            float dy = y - cy;
            float dist = sqrtf(dx * dx + dy * dy);
            float factor = 1.0f - (dist / max_dist) * strength;
            factor = clamp_f(factor, 0.0f, 1.0f);

            color_t c = unpack_color(fb->pixels[y * fb->stride + x]);
            fb->pixels[y * fb->stride + x] = pack_color(
                (uint8_t)(c.r * factor),
                (uint8_t)(c.g * factor),
                (uint8_t)(c.b * factor),
                c.a
            );
        }
    }
}

void render_apply_scanlines(framebuffer_t *fb) {
    for (int y = 0; y < fb->height; y += 2) {
        for (int x = 0; x < fb->width; x++) {
            color_t c = unpack_color(fb->pixels[y * fb->stride + x]);
            fb->pixels[y * fb->stride + x] = pack_color(
                (uint8_t)(c.r * 0.9f),
                (uint8_t)(c.g * 0.9f),
                (uint8_t)(c.b * 0.9f),
                c.a
            );
        }
    }
}

void render_post_process(render_context_t *ctx) {
    if (ctx->config->enable_bloom) {
        render_apply_bloom(ctx->fb, ctx->config->bloom_intensity);
    }

    if (ctx->config->enable_atmosphere) {
        render_apply_vignette(ctx->fb, 0.4f);
    }

    if (ctx->config->enable_scanlines) {
        render_apply_scanlines(ctx->fb);
    }
}

// ============================================================================
// Main render function
// ============================================================================

void render_frame(render_context_t *ctx, geometry_t *geom, void *starfield_ptr, float dt) {
    ctx->time += dt;
    ctx->rotation_angle += dt * ctx->config->rotation_speed;

    // Clear
    fb_clear(ctx->fb, pack_color(0, 0, 0, 255));
    if (ctx->config->enable_occlusion) {
        fb_clear_depth(ctx->fb);
    }

    // Render starfield (background layer)
    if (starfield_ptr && ctx->config->enable_stars) {
        starfield_t *stars = (starfield_t *)starfield_ptr;
        starfield_render(stars, ctx->fb, ctx->time, ctx->config->style);
    }

    // Update camera
    render_update_camera(ctx);

    // Rotate geometry
    mat4_t rot_y = mat4_rotate_y(ctx->rotation_angle);
    mat4_t rot_x = mat4_rotate_x(ctx->rotation_angle * 0.3f);
    mat4_t rot = mat4_multiply(rot_y, rot_x);

    // Create temporary rotated geometry
    geometry_t temp_geom = *geom;
    temp_geom.lines = malloc(sizeof(line_segment_t) * geom->line_count);
    memcpy(temp_geom.lines, geom->lines, sizeof(line_segment_t) * geom->line_count);

    geom_transform(&temp_geom, rot);

    // Project and render
    render_project_geometry(ctx, &temp_geom);
    render_draw_lines(ctx);

    // Post-processing
    render_post_process(ctx);

    free(temp_geom.lines);
}
