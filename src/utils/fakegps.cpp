#include "config.h"
#include "HTTPClient.h"
#include "fakegps.h"

#include "hardware/gpsctl.h"
#include "hardware/wifictl.h"
#include "utils/json_psram_allocator.h"

static float len = 0;
static float lon = 0;

EventGroupHandle_t fakegps_event_handle = NULL;
TaskHanldle_t _fakegps_get_location_Task = NULL;
static volatile bool fakegps_wifi_enable = false;

void fakegps_get_location_Task(void * pvParameters);
bool fakegps_wifictl_event_cb(EventBits_t event, void *arg);
bool fakegps_gpsctl_event_cb(EventBits_t event, void *arg );
void fakegps_start_task(void);

void fakegps_setup( void ) {
    fakegps_event_handle = xEventGroupCreate();
    fakegps_wifi_enable = false;
    wifictl_register_cb(WIFICTL_CONNECT_IP | WIFICTL_DISCONNECT | WIFICTL_OFF, fakegps_wifictl_event_cb, "wifictl fakegps");
    gpsctl_register_cb(GPSCTL_ENABLE, fakegps_gpsctl_event_cb, "gpsctl fakegps");
}

double fakegps_get_last_lat(void ) {
    return (lat);
}

double fakegps_get_last_lon(void) {
    return (lon);
}

bool fakegps_gpsctl_event_cb(EventBits_t event, void *arg) {
    switch (event) {
        case GPSCTL_ENABLE:
            if(fakegps_wifi_enable) {
                fakegps_start_task();
            }
            break;
    }
    return (true);
}
