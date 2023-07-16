#ifndef PRISM_FUNCTIONS_HPP
#define PRISM_FUNCTIONS_HPP
#include "Float.hpp"

namespace Prism {
template<typename T>
T mix(const T& a, const T& b, Float t) {
  return a * (Float(1) - t) + b * t;
}
}  // namespace Prism

#endif  // PRISM_FUNCTIONS_HPP
