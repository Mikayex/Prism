#ifndef PRISM_POINT2_HPP
#define PRISM_POINT2_HPP
#include <glog/logging.h>

#include <cmath>
#include <limits>

namespace Prism {

template<typename T>
class Point2 {
public:
  T x{}, y{};

  Point2() = default;
  Point2(T x, T y) : x(x), y(y) {
    DCHECK(!hasNaN());
  }

  Point2(const Point2<T> &v) : x(v.x), y(v.y) {
    DCHECK(!v.hasNaN());
  }

  Point2<T> &operator=(const Point2<T> &v) {
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

extern template class Point2<float>;
extern template class Point2<int>;

using Point2f = Point2<float>;
using Point2i = Point2<int>;

}  // namespace Prism

#endif  // PRISM_POINT2_HPP
