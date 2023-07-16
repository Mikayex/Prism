#include <glog/logging.h>
#include <stb_image_write.h>

#include <Ray.hpp>
#include <cstdlib>
#include <iostream>
#include <math/Functions.hpp>
#include <math/Vector3.hpp>
#include <memory>
#include <shapes/Sphere.hpp>

using namespace Prism;

constexpr Float aspectRatio = 16.0 / 9.0;
constexpr int width = 800;
constexpr int height = static_cast<int>(width / aspectRatio);
constexpr int numPixels = width * height;

namespace {
void logProgress(int x, int y) {
  LOG_EVERY_T(INFO, 0.25) << ((100.0 * y * width + x) / numPixels) << '%';
}

Vector3f rayColor(const Ray &ray, const Shape &shape) {
  if (const auto intersection = shape.intersect(ray); intersection.has_value() && *intersection > 0) {
    return {1, 0, 0};
  }

  const auto normalizedDirection = normalize(ray.d);
  const auto t = Float(0.5) * (normalizedDirection.y + Float(1));
  return mix(Vector3f(1.0, 1.0, 1.0), Vector3f(0.5, 0.7, 1.0), t);
}
}  // namespace

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  FLAGS_logtostdout = true;
  FLAGS_colorlogtostdout = true;

  DLOG(WARNING) << "Using debug build";

  const auto image_data = std::make_unique<std::uint8_t[]>(numPixels * 3);

  constexpr Float viewportHeight = 2.0;
  constexpr Float viewportWidth = aspectRatio * viewportHeight;
  constexpr Float focalLength = 1.0;

  auto origin = Point3f(0, 0, 0);
  auto horizontal = Vector3f(viewportWidth, 0, 0);
  auto vertical = Vector3f(0, viewportHeight, 0);
  auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3f(0, 0, focalLength);

  const auto sphere = Sphere(Point3f(0, 0, 1), 0.5);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      logProgress(x, y);

      const auto u = static_cast<Float>(x) / (width - 1);
      const auto v = Float(1) - static_cast<Float>(y) / (height - 1);

      Vector3f direction = Vector3f(origin, lower_left_corner + horizontal * u + vertical * v);
      Ray ray(origin, direction);

      const auto color = rayColor(ray, sphere);
      const auto color8 = color * 255.999;

      image_data[y * width * 3 + x * 3 + 0] = static_cast<std::uint8_t>(color8.x);
      image_data[y * width * 3 + x * 3 + 1] = static_cast<std::uint8_t>(color8.y);
      image_data[y * width * 3 + x * 3 + 2] = static_cast<std::uint8_t>(color8.z);
    }
  }

  LOG(INFO) << "Writing image";
  stbi_write_png("render.png", width, height, 3, image_data.get(), 0);

  return EXIT_SUCCESS;
}