#ifndef PRISM_SWAPPABLE_HPP
#define PRISM_SWAPPABLE_HPP
#include <functional>
#include <stdexcept>
#include <vector>

namespace Prism::Utils {

template<typename T>
class Swappable {
public:
  Swappable() = default;

  Swappable(std::size_t size, std::function<T(std::size_t index)> generator) {
    m_data.reserve(size);
    for (std::size_t i = 0; i < size; ++i)
      m_data.push_back(generator(i));
  }

  Swappable(std::size_t size, const T& value) : m_data(size, value) {}

  [[nodiscard]] bool empty() const {
    return m_data.empty();
  }

  [[nodiscard]] std::size_t size() const {
    return m_data.size();
  }

  [[nodiscard]] std::size_t currentIndex() const {
    if (empty())
      throw std::logic_error("Empty swappable");

    return m_currentIndex;
  }

  [[nodiscard]] const T& current() const {
    if (empty())
      throw std::logic_error("Empty swappable");

    return m_data[m_currentIndex];
  }

  [[nodiscard]] T& current() {
    if (empty())
      throw std::logic_error("Empty swappable");

    return m_data[m_currentIndex];
  }

  void next() {
    if (empty())
      throw std::logic_error("Empty swappable");

    m_currentIndex = (m_currentIndex + 1) % size();
  }

private:
  std::size_t m_currentIndex = 0;
  std::vector<T> m_data;
};

}  // namespace Prism::Utils

#endif  // PRISM_SWAPPABLE_HPP
