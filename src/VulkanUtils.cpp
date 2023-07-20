#include "VulkanUtils.hpp"

namespace Prism {
std::vector<const char*> toCharArrays(const std::unordered_set<std::string>& strings) {
  std::vector<const char*> charArrays;
  charArrays.reserve(strings.size());
  for (const auto& s : strings)
    charArrays.push_back(s.c_str());

  return charArrays;
}
}  // namespace Prism
