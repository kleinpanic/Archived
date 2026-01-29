#include "earth/backend.h"

#ifdef USE_FBDEV

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>

typedef struct {
    int fd;
    uint8_t *mapped;
    size_t mapped_size;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
} fbdev_backend_data_t;

static void fbdev_init(backend_t *backend, config_t *config) {
    fbdev_backend_data_t *data = malloc(sizeof(fbdev_backend_data_t));

    // Open framebuffer device
    data->fd = open(config->fbdev_path, O_RDWR);
    if (data->fd < 0) {
        fprintf(stderr, "Failed to open framebuffer device: %s\n", config->fbdev_path);
        free(data);
        exit(1);
    }

    // Get variable screen info
    if (ioctl(data->fd, FBIOGET_VSCREENINFO, &data->vinfo) < 0) {
        fprintf(stderr, "Failed to get variable screen info\n");
        close(data->fd);
        free(data);
        exit(1);
    }

    // Get fixed screen info
    if (ioctl(data->fd, FBIOGET_FSCREENINFO, &data->finfo) < 0) {
        fprintf(stderr, "Failed to get fixed screen info\n");
        close(data->fd);
        free(data);
        exit(1);
    }

    // Map framebuffer
    data->mapped_size = data->finfo.smem_len;
    data->mapped = mmap(NULL, data->mapped_size, PROT_READ | PROT_WRITE,
                       MAP_SHARED, data->fd, 0);

    if (data->mapped == MAP_FAILED) {
        fprintf(stderr, "Failed to mmap framebuffer\n");
        close(data->fd);
        free(data);
        exit(1);
    }

    printf("Framebuffer: %dx%d, %d bpp\n",
           data->vinfo.xres, data->vinfo.yres, data->vinfo.bits_per_pixel);

    backend->user_data = data;
}

static void fbdev_present(backend_t *backend, framebuffer_t *fb) {
    fbdev_backend_data_t *data = (fbdev_backend_data_t *)backend->user_data;

    // Simple copy (assumes 32bpp RGBA format)
    int copy_width = (fb->width < (int)data->vinfo.xres) ? fb->width : (int)data->vinfo.xres;
    int copy_height = (fb->height < (int)data->vinfo.yres) ? fb->height : (int)data->vinfo.yres;

    for (int y = 0; y < copy_height; y++) {
        uint32_t *dst = (uint32_t *)(data->mapped + y * data->finfo.line_length);
        uint32_t *src = fb->pixels + y * fb->stride;
        memcpy(dst, src, copy_width * sizeof(uint32_t));
    }
}

static bool fbdev_should_quit(backend_t *backend) {
    (void)backend;
    return false; // Controlled by app
}

static void fbdev_shutdown(backend_t *backend) {
    fbdev_backend_data_t *data = (fbdev_backend_data_t *)backend->user_data;

    munmap(data->mapped, data->mapped_size);
    close(data->fd);
    free(data);
}

backend_t *backend_fbdev_create(void) {
    backend_t *backend = malloc(sizeof(backend_t));
    backend->init = fbdev_init;
    backend->present = fbdev_present;
    backend->should_quit = fbdev_should_quit;
    backend->shutdown = fbdev_shutdown;
    backend->user_data = NULL;
    return backend;
}

#endif // USE_FBDEV
