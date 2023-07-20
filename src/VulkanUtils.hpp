#pragma once
#include <string>
#include <type_traits>
#include <unordered_set>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace vk {
template<typename T, typename Dispatch>
bool operator==(const UniqueHandle<T, Dispatch>& handle, std::nullptr_t) {
  return !handle;
}

template<typename T, typename = std::enable_if_t<isVulkanHandleType<T>::value>>
bool operator==(const T& handle, std::nullptr_t) {
  return !handle;
}
}  // namespace vk

namespace Prism {
std::vector<const char*> toCharArrays(const std::unordered_set<std::string>& strings);
}  // namespace Prism