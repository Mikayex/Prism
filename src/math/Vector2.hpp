#ifndef PRISM_VECTOR2_HPP
#define PRISM_VECTOR2_HPP
#include <cassert>
#include <cmath>
#include <limits>

namespace Prism {
template<typename T>
class Vector2 {
public:
  T x{}, y{};

  Vector2() = default;
  Vector2(T x, T y) : x(x), y(y) {
    assert(!hasNaN());
  }

  Vector2(const Vector2<T> &v) : x(v.x), y(v.y) {
    assert(!v.hasNaN());
  }

  Vector2<T> &operator=(const Vector2<T> &v) {
    assert(!v.hasNaN());
    x = v.x;
    y = v.y;
    return *this;
  }

  T operator[](int i) const {
    assert(i >= 0 && i <= 1);
    if (i == 0) return x;
    return y;
  }

  T &operator[](int i) {
    assert(i >= 0 && i <= 1);
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

extern template class Vector2<float>;
extern template class Vector2<int>;

using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;

}  // namespace Prism

#endif  // PRISM_VECTOR2_HPP
