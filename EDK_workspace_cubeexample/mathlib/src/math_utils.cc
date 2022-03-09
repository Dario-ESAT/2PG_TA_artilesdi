// @author Enrique Gil <gilte@esat-alumni.com>
// @file math_utils class implementation

#include "math_utils.h"

namespace matlib {

inline MathUtils::MathUtils() {

}

inline MathUtils::MathUtils(const MathUtils& copy) {

}

inline MathUtils::~MathUtils() {

}

float MathUtils::Clamp(float value, float minVal, float maxVal) {
  if(value > maxVal) {
    return maxVal;
  }

  if(value < minVal) {
    return minVal;
  }

  return value;
}

}