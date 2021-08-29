#include "config.h"
#include "HTTPClient.h"
#include "fakegps.h"

#include "hardware/gpsctl.h"
#include "hardware/wifictl.h"
#include "utils/json_psram_allocator.h"

static float lat = 0;
static float lon = 0;

EventGroupHandle_t fakegps_event_handle = NULL;
TaskHandle_t _fakegps_get_location_Task = NULL;
static volatile bool fakegps_wifi_enable = false;

void fakegps_get_location_Task(void * pvParameters);
bool fakegps_wifictl_event_cb(EventBits_t event, void *arg);
bool fakegps_gpsctl_event_cb(EventBits_t event, void *arg);
void fakegps_start_task(void);

void fakegps_setup(void) {
    fakegps_event_handle = xEventGroupCreate();
    fakegps_wifi_enable = false;
    wifictl_register_cb( WIFICTL_CONNECT_IP | WIFICTL_DISCONNECT | WIFICTL_OFF, fakegps_wifictl_event_cb, "wifictl fakegps");
    gpsctl_register_cb( GPSCTL_ENABLE, fakegps_gpsctl_event_cb, "gpsctl fakegps");
}

double fakegps_get_last_lat(void) {
    return( lat );
}

double fakegps_get_last_lon(void) {
    return( lon );
}

bool fakegps_gpsctl_event_cb(EventBits_t event, void *arg) {
    switch (event) {
        case GPSCTL_ENABLE:
            if( fakegps_wifi_enable ) {
                fakegps_start_task();
            }
            break;
    }
    return(true);
}

bool fakegps_wifictl_event_cb(EventBits_t event, void *arg) {
    switch(event) {
        case WIFICTL_CONNECT_IP:
            fakegps_wifi_enable = true;
            fakegps_start_task();
            break;
        case WIFICTL_DISCONNECT:
            fakegps_wifi_enable = false;
            break;
        case WIFICTL_OFF:
            fakegps_wifi_enable = false;
            break;
    }
    return(true);
}

void fakegps_start_task(void) {
    if ( xEventGroupGetBits( fakegps_event_handle ) & FAKEGPS_SYNC_REQUEST ) {
        return;
    }
    else {
        if ( gpsctl_get_gps_over_ip() && gpsctl_get_autoon() ) {
            xEventGroupSetBits( fakegps_event_handle, FAKEGPS_SYNC_REQUEST );
            xTaskCreate(    fakegps_get_location_Task,
                            "fakegps update Task",
                            5000,
                            NULL,
                            1,
                            &_fakegps_get_location_Task );
        }
    }
}

void fakegps_get_location_Task(void * pvParameters) {
    log_i("start fakegps task, heap: %d", ESP.getFreeHeap() );

    if ( xEventGroupGetBits( fakegps_event_handle ) & FAKEGPS_SYNC_REQUEST ) {
        int httpcode = -1;
        HTTPClient fakegps_client;

        fakegps_client.setUserAgent( "ESP32-" __FIRMWARE__ );
        fakegps_client.begin( GEOIP_URL );
        httpcode = fakegps_client.GET();

        if ( httpcode != 200 ) {
            log_e("HTTPClient error %d", httpcode );
        }
        else {
            SpiRamJsonDocument doc( fakegps_client.getSize() * 4 );

            DeserializationError error = deserializeJson( doc, fakegps_client.getStream() );
            if (error) {
                log_e("fakegps deserializeJson() failed: %s", error.c_str() );
            }
            else {
                if ( doc["lat"] && doc["lon"] ) {
                    lat = doc["lat"].as<float>();
                    lon = doc["lon"].as<float>();
                    log_i("lat: %f, lon:%f", lat, lon );
                    gpsctl_set_location( lat, lon, 0, GPS_SOURCE_IP, true );
                }
            }
            doc.clear();
        }
        fakegps_client.end();
    }
    xEventGroupClearBits(fakegps_event_handle, FAKEGPS_SYNC_REQUEST);
    log_i("finish fakegps task, heap: %d", ESP.getFreeHeap() );
    vTaskDelete(NULL);
}
    
