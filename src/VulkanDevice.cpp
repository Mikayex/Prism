#include "VulkanDevice.hpp"

#include <GLFW/glfw3.h>
#include <glog/logging.h>

#include <optional>

#include "VulkanUtils.hpp"

namespace Prism {
namespace {
struct QueueFamilies {
  std::uint32_t presentQueueFamilyIndex;
  std::uint32_t graphicsQueueFamilyIndex;
};

QueueFamilies findQueueFamilies(const vk::Instance& instance, const vk::PhysicalDevice& physicalDevice) {
  std::optional<std::uint32_t> presentQueue, graphicsQueue;
  const auto queueProperties = physicalDevice.getQueueFamilyProperties();

  for (std::uint32_t i = 0; i < queueProperties.size(); ++i) {
    const auto& queue = queueProperties[i];
    if (queue.queueFlags & vk::QueueFlagBits::eGraphics)
      graphicsQueue = i;

    if (glfwGetPhysicalDevicePresentationSupport(instance, physicalDevice, i))
      presentQueue = i;

    if (graphicsQueue.has_value() && graphicsQueue == presentQueue)
      break;  // Found a queue that supports both
  }

  CHECK(presentQueue.has_value());
  CHECK(graphicsQueue.has_value());

  return {*presentQueue, *graphicsQueue};
}
}  // namespace
VulkanDevice::VulkanDevice(const vk::Instance& instance, vk::PhysicalDevice physicalDevice,
                           const std::vector<std::string>& extensions, const std::vector<std::string>& layers)
    : m_vkPhysicalDevice(physicalDevice) {
  const auto queueFamilies = findQueueFamilies(instance, m_vkPhysicalDevice);

  std::array<float, 1> queuePriorities = {1.0f};
  std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

  queueCreateInfos.emplace_back(vk::DeviceQueueCreateFlags{}, queueFamilies.graphicsQueueFamilyIndex, queuePriorities);

  if (queueFamilies.presentQueueFamilyIndex != queueFamilies.graphicsQueueFamilyIndex)
    queueCreateInfos.emplace_back(vk::DeviceQueueCreateFlags{}, queueFamilies.presentQueueFamilyIndex, queuePriorities);

  const std::unordered_set<std::string> uniqueExtensions{extensions.cbegin(), extensions.cend()};
  const auto extensionsList = toCharArrays(uniqueExtensions);

  const std::unordered_set<std::string> uniqueLayers{layers.cbegin(), layers.cend()};
  const auto layersList = toCharArrays(uniqueLayers);

  const vk::DeviceCreateInfo deviceCreateInfo{
      {},                // flags
      queueCreateInfos,  // queueCreateInfos
      layersList,        // layers
      extensionsList,    // extensions
  };

  m_vkDevice = m_vkPhysicalDevice.createDeviceUnique(deviceCreateInfo);
  CHECK_NOTNULL(m_vkDevice);

  m_graphicsQueue = m_vkDevice->getQueue(queueFamilies.graphicsQueueFamilyIndex, 0);
  if (queueFamilies.graphicsQueueFamilyIndex == queueFamilies.presentQueueFamilyIndex)
    m_presentQueue = m_graphicsQueue;
  else
    m_presentQueue = m_vkDevice->getQueue(queueFamilies.presentQueueFamilyIndex, 0);

  CHECK_NOTNULL(m_graphicsQueue);
  CHECK_NOTNULL(m_presentQueue);
}
}  // namespace Prism