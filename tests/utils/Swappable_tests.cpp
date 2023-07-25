#include <gtest/gtest.h>

#include <utils/Swappable.hpp>

using namespace Prism::Utils;

TEST(SwappableTest, Empty) {
  Swappable<int> s;
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);

  EXPECT_THROW((void)s.currentIndex(), std::logic_error);
  EXPECT_THROW((void)s.current(), std::logic_error);

  const Swappable<int> constS = s;
  EXPECT_TRUE(constS.empty());
  EXPECT_EQ(constS.size(), 0);

  EXPECT_THROW((void)constS.currentIndex(), std::logic_error);
  EXPECT_THROW((void)constS.current(), std::logic_error);
}

TEST(SwappableTest, ClonedValues) {
  constexpr std::size_t size = 3;

  Swappable<int> s(size, 42);
  EXPECT_FALSE(s.empty());
  EXPECT_EQ(s.size(), size);

  EXPECT_EQ(s.currentIndex(), 0);
  EXPECT_EQ(s.current(), 42);

  for (std::size_t i = 0; i < size * 2; ++i) {
    EXPECT_EQ(s.currentIndex(), i % size);
    EXPECT_EQ(s.current(), 42);
    s.next();
  }

  const Swappable<int> constS = s;
  EXPECT_FALSE(constS.empty());
  EXPECT_EQ(constS.size(), size);

  EXPECT_EQ(constS.currentIndex(), 0);
  EXPECT_EQ(constS.current(), 42);
}

TEST(SwappableTest, GeneratedValues) {
  constexpr std::size_t size = 3;

  Swappable<int> s(size, [](std::size_t index) { return static_cast<int>(index); });
  EXPECT_FALSE(s.empty());
  EXPECT_EQ(s.size(), size);

  for (std::size_t i = 0; i < size * 2; ++i) {
    EXPECT_EQ(s.currentIndex(), i % size);
    EXPECT_EQ(s.current(), i % size);
    s.next();
  }

  const Swappable<int> constS = s;
  EXPECT_FALSE(constS.empty());
  EXPECT_EQ(constS.size(), size);

  EXPECT_EQ(constS.currentIndex(), 0);
  EXPECT_EQ(constS.current(), 0);
}