#include <stdio.h>
#include <stdlib.h>
#include "service_control.h"

int disable_services() {
    return system("nmcli radio wifi off && rfkill block bluetooth");
}

int enable_services() {
    return system("nmcli radio wifi on && rfkill unblock bluetooth");
}
