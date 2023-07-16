#ifndef PRISM_VECTOR3_HPP
#define PRISM_VECTOR3_HPP
#include <glog/logging.h>

#include <cmath>
#include <limits>

#include "Float.hpp"

namespace Prism {
template<typename T, bool IsPoint = false>
class Vector3 {
public:
  T x{}, y{}, z{};

  Vector3() = default;
  Vector3(T x, T y, T z) : x(x), y(y), z(z) {
    DCHECK(!hasNaN());
  }

  Vector3(Vector3<T, true> from, Vector3<T, true> to) : x(to.x - from.x), y(to.y - from.y), z(to.z - from.z) {
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

  Vector3<T, IsPoint> operator-() const {
    return Vector3<T, IsPoint>(-x, -y, -z);
  }

  Vector3<T, IsPoint> operator+(const Vector3<T, IsPoint> &v) {
    DCHECK(!v.hasNaN());
    return Vector3<T, IsPoint>(x + v.x, y + v.y, z + v.z);
  }

  Vector3<T, IsPoint> &operator+=(const Vector3<T, IsPoint> &v) {
    DCHECK(!v.hasNaN());
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  Vector3<T, IsPoint> operator-(const Vector3<T, IsPoint> &v) {
    DCHECK(!v.hasNaN());
    return Vector3<T, IsPoint>(x - v.x, y - v.y, z - v.z);
  }

  Vector3<T, IsPoint> &operator-=(const Vector3<T, IsPoint> &v) {
    DCHECK(!v.hasNaN());
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  template<typename U>
  Vector3<T, IsPoint> operator*(U s) const {
    return Vector3<T, IsPoint>(s * x, s * y, s * z);
  }

  template<typename U>
  Vector3<T, IsPoint> &operator*=(U s) {
    DCHECK(!isNaN(s));
    x *= s;
    y *= s;
    z *= s;
    return *this;
  }
  template<typename U>
  Vector3<T, IsPoint> operator/(U f) const {
    CHECK_NE(f, 0);
    const Float inv = static_cast<Float>(1) / f;
    return Vector3<T, IsPoint>(x * inv, y * inv, z * inv);
  }

  template<typename U>
  Vector3<T> &operator/=(U f) {
    CHECK_NE(f, 0);
    const Float inv = static_cast<Float>(1) / f;
    x *= inv;
    y *= inv;
    z *= inv;
    return *this;
  }

  [[nodiscard]] bool hasNaN() const {
    if constexpr (std::numeric_limits<T>::has_quiet_NaN) {
      return std::isnan(x) || std::isnan(y) || std::isnan(z);
    } else {
      return false;
    }
  }

  [[nodiscard]] Float lengthSquared() const {
    return x * x + y * y + z * z;
  }

  [[nodiscard]] Float length() const {
    return std::sqrt(lengthSquared());
  }
};

extern template class Vector3<Float, false>;
extern template class Vector3<Float, true>;
extern template class Vector3<int, false>;
extern template class Vector3<int, true>;

template<typename T>
using Point3 = Vector3<T, true>;

template<typename T>
inline Point3<T> operator+(const Point3<T> &p, const Vector3<T> &v) {
  DCHECK(!p.hasNaN());
  DCHECK(!v.hasNaN());
  return Point3<T>(p.x + v.x, p.y + v.y, p.z + v.z);
}

template<typename T>
inline Point3<T> &operator+=(Point3<T> &p, const Vector3<T> &v) {
  DCHECK(!p.hasNaN());
  DCHECK(!v.hasNaN());
  p.x += v.x;
  p.y += v.y;
  p.z += v.z;
  return p;
}

template<typename T>
inline Point3<T> operator-(const Point3<T> &p, const Vector3<T> &v) {
  DCHECK(!p.hasNaN());
  DCHECK(!v.hasNaN());
  return Point3<T>(p.x - v.x, p.y - v.y, p.z - v.z);
}

template<typename T>
inline Point3<T> &operator-=(Point3<T> &p, const Vector3<T> &v) {
  DCHECK(!p.hasNaN());
  DCHECK(!v.hasNaN());
  p.x -= v.x;
  p.y -= v.y;
  p.z -= v.z;
  return p;
}

template<typename T>
[[nodiscard]] Vector3<T> normalize(const Vector3<T> &v) {
  return v / v.length();
}

using Point3f = Point3<Float>;
using Point3i = Point3<int>;

using Vector3f = Vector3<Float, false>;
using Vector3i = Vector3<int, false>;

}  // namespace Prism

#endif  // PRISM_VECTOR3_HPP
