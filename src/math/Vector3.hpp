#ifndef PRISM_VECTOR3_HPP
#define PRISM_VECTOR3_HPP
#include <cassert>
#include <cmath>
#include <limits>

namespace Prism {
template<typename T>
class Vector3 {
public:
  T x{}, y{}, z{};

  Vector3() = default;
  Vector3(T x, T y, T z) : x(x), y(y), z(z) {
    assert(!hasNaN());
  }

  Vector3(const Vector3<T> &v) : x(v.x), y(v.y), z(v.z) {
    assert(!v.hasNaN());
  }

  Vector3<T> &operator=(const Vector3<T> &v) {
    assert(!v.hasNaN());
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
  }

  T operator[](int i) const {
    assert(i >= 0 && i <= 2);
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
  }

  T &operator[](int i) {
    assert(i >= 0 && i <= 2);
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
  }

  [[nodiscard]] bool hasNaN() const {
    if constexpr (std::numeric_limits<T>::has_quiet_NaN) {
      return std::isnan(x) || std::isnan(y) || std::isnan(z);
    } else {
      return false;
    }
  }
};

extern template class Vector3<float>;
extern template class Vector3<int>;

using Vector3f = Vector3<float>;
using Vector3i = Vector3<int>;

}  // namespace Prism

#endif  // PRISM_VECTOR3_HPP
