#include <gtest/gtest.h>

#include <math/Vector3.hpp>

using namespace Prism;

template<typename T>
class Vector3Test : public testing::Test {};

using VectorTypes = ::testing::Types<Float, int>;
TYPED_TEST_SUITE(Vector3Test, VectorTypes);

TYPED_TEST(Vector3Test, Constructors) {
  using T = TypeParam;
  using V = Vector3<T>;

  const V defaultVector;
  EXPECT_EQ(defaultVector.x, T{});
  EXPECT_EQ(defaultVector.y, T{});
  EXPECT_EQ(defaultVector.z, T{});

  const V v(-1, 2, 3);
  EXPECT_EQ(v.x, -1);
  EXPECT_EQ(v.y, 2);
  EXPECT_EQ(v.z, 3);

  const V v2(v);
  EXPECT_EQ(v2.x, -1);
  EXPECT_EQ(v2.y, 2);
  EXPECT_EQ(v2.z, 3);
}

TYPED_TEST(Vector3Test, hasNaN) {
  using T = TypeParam;
  using V = Vector3<T>;

  const V defaultVector;
  EXPECT_FALSE(defaultVector.hasNaN());

  const V v(-1, 2, 3);
  EXPECT_FALSE(v.hasNaN());

  V v2 = v;
  v2.x = std::numeric_limits<T>::quiet_NaN();
  EXPECT_EQ(v2.hasNaN(), std::numeric_limits<T>::has_quiet_NaN);

  v2 = v;
  v2.y = std::numeric_limits<T>::quiet_NaN();
  EXPECT_EQ(v2.hasNaN(), std::numeric_limits<T>::has_quiet_NaN);

  v2 = v;
  v2.z = std::numeric_limits<T>::quiet_NaN();
  EXPECT_EQ(v2.hasNaN(), std::numeric_limits<T>::has_quiet_NaN);
}