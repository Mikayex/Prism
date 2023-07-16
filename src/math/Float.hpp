#ifndef PRISM_FLOAT_HPP
#define PRISM_FLOAT_HPP
#include <limits>

namespace Prism {
using Float = float;

constexpr float infinity = std::numeric_limits<Float>::infinity();
}  // namespace Prism

#endif  // PRISM_FLOAT_HPP
