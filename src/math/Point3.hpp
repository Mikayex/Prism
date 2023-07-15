#ifndef PRISM_POINT3_HPP
#define PRISM_POINT3_HPP
#include <glog/logging.h>

#include <cmath>
#include <limits>

namespace Prism {

template<typename T>
class Point3 {
public:
  T x{}, y{};

  Point3() = default;
  Point3(T x, T y) : x(x), y(y) {
    DCHECK(!hasNaN());
  }

  Point3(const Point3<T> &v) : x(v.x), y(v.y) {
    DCHECK(!v.hasNaN());
  }

  Point3<T> &operator=(const Point3<T> &v) {
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

extern template class Point3<float>;
extern template class Point3<int>;

using Point3f = Point3<float>;
using Point3i = Point3<int>;

}  // namespace Prism

#endif  // PRISM_POINT3_HPP
