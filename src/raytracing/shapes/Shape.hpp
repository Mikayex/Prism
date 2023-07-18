#ifndef PRISM_SHAPE_HPP
#define PRISM_SHAPE_HPP
#include <optional>

#include "math/Float.hpp"

namespace Prism {
class Ray;

class Shape {
public:
  virtual ~Shape();

  [[nodiscard]] virtual std::optional<Float> intersect(const Ray& ray) const = 0;
};

}  // namespace Prism

#endif  // PRISM_SHAPE_HPP
