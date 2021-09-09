#include "Applausometer.h"

Applausometer::Applausometer(Adafruit_NeoPixel &neoPixel)
: coleringSchema(Schema2),
  showMode(DifferentColor),
  neoPixel(neoPixel) {
}

uint32_t Applausometer::colorFromIntensity(float intensity) {
  switch (coleringSchema) {
    case Schema1: {
      return neoPixel.Color(0xCD / 2, 0x00, 0xCD /2 );
  }
    case Schema2: {
      uint16_t hue;
      if (intensity > 0.5)
        hue = (1.0 - intensity) * 65536 / 10;
      else if (intensity < 0.4)
        hue = (1.0 - intensity) * 65536  / 3;
      else {
        // Interpolate between the two
        hue = 0.6 * 65536 / 3 + (0.5 * 65536 / 10 - 0.6 * 65536 / 10) * (intensity - 0.4) * 10;
      }
      return Adafruit_NeoPixel::ColorHSV(hue, 255, 40);
    }
    case Schema3: {
      return neoPixel.Color(0, 40, 0);
    }
    default:
      return neoPixel.Color(0,0,0);
  };
}

void Applausometer::setIntensity(float intensity) {
  uint16_t height = intensity * neoPixel.numPixels();

  for (uint16_t i = 0; i < neoPixel.numPixels(); ++i) {
    uint32_t color;
    switch (showMode) {
      case All:
        color = colorFromIntensity((float)i / 65.0);
        break;
      case SameColor:
        color = colorFromIntensity(intensity);
        break;
      case DifferentColor:
      if (i < height) {
         color = colorFromIntensity((float)i / 65.0);
      } else {
        color = neoPixel.Color(0, 0, 0);
      }
      break;
    case Delay:
        //push all pixels down
        if (i < neoPixel.numPixels() - 1) {
          color = neoPixel.getPixelColor(i + 1);
        } else {
          color = colorFromIntensity(intensity);
        }
        break;
    default:
      color = neoPixel.Color(0, 0, 128);
    }
    neoPixel.setPixelColor(i, color);
  }
  neoPixel.show();
}
