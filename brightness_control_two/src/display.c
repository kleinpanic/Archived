#include "display.h"
#include "brightness.h"
#include "utils.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void draw_icon(Display *d, Window w, GC gc) {
    XDrawArc(d, w, gc, 70, 10, 60, 60, 0, 360 * 64);
    for (int i = 0; i < 8; ++i) {
        int x1 = 100 + 30 * cos(i * M_PI / 4);
        int y1 = 40 + 30 * sin(i * M_PI / 4);
        int x2 = 100 + 45 * cos(i * M_PI / 4);
        int y2 = 40 + 45 * sin(i * M_PI / 4);
        XDrawLine(d, w, gc, x1, y1, x2, y2);
    }
}

void display_brightness(int brightness) {
    Display *d;
    Window w;
    XEvent e;
    int screen;
    unsigned int display_width, display_height;
    int width = 200, height = 120;
    int sections = 10;
    int graph_height = 10;

    if ((d = XOpenDisplay(NULL)) == NULL) {
        exit(1);
    }

    screen = DefaultScreen(d);
    display_width = DisplayWidth(d, screen);
    display_height = DisplayHeight(d, screen);

    w = XCreateSimpleWindow(d, RootWindow(d, screen), (display_width - width) / 2, (display_height - height) / 2, width, height, 1,
                            BlackPixel(d, screen), WhitePixel(d, screen));
    XSetWindowBackground(d, w, 0xD3D3D3);
    XStoreName(d, w, "BrightnessControl");
    XClassHint *classHint = XAllocClassHint();
    classHint->res_name = "brightnesscontrol";
    classHint->res_class = "BrightnessControl";
    XSetClassHint(d, w, classHint);
    XFree(classHint);

    XSelectInput(d, w, ExposureMask | KeyPressMask | StructureNotifyMask);
    XMapWindow(d, w);

    GC gc = XCreateGC(d, w, 0, NULL);
    GC bg_gc = XCreateGC(d, w, 0, NULL);
    XSetForeground(d, gc, BlackPixel(d, screen));
    XSetBackground(d, bg_gc, WhitePixel(d, screen));
    XSetLineAttributes(d, gc, 3, LineSolid, CapButt, JoinMiter);

    FILE *max_brightness_file = fopen(MAX_BRIGHTNESS_PATH, "r");
    int max_brightness;
    fscanf(max_brightness_file, "%d", &max_brightness);
    fclose(max_brightness_file);

    while (1) {
        XNextEvent(d, &e);
        if (e.type == MapNotify) {
            break;
        }
    }

    draw_icon(d, w, gc);

    XSetForeground(d, bg_gc, 0xA9A9A9);
    XFillRectangle(d, w, bg_gc, 10, 100, 180, graph_height);

    int filled_sections = (int)((brightness / (double)max_brightness) * sections);
    for (int i = 0; i < sections; ++i) {
        if (i < filled_sections) {
            XSetForeground(d, gc, 0xFFFFFF);
        } else {
            XSetForeground(d, gc, 0xA9A9A9);
        }
        XFillRectangle(d, w, gc, 10 + i * (180 / sections), 100, 180 / sections - 2, graph_height);
    }

    XFlush(d);
    sleep(1);
    XDestroyWindow(d, w);
    XCloseDisplay(d);
}
