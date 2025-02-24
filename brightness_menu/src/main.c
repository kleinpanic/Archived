#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gtk/gtk.h>
#include "display.h"

void run_cli();
void run_gui();

int main(int argc, char *argv[]) {
    int opt;
    int gui_mode = 0;

    // Parse command-line arguments
    while ((opt = getopt(argc, argv, "g")) != -1) {
        switch (opt) {
            case 'g':
                gui_mode = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s [-g]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (gui_mode) {
        run_gui(argc, argv);
    } else {
        run_cli();
    }

    return 0;
}

void run_cli() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    display_main_screen();

    endwin();
}

void run_gui(int argc, char *argv[]) {
    GtkWidget *window;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_window_set_title(GTK_WINDOW(window), "Screen Control GUI");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    gtk_widget_show_all(window);

    gtk_main();
}
