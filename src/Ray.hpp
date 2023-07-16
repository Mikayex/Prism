#ifndef PRISM_RAY_HPP
#define PRISM_RAY_HPP
#include <climits>

#include "math/Float.hpp"
#include "math/Vector3.hpp"

namespace Prism {

class Ray {
public:
  Point3f o;
  Vector3f d;
  float tMax = infinity;

  Ray();
  Ray(const Point3f& o, const Vector3f& d, float tMax = infinity);

  [[nodiscard]] Point3f operator()(Float t) const;
};

}  // namespace Prism

#endif  // PRISM_RAY_HPP
