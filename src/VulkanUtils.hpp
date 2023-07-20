#pragma once
#include <string>
#include <unordered_set>
#include <vector>
#include <vulkan/vulkan.hpp>

template<typename T, typename Dispatch, std::enable_if_t<vk::isVulkanHandleType<T>::value, bool> = true>
bool operator==(vk::UniqueHandle<T, Dispatch>& handle, std::nullptr_t nullptrValue) {
  return !handle;
}

template<typename T, typename = std::enable_if_t<vk::isVulkanHandleType<T>::value>>
bool operator==(T& handle, std::nullptr_t nullptrValue) {
  return !handle;
}

namespace Prism {
std::vector<const char*> toCharArrays(const std::unordered_set<std::string>& strings);
}  // namespace Prism