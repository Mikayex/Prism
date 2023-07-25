#include "Application.hpp"

#include <GLFW/glfw3.h>
#include <glog/logging.h>

#include <algorithm>
#include <optional>
#include <set>
#include <unordered_set>
#include <vector>

#include "VulkanDevice.hpp"
#include "VulkanSwapchain.hpp"
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

vk::PhysicalDevice selectVulkanDevice(vk::Instance instance, const std::vector<std::string>& deviceExtensions = {}) {
  const auto physicalDevices = instance.enumeratePhysicalDevices();
  CHECK(!physicalDevices.empty());

  std::set<std::string> requiredExtensions(deviceExtensions.cbegin(), deviceExtensions.cend());

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

    // Check extensions
    const auto deviceExtensionsList = gpu.enumerateDeviceExtensionProperties();
    std::set<std::string> supportedExtensions;
    for (const auto& extension : deviceExtensionsList)
      supportedExtensions.insert(extension.extensionName);

    if (!std::includes(supportedExtensions.cbegin(), supportedExtensions.cend(), requiredExtensions.cbegin(),
                       requiredExtensions.cend()))
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

vk::UniqueSurfaceKHR createSurface(vk::Instance instance, GLFWwindow* window) {
  VkSurfaceKHR surface;
  const auto result = glfwCreateWindowSurface(instance, DCHECK_NOTNULL(window), nullptr, &surface);
  CHECK_EQ(vk::Result{result}, vk::Result::eSuccess);
  return vk::UniqueSurfaceKHR(surface, instance);
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

  m_swapchain.reset();
  m_vkSurface.reset();
  glfwDestroyWindow(m_window);
  m_frameData = {};
  m_device.reset();
  m_vkInstance.reset();
  glfwTerminate();
}

void Application::run() {
  while (!glfwWindowShouldClose(m_window)) {
    try {
      // Do image acquisition first, so we can do other things on CPU while waiting for image
      const auto imageIndex = acquireImage();

      glfwPollEvents();

      renderFrame(imageIndex);
      presentFrame(imageIndex);
    } catch (vk::OutOfDateKHRError&) {
      // TODO: Recreate swapchain
    }
    m_frameData.next();
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

  const std::vector<std::string> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

  const auto physicalDevice = selectVulkanDevice(*m_vkInstance, deviceExtensions);
  LOG(INFO) << "Using Vulkan device: " << physicalDevice.getProperties().deviceName;

  m_device = std::make_unique<VulkanDevice>(*m_vkInstance, physicalDevice, deviceExtensions);

  m_vkSurface = createSurface(*m_vkInstance, m_window);

  int width, height;
  glfwGetFramebufferSize(m_window, &width, &height);
  const vk::Extent2D desiredExtent{static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height)};
  m_swapchain = std::make_unique<VulkanSwapchain>(*m_device, *m_vkSurface, desiredExtent);

  const auto numFrames = static_cast<std::uint32_t>(m_swapchain->images().size());
  auto commandBuffers = m_device->device().allocateCommandBuffersUnique({
      m_device->commandPool(),
      vk::CommandBufferLevel::ePrimary,
      numFrames,
  });
  m_frameData = Utils::Swappable<FrameData>(numFrames, [this, &commandBuffers](std::size_t index) {
    constexpr vk::SemaphoreCreateInfo semaphoreCreateInfo{};
    constexpr vk::FenceCreateInfo fenceCreateInfo{
        vk::FenceCreateFlagBits::eSignaled,
    };

    return FrameData{
        m_device->device().createSemaphoreUnique(semaphoreCreateInfo),  // imageAcquiredSemaphore
        m_device->device().createSemaphoreUnique(semaphoreCreateInfo),  // renderFinishedSemaphore
        m_device->device().createFenceUnique(fenceCreateInfo),          // commandBufferFence
        std::move(commandBuffers[index]),                               // commandBuffer
    };
  });
}

std::uint32_t Application::acquireImage() {
  const auto result = m_device->device().acquireNextImageKHR(m_swapchain->swapchain(), UINT64_MAX,
                                                             *m_frameData.current().imageAcquiredSemaphore);
  return result.value;
}

void Application::renderFrame(std::uint32_t imageIndex) {
  auto& commandBufferFence = *m_frameData.current().commandBufferFence;
  const auto result = m_device->device().waitForFences(commandBufferFence, VK_TRUE, UINT64_MAX);
  vk::resultCheck(result, "vk::Device::waitForFences");
  m_device->device().resetFences(commandBufferFence);

  auto& commandBuffer = *m_frameData.current().commandBuffer;
  commandBuffer.reset();
  commandBuffer.begin(vk::CommandBufferBeginInfo{});
  // TODO

  // Transition swapchain image layout
  // Should be removed to use an implcit transition in render pass
  const vk::ImageMemoryBarrier barrier{vk::AccessFlagBits::eNone,                 // srcAccess
                                       vk::AccessFlagBits::eColorAttachmentRead,  // dstAccess
                                       vk::ImageLayout::eUndefined,               // oldLayout
                                       vk::ImageLayout::ePresentSrcKHR,           // newLayout
                                       VK_QUEUE_FAMILY_IGNORED,
                                       VK_QUEUE_FAMILY_IGNORED,
                                       m_swapchain->images()[imageIndex],
                                       vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};
  commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe,
                                vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::DependencyFlagBits::eByRegion,
                                {}, {}, barrier);
  commandBuffer.end();

  std::array<vk::PipelineStageFlags, 1> waitStages = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
  const vk::SubmitInfo submitInfo{
      *m_frameData.current().imageAcquiredSemaphore,  // waitSemaphores
      waitStages,                                     // waitDstStageMask
      commandBuffer,
  };
  m_device->graphicsQueue().submit(submitInfo, *m_frameData.current().commandBufferFence);
}

void Application::presentFrame(std::uint32_t imageIndex) {
  vk::PresentInfoKHR presentInfo{
      nullptr,                   // waitSemaphores
      m_swapchain->swapchain(),  // swapchains
      imageIndex,                // imageIndices
  };
  const auto result = m_device->presentQueue().presentKHR(presentInfo);

  switch (result) {
    case vk::Result::eSuccess:
      break;
    case vk::Result::eSuboptimalKHR:
      break;  // TODO Ignore for now...
    default:
      vk::throwResultException(result, "vk::Queue::presentKHR");
  }
}

}  // namespace Prism