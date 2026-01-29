#include "earth/geom.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ============================================================================
// Geometry management
// ============================================================================

geometry_t *geom_create(int max_lines, int max_vertices) {
    geometry_t *geom = malloc(sizeof(geometry_t));
    if (!geom) return NULL;

    geom->lines = malloc(sizeof(line_segment_t) * max_lines);
    geom->vertices = malloc(sizeof(vec3_t) * max_vertices);

    if (!geom->lines || !geom->vertices) {
        geom_destroy(geom);
        return NULL;
    }

    geom->line_count = 0;
    geom->line_capacity = max_lines;
    geom->vertex_count = 0;
    geom->vertex_capacity = max_vertices;

    return geom;
}

void geom_destroy(geometry_t *geom) {
    if (!geom) return;
    free(geom->lines);
    free(geom->vertices);
    free(geom);
}

void geom_clear(geometry_t *geom) {
    geom->line_count = 0;
    geom->vertex_count = 0;
}

void geom_add_line(geometry_t *geom, vec3_t start, vec3_t end) {
    if (geom->line_count >= geom->line_capacity) return;

    geom->lines[geom->line_count++] = (line_segment_t){
        .start = start,
        .end = end,
        .intensity = 1.0f
    };
}

// ============================================================================
// Sphere generation
// ============================================================================

vec3_t sphere_point(float lat, float lon, float radius) {
    float lat_rad = lat * DEG_TO_RAD;
    float lon_rad = lon * DEG_TO_RAD;

    return (vec3_t){
        radius * cosf(lat_rad) * cosf(lon_rad),
        radius * sinf(lat_rad),
        radius * cosf(lat_rad) * sinf(lon_rad)
    };
}

void geom_add_lat_long_grid(geometry_t *geom, float radius, int lat_divs, int long_divs) {
    // Latitude lines (horizontal circles)
    for (int lat = -lat_divs/2 + 1; lat < lat_divs/2; lat++) {
        float latitude = (float)lat / (float)lat_divs * 180.0f;

        for (int lon = 0; lon < long_divs; lon++) {
            float lon1 = (float)lon / (float)long_divs * 360.0f;
            float lon2 = (float)(lon + 1) / (float)long_divs * 360.0f;

            vec3_t p1 = sphere_point(latitude, lon1, radius);
            vec3_t p2 = sphere_point(latitude, lon2, radius);

            geom_add_line(geom, p1, p2);
        }
    }

    // Longitude lines (meridians)
    for (int lon = 0; lon < long_divs; lon++) {
        float longitude = (float)lon / (float)long_divs * 360.0f;

        for (int lat = -lat_divs/2; lat < lat_divs/2; lat++) {
            float lat1 = (float)lat / (float)lat_divs * 180.0f;
            float lat2 = (float)(lat + 1) / (float)lat_divs * 180.0f;

            vec3_t p1 = sphere_point(lat1, longitude, radius);
            vec3_t p2 = sphere_point(lat2, longitude, radius);

            geom_add_line(geom, p1, p2);
        }
    }
}

// Icosahedron base vertices (golden ratio)
#define ICO_T 1.618033988749895f

static vec3_t ico_vertices[12] = {
    {-1, ICO_T, 0}, {1, ICO_T, 0}, {-1, -ICO_T, 0}, {1, -ICO_T, 0},
    {0, -1, ICO_T}, {0, 1, ICO_T}, {0, -1, -ICO_T}, {0, 1, -ICO_T},
    {ICO_T, 0, -1}, {ICO_T, 0, 1}, {-ICO_T, 0, -1}, {-ICO_T, 0, 1}
};

static int ico_edges[30][2] = {
    {0,11}, {0,5}, {0,1}, {0,7}, {0,10},
    {1,5}, {1,9}, {1,8}, {1,7},
    {2,11}, {2,10}, {2,3}, {2,6}, {2,4},
    {3,9}, {3,4}, {3,6}, {3,8},
    {4,9}, {4,5}, {4,11},
    {5,11}, {5,9},
    {6,10}, {6,7}, {6,8},
    {7,10}, {7,8},
    {8,9},
    {10,11}
};

void geom_add_icosphere(geometry_t *geom, float radius, int subdivisions) {
    // Start with icosahedron edges
    for (int i = 0; i < 30; i++) {
        vec3_t v0 = vec3_normalize(ico_vertices[ico_edges[i][0]]);
        vec3_t v1 = vec3_normalize(ico_vertices[ico_edges[i][1]]);

        if (subdivisions == 0) {
            geom_add_line(geom, vec3_scale(v0, radius), vec3_scale(v1, radius));
        } else {
            // Subdivide edge
            int segments = 1 << subdivisions; // 2^subdivisions
            for (int j = 0; j < segments; j++) {
                float t0 = (float)j / (float)segments;
                float t1 = (float)(j + 1) / (float)segments;

                vec3_t p0 = vec3_normalize(vec3_lerp(v0, v1, t0));
                vec3_t p1 = vec3_normalize(vec3_lerp(v0, v1, t1));

                geom_add_line(geom, vec3_scale(p0, radius), vec3_scale(p1, radius));
            }
        }
    }
}

void geom_add_great_circles(geometry_t *geom, float radius, int count) {
    // Add several great circles at different orientations
    for (int c = 0; c < count; c++) {
        float angle = (float)c / (float)count * PI;

        int segments = 64;
        for (int i = 0; i < segments; i++) {
            float t0 = (float)i / (float)segments * 2.0f * PI;
            float t1 = (float)(i + 1) / (float)segments * 2.0f * PI;

            // Circle in XY plane, then rotate
            vec3_t p0 = vec3(cosf(t0) * radius, sinf(t0) * radius, 0);
            vec3_t p1 = vec3(cosf(t1) * radius, sinf(t1) * radius, 0);

            // Rotate around Z axis
            float ca = cosf(angle);
            float sa = sinf(angle);
            vec3_t r0 = vec3(p0.x * ca - p0.y * sa, p0.x * sa + p0.y * ca, p0.z);
            vec3_t r1 = vec3(p1.x * ca - p1.y * sa, p1.x * sa + p1.y * ca, p1.z);

            geom_add_line(geom, r0, r1);
        }
    }
}

// Simple LCG random number generator
static uint32_t lcg_state = 12345;

static void seed_rng(uint32_t seed) {
    lcg_state = seed;
}

static float randf(void) {
    lcg_state = lcg_state * 1664525u + 1013904223u;
    return (float)(lcg_state >> 8) / 16777216.0f;
}

static float randf_range(float min, float max) {
    return min + randf() * (max - min);
}

void geom_add_procedural_coastlines(geometry_t *geom, float radius, uint32_t seed, int detail) {
    seed_rng(seed);

    // Generate several procedural "coastline-like" paths on the sphere
    int num_paths = 5 + (detail / 2);

    for (int path = 0; path < num_paths; path++) {
        // Random starting point
        float start_lat = randf_range(-60.0f, 60.0f);
        float start_lon = randf_range(0.0f, 360.0f);

        // Generate a wiggly path
        int segments = 20 + detail * 3;
        float lat = start_lat;
        float lon = start_lon;

        vec3_t prev = sphere_point(lat, lon, radius);

        for (int i = 0; i < segments; i++) {
            // Random walk
            lat += randf_range(-5.0f, 5.0f);
            lon += randf_range(-8.0f, 8.0f);

            // Clamp latitude
            if (lat < -80.0f) lat = -80.0f;
            if (lat > 80.0f) lat = 80.0f;

            // Wrap longitude
            if (lon < 0.0f) lon += 360.0f;
            if (lon >= 360.0f) lon -= 360.0f;

            vec3_t curr = sphere_point(lat, lon, radius);

            geom_add_line(geom, prev, curr);
            prev = curr;
        }
    }
}

// ============================================================================
// Utility
// ============================================================================

bool is_front_facing(vec3_t point, vec3_t view_dir) {
    // Point normal is the normalized position for a sphere at origin
    vec3_t normal = vec3_normalize(point);
    return vec3_dot(normal, view_dir) < 0.0f;
}

void geom_transform(geometry_t *geom, mat4_t matrix) {
    for (int i = 0; i < geom->line_count; i++) {
        vec4_t start4 = vec4(geom->lines[i].start.x, geom->lines[i].start.y,
                            geom->lines[i].start.z, 1.0f);
        vec4_t end4 = vec4(geom->lines[i].end.x, geom->lines[i].end.y,
                          geom->lines[i].end.z, 1.0f);

        vec4_t t_start = mat4_mul_vec4(matrix, start4);
        vec4_t t_end = mat4_mul_vec4(matrix, end4);

        geom->lines[i].start = vec3(t_start.x, t_start.y, t_start.z);
        geom->lines[i].end = vec3(t_end.x, t_end.y, t_end.z);
    }
}
