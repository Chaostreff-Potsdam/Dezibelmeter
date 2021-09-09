#pragma once

#include <stdint.h>

class ValueHistory {
public:
  ValueHistory();
  void addValue(float intensity);

  float value(uint16_t past);
private:
  float history[1000];
  uint16_t currentIndex;
  bool allInitialized;
};
