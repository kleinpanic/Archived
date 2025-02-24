#ifndef DISPLAY_H
#define DISPLAY_H

#include <X11/Xlib.h> // Include X11 headers for Display, Window, and GC types

void draw_icon(Display *d, Window w, GC gc);
void display_brightness(int brightness);

#endif
