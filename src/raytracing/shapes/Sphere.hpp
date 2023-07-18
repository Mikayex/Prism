#ifndef PRISM_SPHERE_HPP
#define PRISM_SPHERE_HPP
#include "Shape.hpp"
#include "math/Vector3.hpp"

namespace Prism {

class Sphere final : public Shape {
public:
  Sphere(const Point3f& center, Float radius);
  [[nodiscard]] std::optional<Float> intersect(const Ray& ray) const override;

private:
  const Point3f center;
  const Float radius;
};

}  // namespace Prism

#endif  // PRISM_SPHERE_HPP
