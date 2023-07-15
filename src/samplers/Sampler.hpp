#ifndef PRISM_SAMPLER_HPP
#define PRISM_SAMPLER_HPP
#include <cstdint>
#include <vector>

#include "../math/Point2.hpp"

namespace Prism {

class Sampler {
public:
  const std::int64_t samplesPerPixel;

  explicit Sampler(std::int64_t samplesPerPixel);
  virtual ~Sampler();

  virtual void startPixel(const Point2i &p);

  virtual float get1D() = 0;
  virtual Point2f get2D() = 0;

  [[nodiscard]] virtual int roundCount(int n) const {
    return n;
  }

  void request1DArray(int n);
  void request2DArray(int n);

  const float *get1DArray(int n);
  const Point2f *get2DArray(int n);

  virtual bool startNextSample();
  [[nodiscard]] virtual std::unique_ptr<Sampler> Clone(int seed) = 0;

  [[nodiscard]] std::int64_t CurrentSampleNumber() const {
    return currentPixelSampleIndex;
  }
  virtual bool setSampleNumber(std::int64_t sampleNum);

protected:
  Point2i currentPixel;
  std::int64_t currentPixelSampleIndex{};
  std::vector<int> samples1DArraySizes, samples2DArraySizes;
  std::vector<std::vector<float>> sampleArray1D;
  std::vector<std::vector<Point2f>> sampleArray2D;

private:
  std::size_t array1DOffset{}, array2DOffset{};
};

}  // namespace Prism

#endif  // PRISM_SAMPLER_HPP
