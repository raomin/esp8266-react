#ifndef LightBrokerSettingsService_h
#define LightBrokerSettingsService_h

#include <HttpEndpoint.h>
#include <FSPersistence.h>

#define LIGHT_BROKER_SETTINGS_FILE "/config/brokerSettings.json"
#define LIGHT_BROKER_SETTINGS_PATH "/rest/brokerSettings"

static String defaultDeviceValue(String prefix = "") {
#ifdef ESP32
  return prefix + String((unsigned long)ESP.getEfuseMac(), HEX);
#elif defined(ESP8266)
  return prefix + String(ESP.getChipId(), HEX);
#endif
}

class LightBrokerSettings {
 public:
  String mqttPath;
  String name;
  String uniqueId;

  static void serialize(LightBrokerSettings& settings, JsonObject& root) {
    root["mqtt_path"] = settings.mqttPath;
    root["name"] = settings.name;
    root["unique_id"] = settings.uniqueId;
  }

  static void deserialize(JsonObject& root, LightBrokerSettings& settings) {
    settings.mqttPath = root["mqtt_path"] | defaultDeviceValue("homeassistant/light/");
    settings.name = root["name"] | defaultDeviceValue("light-");
    settings.uniqueId = root["unique_id"] | defaultDeviceValue("light-");
  }
};

class LightBrokerSettingsService : public StatefulService<LightBrokerSettings> {
 public:
  LightBrokerSettingsService(AsyncWebServer* server, FS* fs, SecurityManager* securityManager);
  void begin();

 private:
  HttpEndpoint<LightBrokerSettings> _httpEndpoint;
  FSPersistence<LightBrokerSettings> _fsPersistence;
};

#endif  // end LightBrokerSettingsService_h
