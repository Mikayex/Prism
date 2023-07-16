#include <gtest/gtest.h>

#include <math/Vector3.hpp>

using namespace Prism;

template<typename T>
class Point3Test : public testing::Test {};

using PointTypes = ::testing::Types<float, int>;
TYPED_TEST_SUITE(Point3Test, PointTypes);

TYPED_TEST(Point3Test, Constructors) {
  using T = TypeParam;
  using P = Point3<T>;

  const P defaultPoint;
  EXPECT_EQ(defaultPoint.x, T{});
  EXPECT_EQ(defaultPoint.y, T{});

  const P p(-1, 2, 3);
  EXPECT_EQ(p.x, -1);
  EXPECT_EQ(p.y, 2);

  const P p2(p);  // Copy constructor
  EXPECT_EQ(p2.x, -1);
  EXPECT_EQ(p2.y, 2);
}

TYPED_TEST(Point3Test, hasNaN) {
  using T = TypeParam;
  using P = Point3<T>;

  const P defaultPoint;
  EXPECT_FALSE(defaultPoint.hasNaN());

  const P p(-1, 2, 3);
  EXPECT_FALSE(p.hasNaN());

  P p2 = p;
  p2.x = std::numeric_limits<T>::quiet_NaN();
  EXPECT_EQ(p2.hasNaN(), std::numeric_limits<T>::has_quiet_NaN);

  p2 = p;
  p2.y = std::numeric_limits<T>::quiet_NaN();
  EXPECT_EQ(p2.hasNaN(), std::numeric_limits<T>::has_quiet_NaN);
}