#include "history.h"

ValueHistory::ValueHistory() :
  currentIndex(0),
  allInitialized(false) {
}

void ValueHistory::addValue(float intensity) {
  history[currentIndex] = intensity;
  currentIndex += 1;
  if (currentIndex >= 1000) {
      currentIndex = 0;
      allInitialized = true;
    }
}

float ValueHistory::value(uint16_t past) {
  if (currentIndex <= past) {
      if (allInitialized) {
          return history[currentIndex + 1000 - past - 1];
        }
      else {
          return -1.0;
        }
    }
  else {
      return history[currentIndex - past - 1];
    }
}
