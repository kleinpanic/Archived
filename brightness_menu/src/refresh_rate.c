#include <ncurses.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#include <stdlib.h>
#include <stdio.h>
#include "refresh_rate.h"

void list_refresh_rates(XRRScreenResources *res, XRRCrtcInfo *crtc_info) {
    mvprintw(5, 0, "Available Refresh Rates:");
    int rate_count = 0;
    for (int i = 0; i < res->nmode; i++) {
        XRRModeInfo mode = res->modes[i];
        for (int j = 0; j < crtc_info->noutput; j++) {
            if (crtc_info->outputs[j] == mode.id) {
                double refresh_rate = (double)mode.dotClock / (mode.hTotal * mode.vTotal);
                mvprintw(6 + rate_count, 0, "%d: %.2f Hz", rate_count, refresh_rate);
                rate_count++;
            }
        }
    }
    refresh();
}

void set_refresh_rate() {
    // Placeholder for setting the refresh rate based on selected index
    mvprintw(7, 0, "Setting refresh rate feature coming soon...");
    refresh();
}

void adjust_refresh_rate() {
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

    XRRCrtcInfo *crtc_info = XRRGetCrtcInfo(dpy, res, res->crtcs[0]);
    if (crtc_info == NULL) {
        mvprintw(4, 0, "Unable to get CRTC info");
        XRRFreeScreenResources(res);
        XCloseDisplay(dpy);
        refresh();
        getch();
        return;
    }

    list_refresh_rates(res, crtc_info);

    mvprintw(6 + res->nmode, 0, "Enter the index of the desired refresh rate: ");
    refresh();

    echo();
    int rate_index;
    scanw("%d", &rate_index);
    noecho();

    set_refresh_rate();

    XRRFreeCrtcInfo(crtc_info);
    XRRFreeScreenResources(res);
    XCloseDisplay(dpy);

    getch();
}
