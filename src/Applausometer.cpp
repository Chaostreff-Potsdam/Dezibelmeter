#include "Applausometer.h"

Applausometer::Applausometer(Adafruit_NeoPixel &neoPixel)
: coleringSchema(Schema2),
  showMode(DifferentColor),
  neoPixel(neoPixel) {
}

uint32_t Applausometer::colorFromIntensity(float intensity) {
  switch (coleringSchema) {
    case Schema1: {
      return neoPixel.Color(0, 0, 0);
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
      return neoPixel.Color(0, 0,0);
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

void addSelectOption(std::ostream &stream, const char *value, const char *humanName, bool selected) {
  stream << "<option value=\"";
  stream << value;
  stream << "\"";
  if (selected) {
    stream << " selected";
  }
  stream << ">";
  stream << humanName;
  stream << "</option>";
}

void addSelect(std::ostream &stream, const char *name, const char *humanName, std::function<void(std::ostream&)> fun) {
  stream << "<label for=\"" << name << "\">" << humanName << "</label>";
  stream << "<select name=\"" << name << "\">";
  fun(stream);
  stream << "</select><br />";
}


ConfigOption Applausometer::configuration() {
  auto toFormFragment = [this](std::ostream &out) {
    addSelect(out, "color", "Color", [this](std::ostream &out) {
      addSelectOption(out, "color1", "Black", coleringSchema == Schema1);
      addSelectOption(out, "color2", "Green to Red", coleringSchema == Schema2);
      addSelectOption(out, "color3", "Green", coleringSchema == Schema3);
    });

    addSelect(out, "mode", "Mode", [this](std::ostream &out) {
      addSelectOption(out, "all", "Show all", showMode == All);
      addSelectOption(out, "sameColor", "Same color", showMode == SameColor);
      addSelectOption(out, "differentColor", "Different color", showMode == DifferentColor);
      addSelectOption(out, "delay", "Delay", showMode == Delay);
    });
  };

  auto changeConfiguration = [this](std::function<String(String)> args) {
    String color = args("color");
    String mode = args("mode");

    Serial.println(color);
    Serial.println(mode);

    if (color == "color1")
      coleringSchema = Schema1;
    else if (color == "color2")
      coleringSchema = Schema2;
    else if (color == "color3")
      coleringSchema = Schema3;

    if (mode == "all")
      showMode = All;
    else if (mode == "sameColor")
      showMode = SameColor;
    else if (mode == "differentColor")
      showMode = DifferentColor;
    else if (mode == "delay")
      showMode = Delay;
  };

  return ConfigOption(toFormFragment, changeConfiguration);
;
}
