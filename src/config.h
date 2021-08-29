#ifndef _CONFIG_H

    #if defined( LILYGO_WATCH_2020_V1 )
        #define WATCH_VERSION_NAME  "V1"
    #elif defined( LILYGO_WATCH_2020_V2 )
        #define WATCH_VERSION_NAME  "V2"
    #elif defined( LILYGO_WATCH_2020_V3 )
        #define WATCH_VERSION_NAME  "V3"
    #else
        #error "no ttgo t-watch 2020 version defined"
    #endif


    #define LILYGO_WATCH_LVGL                       
    #define TWATCH_USE_PSRAM_ALLOC_LVGL             

    #define ENABLE_WEBSERVER                        
    #define ENABLE_FTPSERVER                        
    
    #define USE_EXTENDED_CHARSET CHARSET_CYRILLIC

    #define __FIRMWARE__            "2021081201"

    #ifdef __cplusplus
        #include <LilyGoWatch.h>
        #define _CONFIG_H 
    #endif

#endif 