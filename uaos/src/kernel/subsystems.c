#include "../kernel/kernel.h"

// Stubs for kernel subsystems that are not fully implemented yet

void process_init(void) {
    // Stub
}

void interrupts_init(void) {
    // Stub
}

// App registration logic
struct app_entry {
    char name[32];
    app_entry_t entry;
};

static struct app_entry registered_apps[10];
static int app_count = 0;

void app_register(const char* name, app_entry_t entry) {
    if (app_count < 10) {
        // Manual string copy to avoid dependency issues if string.h is tricky
        int i = 0;
        while(name[i] && i < 31) {
            registered_apps[app_count].name[i] = name[i];
            i++;
        }
        registered_apps[app_count].name[i] = '\0';
        registered_apps[app_count].entry = entry;
        app_count++;
    }
}
