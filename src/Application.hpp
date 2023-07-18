#ifndef PRISM_APPLICATION_HPP
#define PRISM_APPLICATION_HPP

#include <GLFW/glfw3.h>

#include <string>
#include <vulkan/vulkan.hpp>

namespace Prism {

class Application {
public:
  Application(int width, int height, const std::string& title);
  ~Application();

  void run();

private:
  void initVulkan();

  GLFWwindow* m_window = nullptr;
};

}  // namespace Prism

#endif  // PRISM_APPLICATION_HPP
