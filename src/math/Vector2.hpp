#ifndef PRISM_VECTOR2_HPP
#define PRISM_VECTOR2_HPP
#include <glog/logging.h>

#include <cmath>
#include <limits>

#include "Float.hpp"

namespace Prism {
template<typename T, bool IsPoint = false>
class Vector2 {
public:
  T x{}, y{};

  Vector2() = default;
  Vector2(T x, T y) : x(x), y(y) {
    DCHECK(!hasNaN());
  }

  Vector2(const Vector2<T, IsPoint> &v) : x(v.x), y(v.y) {
    DCHECK(!v.hasNaN());
  }

  Vector2<T, IsPoint> &operator=(const Vector2<T, IsPoint> &v) {
    DCHECK(!v.hasNaN());
    x = v.x;
    y = v.y;
    return *this;
  }

  T operator[](int i) const {
    DCHECK(i >= 0 && i <= 1);
    if (i == 0) return x;
    return y;
  }

  T &operator[](int i) {
    DCHECK(i >= 0 && i <= 1);
    if (i == 0) return x;
    return y;
  }

  [[nodiscard]] bool hasNaN() const {
    if constexpr (std::numeric_limits<T>::has_quiet_NaN) {
      return std::isnan(x) || std::isnan(y);
    } else {
      return false;
    }
  }
};

extern template class Vector2<Float, false>;
extern template class Vector2<Float, true>;
extern template class Vector2<int, false>;
extern template class Vector2<int, true>;

template<typename T>
using Point2 = Vector2<T, true>;

using Point2f = Vector2<Float, true>;
using Point2i = Vector2<int, true>;

using Vector2f = Vector2<Float, false>;
using Vector2i = Vector2<int, false>;
}  // namespace Prism

#endif  // PRISM_VECTOR2_HPP
