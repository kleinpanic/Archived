#ifndef BRIGHTNESS_H
#define BRIGHTNESS_H

#define BRIGHTNESS_PATH "/sys/class/backlight/intel_backlight/brightness"
#define MAX_BRIGHTNESS_PATH "/sys/class/backlight/intel_backlight/max_brightness"

void set_brightness(int value, int raw);
void adjust_brightness(const char *direction);
void display_brightness(int brightness);
void save_brightness(void);
void restore_brightness(void);
void list_devices(void);
void print_info(void);
void set_min_value(int value);
void set_exponent(float exponent);
void set_device(char *device_name);
void set_class(char *class_name);
void enable_quiet_mode(void);
void enable_pretend_mode(void);

#endif
