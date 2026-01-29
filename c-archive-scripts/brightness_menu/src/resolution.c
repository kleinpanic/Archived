#include <ncurses.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <stdlib.h>
#include <stdio.h>
#include "resolution.h"

void list_resolutions(XRRScreenResources *res) {
    mvprintw(5, 0, "Available Resolutions:");
    for (int i = 0; i < res->nmode; i++) {
        XRRModeInfo mode = res->modes[i];
        mvprintw(6 + i, 0, "%d: %dx%d", i, mode.width, mode.height);
    }
    refresh();
}

void set_resolution(Display *dpy, Window root, XRRScreenResources *res, int mode_index) {
    if (mode_index < 0 || mode_index >= res->nmode) {
        mvprintw(7 + res->nmode, 0, "Invalid mode index");
        return;
    }

    XRRModeInfo mode = res->modes[mode_index];
    XRRSetScreenSize(dpy, root, mode.width, mode.height, mode.width, mode.height);
    XRRSetCrtcConfig(dpy, res, res->crtcs[0], CurrentTime, 0, 0, mode.id, RR_Rotate_0, NULL, 0);

    mvprintw(7 + res->nmode, 0, "Resolution set to %dx%d", mode.width, mode.height);
    refresh();
}

void adjust_resolution() {
    Display *dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
        mvprintw(4, 0, "Unable to open X display");
        refresh();
        getch();
        return;
    }

    Window root = DefaultRootWindow(dpy);
    XRRScreenResources *res = XRRGetScreenResources(dpy, root);
    if (res == NULL) {
        mvprintw(4, 0, "Unable to get screen resources");
        XCloseDisplay(dpy);
        refresh();
        getch();
        return;
    }

    list_resolutions(res);

    mvprintw(6 + res->nmode, 0, "Enter the index of the desired resolution: ");
    refresh();

    echo();
    int mode_index;
    scanw("%d", &mode_index);
    noecho();

    set_resolution(dpy, root, res, mode_index);

    XRRFreeScreenResources(res);
    XCloseDisplay(dpy);

    getch();
}
