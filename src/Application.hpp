#ifndef PRISM_APPLICATION_HPP
#define PRISM_APPLICATION_HPP

#include <memory>
#include <string>
#include <vulkan/vulkan.hpp>

struct GLFWwindow;

namespace Prism {
class VulkanDevice;

class Application {
public:
  Application(int width, int height, const std::string& title);
  ~Application();

  void run();

private:
  void initVulkan();

  GLFWwindow* m_window = nullptr;

  vk::UniqueInstance m_vkInstance{};
  std::unique_ptr<VulkanDevice> m_device{};
  vk::UniqueSurfaceKHR m_vkSurface{};
};

}  // namespace Prism

#endif  // PRISM_APPLICATION_HPP
