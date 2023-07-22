#ifndef PRISM_VULKANDEVICE_HPP
#define PRISM_VULKANDEVICE_HPP
#include <vulkan/vulkan.hpp>

namespace Prism {

class VulkanDevice {
public:
  explicit VulkanDevice(const vk::Instance& instance, vk::PhysicalDevice physicalDevice,
                        const std::vector<std::string>& extensions = {}, const std::vector<std::string>& layers = {});

  [[nodiscard]] vk::PhysicalDevice physicalDevice() const {
    return m_vkPhysicalDevice;
  }

  [[nodiscard]] const vk::Device& device() const {
    return *m_vkDevice;
  }

  [[nodiscard]] vk::Device& device() {
    return *m_vkDevice;
  }

  [[nodiscard]] std::uint32_t graphicsQueueFamily() const {
    return m_graphicsQueueFamily;
  }

  [[nodiscard]] vk::Queue graphicsQueue() const {
    return m_graphicsQueue;
  }

  [[nodiscard]] std::uint32_t presentQueueFamily() const {
    return m_presentQueueFamily;
  }

  [[nodiscard]] vk::Queue presentQueue() const {
    return m_presentQueue;
  }

private:
  vk::PhysicalDevice m_vkPhysicalDevice{};
  vk::UniqueDevice m_vkDevice{};

  std::uint32_t m_graphicsQueueFamily;
  vk::Queue m_graphicsQueue;
  std::uint32_t m_presentQueueFamily;
  vk::Queue m_presentQueue;
};

}  // namespace Prism

#endif  // PRISM_VULKANDEVICE_HPP
