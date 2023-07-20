#ifndef PRISM_APPLICATION_HPP
#define PRISM_APPLICATION_HPP

#include <GLFW/glfw3.h>

#include <memory>
#include <string>
#include <vulkan/vulkan.hpp>

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
};

}  // namespace Prism

#endif  // PRISM_APPLICATION_HPP
