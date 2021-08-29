#include "basejsonconfig.h"
#include "json_psram_allocator.h"
#include "alloc.h"
#include <FS.h>

BaseJsonConfig::BaseJsonConfig(const char* configFileName) {
  if (configFileName[0] == '/')
    strlcpy(fileName, configFileName, MAX_CONFIG_FILE_NAME_LENGTH);
  else
  {
    fileName[0] = '/';
    strlcpy(fileName+1, configFileName, MAX_CONFIG_FILE_NAME_LENGTH);
  }
}

bool BaseJsonConfig::load() {
    bool result = false;

    if ( SPIFFS.exists(fileName) ) {

        fs::File file = SPIFFS.open(fileName, FILE_READ);

        if (!file) {
            log_e("Can't open file: %s!", fileName);
        }
        else {
            int filesize = file.size();
            SpiRamJsonDocument doc( filesize*4 );
            DeserializationError error = deserializeJson( doc, file );

            if ( error || filesize == 0 ) {
                log_e("json config deserializeJson() failed: %s, file: %s", error.c_str(), fileName );
            }
            else {
                log_i("json config deserializeJson() success: %s, file: %s", error.c_str(), fileName );
                result = onLoad(doc);
            }
            doc.clear();
        }
        file.close();
    }

    if ( !result ) {
        log_i("reading json failed, call defaults, file: %s", fileName );
        result = onDefault();
    }

    return result;
}

bool BaseJsonConfig::load( uint32_t size ) {
    bool result = false;

    if ( SPIFFS.exists(fileName) ) {

        fs::File file = SPIFFS.open(fileName, FILE_READ);

        if (!file) {
            log_e("Can't open file: %s!", fileName);
        }
        else {

            SpiRamJsonDocument doc( size );
            DeserializationError error = deserializeJson( doc, file );

            if ( error || size == 0 ) {
                log_e("json config deserializeJson() failed: %s, file: %s", error.c_str(), fileName );
            }
            else {
                log_i("json config deserializeJson() success: %s, file: %s", error.c_str(), fileName );
                result = onLoad(doc);
            }
            doc.clear();
        }
        file.close();
    }

    if ( !result ) {
        log_i("reading json failed, call defaults, file: %s", fileName );
        result = onDefault();
    }

    return result;
}

bool BaseJsonConfig::save( uint32_t size ) {
    bool result = false;
    fs::File file = SPIFFS.open(fileName, FILE_WRITE );

    if (!file) {
        log_e("Can't open file: %s!", fileName);
    }
    else {
        SpiRamJsonDocument doc( size );
        result = onSave(doc);

        if ( doc.overflowed() ) {
            log_e("json to large, some value are missing. use another size");
        }
        
        size_t outSize = 0;
        if (prettyJson)
        outSize = serializeJsonPretty(doc, file);
        else
        outSize = serializeJson(doc, file);

        if (result == true && outSize == 0) {
            log_e("Failed to write config file %s", fileName);
            result = false;
        }
        else {
            log_i("json config serializeJson() success: %s", fileName );
        }
        
        doc.clear();
    }
    file.close();

    return result;
}

bool BaseJsonConfig::save() {
    bool result = false;
    fs::File file = SPIFFS.open(fileName, FILE_WRITE );

    if (!file) {
        log_e("Can't open file: %s!", fileName);
    }
    else {
        auto size = getJsonBufferSize();
        SpiRamJsonDocument doc( size );
        result = onSave(doc);

        if ( doc.overflowed() ) {
            log_e("json to large, some value are missing. use doc.save( uint32_t size )");
        }
        
        size_t outSize = 0;
        if (prettyJson)
        outSize = serializeJsonPretty(doc, file);
        else
        outSize = serializeJson(doc, file);

        if (result == true && outSize == 0) {
            log_e("Failed to write config file %s", fileName);
            result = false;
        }
        else {
            log_i("json config serializeJson() success: %s", fileName );            
        }
        
        doc.clear();
    }
    file.close();

    return result;
}

void BaseJsonConfig::debugPrint() {
    auto size = getJsonBufferSize();
    SpiRamJsonDocument doc(size);
    bool result = onSave(doc);
    if ( result ) {
        serializeJsonPretty(doc, Serial);
    }
}