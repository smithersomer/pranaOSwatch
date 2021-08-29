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


}

void BaseJsonConfig::debugPrint() {
    auto size = getJsonBufferSize();
    SpiRamJsonDocument doc(size);
    bool result = onSave(doc);
    if (result) {
        serializerJsonPretty(doc, Serial);
    }
}