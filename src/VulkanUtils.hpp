#pragma once
#include <vulkan/vulkan.hpp>

template<typename Type, typename Dispatch>
bool operator==(vk::UniqueHandle<Type, Dispatch>& handle, std::nullptr_t nullptrValue) {
  return !handle;
}
