#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "history.h"
#include "config-option.h"

class ConfigServer {
public:
  ConfigServer(const char *apName, const char *password, const char *dnsName, ValueHistory &valueHistory, const ConfigOption &configOption);

  /**
   * Handle connections.
   *
   * Has to be called repeatedly.
   * Returns true when initialization is finished, false if it's not yet finished
   */
  bool handle();

private:
  bool initializationFinished;
  const char *dnsName;
  ESP8266WebServer server;
  ValueHistory &valueHistory;
  const ConfigOption &configOption;

  void handleRoot();
  void handleConfig();
  void handleConfigPost();
};
