#include "config-server.h"

#include <ESP8266mDNS.h>

#include <sstream>

ConfigServer::ConfigServer(const char* apName, const char* password, const char* dnsName, ValueHistory &valueHistory, const ConfigOption &configOption) :
  initializationFinished(false),
  dnsName(dnsName),
  server(80),
  valueHistory(valueHistory),
  configOption(configOption) {
    WiFi.begin(apName, password);
}


void ConfigServer::handleRoot() {
  std::stringstream s;

  s << "Current: " << valueHistory.value(0) << '\n';
  for (int i = 1; i < 20; ++i) {
    s << valueHistory.value(i*5) << ", ";
  }
  s << valueHistory.value(20*5) << "\n";
  server.send(200, "text/plain", s.str().c_str());
}

void ConfigServer::handleConfig() {
  std::stringstream result;

  result << "<html><head><title>Configuration</title></head><body>";

  result << "<form action=\"/configR\" method=\"post\">";

  configOption.toFormFragment(result);

  result << "<input type=\"submit\" value=\"Change\" />";

  result << "</form>";

  result << "</body></html>";

  server.send(200, "text/html", result.str().c_str());
}

void ConfigServer::handleConfigPost() {
  configOption.changeConfiguration([this](String argName) {
    return server.arg(argName);
  });
  handleConfig();
}

bool ConfigServer::handle() {
  if (initializationFinished) {
    Serial.println("Initialization finished");
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());
    server.handleClient();
    return true;
  } else {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Connecting to WiFi");
      return false;
    } else {
      Serial.println("Succesfully connected");

      if (dnsName != nullptr) {
        MDNS.begin(dnsName);
        dnsName = nullptr;

        server.on("/", std::bind(&ConfigServer::handleRoot, this));
        server.on("/config", std::bind(&ConfigServer::handleConfig, this));
        server.on("/configR", HTTP_POST, std::bind(&ConfigServer::handleConfigPost, this));
        server.onNotFound([this]() { server.send(404, "text/plain", "404: Not found"); });

        server.begin();
        initializationFinished = true;
      }
      return true;
    }
  }
}
