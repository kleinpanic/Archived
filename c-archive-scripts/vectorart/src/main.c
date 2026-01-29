#include <stdio.h>
#include <math.h>
#include <unistd.h> // for usleep

#define WIDTH 80
#define HEIGHT 24
#define CUBE_SIZE 1.0
#define SLEEP_TIME 66666 // Microseconds, roughly 1/15th of a second

// Structure to hold a 3D point
typedef struct {
    float x, y, z;
} Point3D;

// Cube vertices
Point3D cube[] = {
    {-CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE},
    { CUBE_SIZE, -CUBE_SIZE, -CUBE_SIZE},
    { CUBE_SIZE,  CUBE_SIZE, -CUBE_SIZE},
    {-CUBE_SIZE,  CUBE_SIZE, -CUBE_SIZE},
    {-CUBE_SIZE, -CUBE_SIZE,  CUBE_SIZE},
    { CUBE_SIZE, -CUBE_SIZE,  CUBE_SIZE},
    { CUBE_SIZE,  CUBE_SIZE,  CUBE_SIZE},
    {-CUBE_SIZE,  CUBE_SIZE,  CUBE_SIZE}
};

// Cube edges
int edges[][2] = {
    {0, 1}, {1, 3}, {3, 2}, {2, 0}, // Bottom face
    {4, 5}, {5, 7}, {7, 6}, {6, 4}, // Top face
    {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Connecting edges
};

// Rotation function
void rotateY(Point3D* p, float angle) {
    float x = p->x * cos(angle) + p->z * sin(angle);
    float z = -p->x * sin(angle) + p->z * cos(angle);
    p->x = x;
    p->z = z;
}

// Projection function
void project(Point3D p, int* x2D, int* y2D, float xm, float ym, float dist) {
    *x2D = (int)(WIDTH / 2 + xm * p.x / (p.z + dist));
    *y2D = (int)(HEIGHT / 2 + ym * p.y / (p.z + dist));
}

// Function to render the cube
void renderCube(Point3D* points, float angle, float xm, float ym, float dist) {
    Point3D transformed[8];
    int proj[8][2];

    // Rotate and project each vertex
    for (int i = 0; i < 8; i++) {
        transformed[i] = points[i];
        rotateY(&transformed[i], angle); // Rotate around Y-axis
        project(transformed[i], &proj[i][0], &proj[i][1], xm, ym, dist);
    }

    // Clear the terminal
    printf("\033[H\033[J");

    // Create an empty screen buffer
    char screen[HEIGHT][WIDTH];
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            screen[i][j] = ' ';
        }
    }

    // Draw edges of the cube
    for (int i = 0; i < 12; ++i) {
        int x1 = proj[edges[i][0]][0];
        int y1 = proj[edges[i][0]][1];
        int x2 = proj[edges[i][1]][0];
        int y2 = proj[edges[i][1]][1];

        // Simple line interpolation for drawing lines between vertices
        for (int j = 0; j <= 10; ++j) {
            int x = x1 + j * (x2 - x1) / 10;
            int y = y1 + j * (y2 - y1) / 10;
            if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
                screen[y][x] = '*';
            }
        }
    }

    // Output the screen buffer
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            putchar(screen[i][j]);
        }
        putchar('\n');
    }
}

int main() {
    float angle = 0;
    float s = 0.1; // sine for initial rotation
    float c = sqrt(1 - s * s); // cosine
    float ym = HEIGHT / 3; // Y magnification
    float xm = 2 * ym; // X magnification
    float dist = 2.0; // Distance for perspective projection

    while (1) {
        renderCube(cube, angle, xm, ym, dist);
        angle += 0.05; // Update rotation angle
        usleep(SLEEP_TIME); // Pause for 1/15th of a second
    }

    return 0;
}

