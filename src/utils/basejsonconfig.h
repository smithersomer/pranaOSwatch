#ifndef BASEJSONCONFIG_H_
#define BASEJSONCONFIG_H_

#define MAX_CONFIG_FILE_NAME_LENGTH 32

// includes
#include "ArduinoJson.h"

class BaseJsonConfig {
public:
  BaseJsonConfig(const char* configFileName);

  bool load();

  bool load( uint32_t size );

  bool save();

  bool save( uint32_t size );

  void debugPrint();
  
protected:

  virtual bool onSave(JsonDocument& document) = 0;
  virtual bool onLoad(JsonDocument& document) = 0;
  virtual bool onDefault( void ) = 0;
  virtual size_t getJsonBufferSize() { return 8192; }

protected:
  char fileName[MAX_CONFIG_FILE_NAME_LENGTH];
  bool prettyJson = true;
};

#endif