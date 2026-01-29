#include "earth/math.h"
#include <stdio.h>
#include <math.h>

static int tests_run = 0;
static int tests_passed = 0;

#define TEST(name) \
    static void test_##name(void); \
    static void run_test_##name(void) { \
        printf("  Running test_%s... ", #name); \
        tests_run++; \
        test_##name(); \
        tests_passed++; \
        printf("OK\n"); \
    } \
    static void test_##name(void)

#define ASSERT_FLOAT_EQ(a, b, epsilon) \
    if (fabsf((a) - (b)) > (epsilon)) { \
        printf("FAIL\n    Expected %f, got %f\n", (float)(b), (float)(a)); \
        return; \
    }

#define ASSERT_TRUE(expr) \
    if (!(expr)) { \
        printf("FAIL\n    Expression failed: %s\n", #expr); \
        return; \
    }

// Tests

TEST(vec3_add) {
    vec3_t a = vec3(1, 2, 3);
    vec3_t b = vec3(4, 5, 6);
    vec3_t c = vec3_add(a, b);

    ASSERT_FLOAT_EQ(c.x, 5, 0.001f);
    ASSERT_FLOAT_EQ(c.y, 7, 0.001f);
    ASSERT_FLOAT_EQ(c.z, 9, 0.001f);
}

TEST(vec3_normalize) {
    vec3_t v = vec3(3, 4, 0);
    vec3_t n = vec3_normalize(v);

    float len = vec3_length(n);
    ASSERT_FLOAT_EQ(len, 1.0f, 0.001f);
    ASSERT_FLOAT_EQ(n.x, 0.6f, 0.001f);
    ASSERT_FLOAT_EQ(n.y, 0.8f, 0.001f);
}

TEST(vec3_cross) {
    vec3_t x_axis = vec3(1, 0, 0);
    vec3_t y_axis = vec3(0, 1, 0);
    vec3_t z_axis = vec3_cross(x_axis, y_axis);

    ASSERT_FLOAT_EQ(z_axis.x, 0, 0.001f);
    ASSERT_FLOAT_EQ(z_axis.y, 0, 0.001f);
    ASSERT_FLOAT_EQ(z_axis.z, 1, 0.001f);
}

TEST(mat4_identity) {
    mat4_t m = mat4_identity();

    ASSERT_FLOAT_EQ(m.m[0], 1, 0.001f);
    ASSERT_FLOAT_EQ(m.m[5], 1, 0.001f);
    ASSERT_FLOAT_EQ(m.m[10], 1, 0.001f);
    ASSERT_FLOAT_EQ(m.m[15], 1, 0.001f);
    ASSERT_FLOAT_EQ(m.m[1], 0, 0.001f);
}

TEST(mat4_translate) {
    mat4_t m = mat4_translate(vec3(10, 20, 30));
    vec4_t v = vec4(0, 0, 0, 1);
    vec4_t result = mat4_mul_vec4(m, v);

    ASSERT_FLOAT_EQ(result.x, 10, 0.001f);
    ASSERT_FLOAT_EQ(result.y, 20, 0.001f);
    ASSERT_FLOAT_EQ(result.z, 30, 0.001f);
    ASSERT_FLOAT_EQ(result.w, 1, 0.001f);
}

TEST(mat4_rotate_z) {
    mat4_t m = mat4_rotate_z(PI / 2.0f); // 90 degrees
    vec4_t v = vec4(1, 0, 0, 1);
    vec4_t result = mat4_mul_vec4(m, v);

    ASSERT_FLOAT_EQ(result.x, 0, 0.001f);
    ASSERT_FLOAT_EQ(result.y, 1, 0.001f);
    ASSERT_FLOAT_EQ(result.z, 0, 0.001f);
}

TEST(clamp_f) {
    ASSERT_FLOAT_EQ(clamp_f(0.5f, 0.0f, 1.0f), 0.5f, 0.001f);
    ASSERT_FLOAT_EQ(clamp_f(-1.0f, 0.0f, 1.0f), 0.0f, 0.001f);
    ASSERT_FLOAT_EQ(clamp_f(2.0f, 0.0f, 1.0f), 1.0f, 0.001f);
}

TEST(lerp_f) {
    ASSERT_FLOAT_EQ(lerp_f(0, 10, 0.5f), 5, 0.001f);
    ASSERT_FLOAT_EQ(lerp_f(0, 10, 0.0f), 0, 0.001f);
    ASSERT_FLOAT_EQ(lerp_f(0, 10, 1.0f), 10, 0.001f);
}

void run_math_tests(void) {
    printf("\nRunning math tests:\n");

    run_test_vec3_add();
    run_test_vec3_normalize();
    run_test_vec3_cross();
    run_test_mat4_identity();
    run_test_mat4_translate();
    run_test_mat4_rotate_z();
    run_test_clamp_f();
    run_test_lerp_f();

    printf("\nMath tests: %d/%d passed\n", tests_passed, tests_run);
}

int main(void) {
    run_math_tests();
    return (tests_run == tests_passed) ? 0 : 1;
}
