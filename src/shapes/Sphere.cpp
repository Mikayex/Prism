#include "Sphere.hpp"

#include <Ray.hpp>

namespace Prism {
Sphere::Sphere(const Point3f& center, Float radius) : center(center), radius(radius) {}

std::optional<Float> Sphere::intersect(const Ray& ray) const {
  const Vector3f oc(ray.o, center);
  const Float a = ray.d.lengthSquared();
  const Float b = 2 * (ray.d.x * oc.x + ray.d.y * oc.y + ray.d.z * oc.z);
  const Float c = oc.lengthSquared() - radius * radius;

  const auto discriminant = b * b - 4 * a * c;
  if (discriminant < 0) return {};  // No hit

  const auto sqrtd = std::sqrt(discriminant);
  return (-b - sqrtd) / (Float(2) * a);
}
}  // namespace Prism