#ifndef PRISM_VECTOR3_HPP
#define PRISM_VECTOR3_HPP
#include <glog/logging.h>

#include <cmath>
#include <limits>

namespace Prism {
template<typename T, bool IsPoint = false>
class Vector3 {
public:
  T x{}, y{}, z{};

  Vector3() = default;
  Vector3(T x, T y, T z) : x(x), y(y), z(z) {
    DCHECK(!hasNaN());
  }

  Vector3(const Vector3<T, IsPoint> &v) : x(v.x), y(v.y), z(v.z) {
    DCHECK(!v.hasNaN());
  }

  Vector3<T, IsPoint> &operator=(const Vector3<T, IsPoint> &v) {
    DCHECK(!v.hasNaN());
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
  }

  T operator[](int i) const {
    DCHECK(i >= 0 && i <= 2);
    if (i == 0) return x;
    if (i == 1) return y;
    return z;
  }

  T &operator[](int i) {
    DCHECK(i >= 0 && i <= 2);
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

extern template class Vector3<float, false>;
extern template class Vector3<float, true>;
extern template class Vector3<int, false>;
extern template class Vector3<int, true>;

template<typename T>
using Point3 = Vector3<T, true>;

using Point3f = Point3<float>;
using Point3i = Point3<int>;

using Vector3f = Vector3<float, false>;
using Vector3i = Vector3<int, false>;

}  // namespace Prism

#endif  // PRISM_VECTOR3_HPP
