#include "Application.hpp"

#include <glog/logging.h>

namespace Prism {
namespace {
void glfwErrorCallback(int error, const char* description) {
  LOG(ERROR) << "Glfw Error " << error << ": " << description;
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
  glfwDestroyWindow(m_window);
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
}

}  // namespace Prism