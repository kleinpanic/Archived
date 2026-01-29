#include "earth/math.h"
#include <string.h>

// ============================================================================
// Vector3 operations
// ============================================================================

vec3_t vec3_add(vec3_t a, vec3_t b) {
    return (vec3_t){a.x + b.x, a.y + b.y, a.z + b.z};
}

vec3_t vec3_sub(vec3_t a, vec3_t b) {
    return (vec3_t){a.x - b.x, a.y - b.y, a.z - b.z};
}

vec3_t vec3_scale(vec3_t v, float s) {
    return (vec3_t){v.x * s, v.y * s, v.z * s};
}

float vec3_dot(vec3_t a, vec3_t b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3_t vec3_cross(vec3_t a, vec3_t b) {
    return (vec3_t){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

float vec3_length_sq(vec3_t v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

float vec3_length(vec3_t v) {
    return sqrtf(vec3_length_sq(v));
}

vec3_t vec3_normalize(vec3_t v) {
    float len = vec3_length(v);
    if (len < 1e-8f) return (vec3_t){0, 0, 0};
    float inv_len = 1.0f / len;
    return vec3_scale(v, inv_len);
}

vec3_t vec3_lerp(vec3_t a, vec3_t b, float t) {
    return (vec3_t){
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t
    };
}

// ============================================================================
// Matrix operations
// ============================================================================

mat4_t mat4_identity(void) {
    mat4_t m = {0};
    m.m[0] = m.m[5] = m.m[10] = m.m[15] = 1.0f;
    return m;
}

mat4_t mat4_multiply(mat4_t a, mat4_t b) {
    mat4_t result = {0};
    for (int col = 0; col < 4; col++) {
        for (int row = 0; row < 4; row++) {
            float sum = 0.0f;
            for (int k = 0; k < 4; k++) {
                sum += a.m[k * 4 + row] * b.m[col * 4 + k];
            }
            result.m[col * 4 + row] = sum;
        }
    }
    return result;
}

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v) {
    return (vec4_t){
        m.m[0] * v.x + m.m[4] * v.y + m.m[8]  * v.z + m.m[12] * v.w,
        m.m[1] * v.x + m.m[5] * v.y + m.m[9]  * v.z + m.m[13] * v.w,
        m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z + m.m[14] * v.w,
        m.m[3] * v.x + m.m[7] * v.y + m.m[11] * v.z + m.m[15] * v.w
    };
}

mat4_t mat4_translate(vec3_t v) {
    mat4_t m = mat4_identity();
    m.m[12] = v.x;
    m.m[13] = v.y;
    m.m[14] = v.z;
    return m;
}

mat4_t mat4_scale(vec3_t v) {
    mat4_t m = {0};
    m.m[0] = v.x;
    m.m[5] = v.y;
    m.m[10] = v.z;
    m.m[15] = 1.0f;
    return m;
}

mat4_t mat4_rotate_x(float angle) {
    mat4_t m = mat4_identity();
    float c = cosf(angle);
    float s = sinf(angle);
    m.m[5] = c;
    m.m[6] = s;
    m.m[9] = -s;
    m.m[10] = c;
    return m;
}

mat4_t mat4_rotate_y(float angle) {
    mat4_t m = mat4_identity();
    float c = cosf(angle);
    float s = sinf(angle);
    m.m[0] = c;
    m.m[2] = -s;
    m.m[8] = s;
    m.m[10] = c;
    return m;
}

mat4_t mat4_rotate_z(float angle) {
    mat4_t m = mat4_identity();
    float c = cosf(angle);
    float s = sinf(angle);
    m.m[0] = c;
    m.m[1] = s;
    m.m[4] = -s;
    m.m[5] = c;
    return m;
}

mat4_t mat4_perspective(float fov, float aspect, float near, float far) {
    mat4_t m = {0};
    float tan_half_fov = tanf(fov * DEG_TO_RAD * 0.5f);

    m.m[0] = 1.0f / (aspect * tan_half_fov);
    m.m[5] = 1.0f / tan_half_fov;
    m.m[10] = -(far + near) / (far - near);
    m.m[11] = -1.0f;
    m.m[14] = -(2.0f * far * near) / (far - near);

    return m;
}

mat4_t mat4_ortho(float left, float right, float bottom, float top, float near, float far) {
    mat4_t m = mat4_identity();

    m.m[0] = 2.0f / (right - left);
    m.m[5] = 2.0f / (top - bottom);
    m.m[10] = -2.0f / (far - near);
    m.m[12] = -(right + left) / (right - left);
    m.m[13] = -(top + bottom) / (top - bottom);
    m.m[14] = -(far + near) / (far - near);

    return m;
}

mat4_t mat4_look_at(vec3_t eye, vec3_t center, vec3_t up) {
    vec3_t f = vec3_normalize(vec3_sub(center, eye));
    vec3_t s = vec3_normalize(vec3_cross(f, up));
    vec3_t u = vec3_cross(s, f);

    mat4_t m = mat4_identity();
    m.m[0] = s.x;
    m.m[4] = s.y;
    m.m[8] = s.z;
    m.m[1] = u.x;
    m.m[5] = u.y;
    m.m[9] = u.z;
    m.m[2] = -f.x;
    m.m[6] = -f.y;
    m.m[10] = -f.z;
    m.m[12] = -vec3_dot(s, eye);
    m.m[13] = -vec3_dot(u, eye);
    m.m[14] = vec3_dot(f, eye);

    return m;
}

// ============================================================================
// Utility functions
// ============================================================================

float clamp_f(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

float lerp_f(float a, float b, float t) {
    return a + (b - a) * t;
}

float smoothstep(float edge0, float edge1, float x) {
    float t = clamp_f((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

int float_to_int(float x) {
    return (int)(x + 0.5f);
}

uint8_t float_to_byte(float x) {
    int i = float_to_int(x * 255.0f);
    if (i < 0) return 0;
    if (i > 255) return 255;
    return (uint8_t)i;
}

// Fast inverse square root (Quake III algorithm)
float fast_inv_sqrt(float x) {
    union {
        float f;
        uint32_t i;
    } conv = {.f = x};
    conv.i = 0x5f3759df - (conv.i >> 1);
    conv.f *= 1.5f - (x * 0.5f * conv.f * conv.f);
    return conv.f;
}
