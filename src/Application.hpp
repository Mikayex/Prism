#ifndef PRISM_APPLICATION_HPP
#define PRISM_APPLICATION_HPP

#include <memory>
#include <string>
#include <utils/Swappable.hpp>
#include <vulkan/vulkan.hpp>

struct GLFWwindow;

namespace Prism {
class VulkanDevice;
class VulkanSwapchain;

class Application {
public:
  Application(int width, int height, const std::string& title);
  ~Application();

  void run();

private:
  void initVulkan();
  std::uint32_t acquireImage();
  void renderFrame(std::uint32_t imageIndex);
  void presentFrame(std::uint32_t imageIndex);

  struct FrameData {
    vk::UniqueSemaphore imageAcquiredSemaphore;
    vk::UniqueSemaphore renderFinishedSemaphore;
    vk::UniqueFence commandBufferFence;
    vk::UniqueCommandBuffer commandBuffer;
  };

  GLFWwindow* m_window = nullptr;

  vk::UniqueInstance m_vkInstance{};
  std::unique_ptr<VulkanDevice> m_device{};
  vk::UniqueSurfaceKHR m_vkSurface{};
  std::unique_ptr<VulkanSwapchain> m_swapchain{};

  Utils::Swappable<FrameData> m_frameData;
};

}  // namespace Prism

#endif  // PRISM_APPLICATION_HPP
