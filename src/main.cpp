#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "Applausometer.h"
#include "config-server.h"
#include "history.h"

#include "config.h"

Adafruit_NeoPixel pixels(Config::ledCount, D1, NEO_GRB + NEO_KHZ800);

Applausometer applausometer(pixels);

ValueHistory valueHistory;

ConfigServer configServer(Config::wifiSsid, Config::wifiPassword, "applausometer", valueHistory);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Startup");

  pinMode(D1, OUTPUT);

  pinMode(A0, INPUT);

  pixels.begin();
}

void loop() {
  configServer.handle();
  int read = analogRead(A0);

  valueHistory.addValue(read);

  Serial.println(read, DEC);

  float voltageValue = read / 1024.0 * 3.2;;
  float voltageValueSub = voltageValue - 0.6;

  float dbValue = voltageValue * 50.0;
  Serial.print(dbValue, 1);
  Serial.println(" dBA");

  float intensity = voltageValueSub / 2.6;

  applausometer.setIntensity(intensity);

  delay(50);
}


