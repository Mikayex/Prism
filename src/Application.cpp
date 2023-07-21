#include "Application.hpp"

#include <glog/logging.h>

#include <algorithm>
#include <optional>
#include <unordered_set>
#include <vector>

#include "VulkanDevice.hpp"
#include "VulkanUtils.hpp"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace Prism {
namespace {
void glfwErrorCallback(int error, const char* description) {
  LOG(ERROR) << "Glfw Error " << error << ": " << description;
}

vk::UniqueInstance createVulkanInstance(const std::vector<std::string>& extensions = {},
                                        const std::vector<std::string>& layers = {}) {
  const std::unordered_set<std::string> uniqueExtensions{extensions.cbegin(), extensions.cend()};
  const auto extensionsList = toCharArrays(uniqueExtensions);

  const std::unordered_set<std::string> uniqueLayers{layers.cbegin(), layers.cend()};
  const auto layersList = toCharArrays(uniqueLayers);

  const vk::InstanceCreateInfo createInfo{
      {},              // flags
      nullptr,         // applicationInfo
      layersList,      // layers
      extensionsList,  // extensions
  };

  auto instance = vk::createInstanceUnique(createInfo);
  VULKAN_HPP_DEFAULT_DISPATCHER.init(*instance);

  return std::move(instance);
}

vk::PhysicalDevice selectVulkanDevice(vk::Instance instance) {
  const auto physicalDevices = instance.enumeratePhysicalDevices();
  CHECK(!physicalDevices.empty());

  std::optional<vk::PhysicalDevice> bestGpu;

  for (const auto& gpu : physicalDevices) {
    const auto gpuProperties = gpu.getProperties();
    const auto queueFamilyProperties = gpu.getQueueFamilyProperties();

    // Check prerequisite
    bool supportsGraphics = false;
    bool supportsPresent = false;
    for (std::size_t i = 0; i < queueFamilyProperties.size(); ++i) {
      if (!supportsPresent && glfwGetPhysicalDevicePresentationSupport(instance, gpu, i))
        supportsPresent = true;

      if (!supportsGraphics && queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics)
        supportsGraphics = true;

      if (supportsPresent && supportsGraphics)
        break;
    }

    if (!supportsPresent || !supportsGraphics)
      continue;

    // TODO: Support the case where multiple discrete GPUs are found
    if (gpuProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
      return gpu;
    }

    bestGpu = gpu;
  }

  CHECK(bestGpu.has_value());
  return *bestGpu;
}
}  // namespace

Application::Application(int width, int height, const std::string& title) {
  glfwSetErrorCallback(glfwErrorCallback);
  if (!glfwInit()) {
    LOG(FATAL) << "Could not initalize GLFW!";
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

  initVulkan();
}

Application::~Application() {
  // Controlled destruction sequence...

  if (m_device)
    m_device->device().waitIdle();

  m_vkSurface.reset();
  glfwDestroyWindow(m_window);
  m_device.reset();
  m_vkInstance.reset();
  glfwTerminate();
}

void Application::run() {
  while (!glfwWindowShouldClose(m_window)) {
    glfwPollEvents();
  }
}

void Application::initVulkan() {
  if (!glfwVulkanSupported()) {
    LOG(FATAL) << "Vulkan not supported!";
  }

  const auto vkGetInstanceProcAddr = PFN_vkGetInstanceProcAddr(
      glfwGetInstanceProcAddress(nullptr, "vkGetInstanceProcAddr"));
  VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

  std::uint32_t glfwExtensionsCount = 0;
  const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
  const std::vector<std::string> instanceExtensions(glfwExtensions, glfwExtensions + glfwExtensionsCount);

  m_vkInstance = CHECK_NOTNULL(createVulkanInstance(instanceExtensions));

  const auto physicalDevice = selectVulkanDevice(*m_vkInstance);
  LOG(INFO) << "Using Vulkan device: " << physicalDevice.getProperties().deviceName;

  m_device = std::make_unique<VulkanDevice>(*m_vkInstance, physicalDevice);

  VkSurfaceKHR surface;
  const auto result = glfwCreateWindowSurface(*m_vkInstance, m_window, nullptr, &surface);
  CHECK_EQ(vk::Result{result}, vk::Result::eSuccess);
  m_vkSurface = vk::UniqueSurfaceKHR(surface, *m_vkInstance);
}

}  // namespace Prism