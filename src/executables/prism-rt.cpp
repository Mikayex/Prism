#include <glog/logging.h>
#include <stb_image_write.h>

#include <cstdlib>
#include <iostream>
#include <memory>

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  FLAGS_logtostdout = true;
  FLAGS_colorlogtostdout = true;

  DLOG(WARNING) << "Using debug build";

  constexpr int width = 800;
  constexpr int height = 600;

  const auto image_data = std::make_unique<std::uint8_t[]>(width * height * 3);

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      const auto r = static_cast<float>(x) / (width - 1);
      const auto g = static_cast<float>(y) / (height - 1);
      const auto b = 0.25;

      const auto ir = static_cast<std::uint8_t>(255.999 * r);
      const auto ig = static_cast<std::uint8_t>(255.999 * g);
      const auto ib = static_cast<std::uint8_t>(255.999 * b);

      image_data[y * width * 3 + x * 3 + 0] = ir;
      image_data[y * width * 3 + x * 3 + 1] = ig;
      image_data[y * width * 3 + x * 3 + 2] = ib;
    }
  }
  stbi_write_png("render.png", width, height, 3, image_data.get(), 0);

  return EXIT_SUCCESS;
}