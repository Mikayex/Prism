#include "Ray.hpp"

namespace Prism {
Ray::Ray() = default;
Ray::Ray(const Point3f& o, const Vector3f& d, float tMax) : o(o), d(d), tMax(tMax) {}
Point3f Ray::operator()(Float t) const {
  return o + d * t;
}
}  // namespace Prism