#pragma once

#include <stdint.h>
#include <Adafruit_NeoPixel.h>
#include "config-option.h"

enum ColoringSchema {
  // Single green color
  Schema1,
  // Changing from green to red
  Schema2,
  Schema3
};

enum ShowMode {
  // Test mode to show all pixels, regardless of sound
  All,
  // All in the color of the highest
  SameColor,
  // All in color of their position
  DifferentColor,
  // Position is delay
  Delay
};

class Applausometer {
public:
  Applausometer(Adafruit_NeoPixel &neoPixel);

  void setIntensity(float intensity);

  ConfigOption configuration();

  ColoringSchema coleringSchema;
  ShowMode showMode;

private:
  uint32_t colorFromIntensity(float);

  Adafruit_NeoPixel &neoPixel;
};
