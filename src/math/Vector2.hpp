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

  Vector2<T, IsPoint> operator-() const {
    return Vector2<T, IsPoint>(-x, -y);
  }

  Vector2<T, IsPoint> operator+(const Vector2<T, IsPoint> &v) {
    DCHECK(!v.hasNaN());
    return Vector2<T, IsPoint>(x + v.x, y + v.y);
  }

  Vector2<T, IsPoint> &operator+=(const Vector2<T, IsPoint> &v) {
    DCHECK(!v.hasNaN());
    x += v.x;
    y += v.y;
    return *this;
  }

  Vector2<T, IsPoint> operator-(const Vector2<T, IsPoint> &v) {
    DCHECK(!v.hasNaN());
    return Vector2<T, IsPoint>(x - v.x, y - v.y);
  }

  Vector2<T, IsPoint> &operator-=(const Vector2<T, IsPoint> &v) {
    DCHECK(!v.hasNaN());
    x -= v.x;
    y -= v.y;
    return *this;
  }

  template<typename U>
  Vector2<T, IsPoint> operator*(U s) const {
    return Vector2<T, IsPoint>(s * x, s * y);
  }

  template<typename U>
  Vector2<T, IsPoint> &operator*=(U s) {
    DCHECK(!isNaN(s));
    x *= s;
    y *= s;
    return *this;
  }
  template<typename U>
  Vector2<T, IsPoint> operator/(U f) const {
    CHECK_NE(f, 0);
    const Float inv = static_cast<Float>(1) / f;
    return Vector2<T, IsPoint>(x * inv, y * inv);
  }

  template<typename U>
  Vector2<T> &operator/=(U f) {
    CHECK_NE(f, 0);
    const Float inv = static_cast<Float>(1) / f;
    x *= inv;
    y *= inv;
    return *this;
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

template<typename T>
inline Point2<T> operator+(const Point2<T> &p, const Vector2<T> &v) {
  DCHECK(!p.hasNaN());
  DCHECK(!v.hasNaN());
  return Point2<T>(p.x + v.x, p.y + v.y);
}

template<typename T>
inline Point2<T> &operator+=(Point2<T> &p, const Vector2<T> &v) {
  DCHECK(!p.hasNaN());
  DCHECK(!v.hasNaN());
  p.x += v.x;
  p.y += v.y;
  return p;
}

template<typename T>
inline Point2<T> operator-(const Point2<T> &p, const Vector2<T> &v) {
  DCHECK(!p.hasNaN());
  DCHECK(!v.hasNaN());
  return Point2<T>(p.x - v.x, p.y - v.y);
}

template<typename T>
inline Point2<T> &operator-=(Point2<T> &p, const Vector2<T> &v) {
  DCHECK(!p.hasNaN());
  DCHECK(!v.hasNaN());
  p.x -= v.x;
  p.y -= v.y;
  return p;
}

using Point2f = Vector2<Float, true>;
using Point2i = Vector2<int, true>;

using Vector2f = Vector2<Float, false>;
using Vector2i = Vector2<int, false>;
}  // namespace Prism

#endif  // PRISM_VECTOR2_HPP
