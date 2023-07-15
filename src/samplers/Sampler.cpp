#include "Sampler.hpp"

#include <glog/logging.h>

namespace Prism {
Sampler::~Sampler() = default;
Sampler::Sampler(std::int64_t samplesPerPixel) : samplesPerPixel{samplesPerPixel} {}

void Sampler::startPixel(const Point2i& p) {
  currentPixel = p;
  currentPixelSampleIndex = 0;

  array1DOffset = 0;
  array2DOffset = 0;
}

void Sampler::request1DArray(int n) {
  CHECK_EQ(roundCount(n), n);
  samples1DArraySizes.push_back(n);
  sampleArray1D.emplace_back(n * samplesPerPixel);
}

void Sampler::request2DArray(int n) {
  CHECK_EQ(roundCount(n), n);
  samples2DArraySizes.push_back(n);
  sampleArray2D.emplace_back(n * samplesPerPixel);
}

const float* Sampler::get1DArray(int n) {
  if (array1DOffset >= sampleArray1D.size()) return nullptr;
  CHECK_EQ(samples1DArraySizes[array1DOffset], n);
  CHECK_LT(currentPixelSampleIndex, samplesPerPixel);
  return &sampleArray1D[array1DOffset++][currentPixelSampleIndex * n];
}

const Point2f* Sampler::get2DArray(int n) {
  if (array2DOffset == sampleArray2D.size()) return nullptr;
  CHECK_EQ(samples2DArraySizes[array2DOffset], n);
  CHECK_LT(currentPixelSampleIndex, samplesPerPixel);
  return &sampleArray2D[array2DOffset++][currentPixelSampleIndex * n];
}

bool Sampler::startNextSample() {
  array1DOffset = 0;
  array2DOffset = 0;
  return ++currentPixelSampleIndex < samplesPerPixel;
}

bool Sampler::setSampleNumber(std::int64_t sampleNum) {
  array1DOffset = 0;
  array2DOffset = 0;
  currentPixelSampleIndex = sampleNum;
  return currentPixelSampleIndex < samplesPerPixel;
}
}  // namespace Prism