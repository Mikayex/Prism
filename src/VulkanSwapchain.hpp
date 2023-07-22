#ifndef PRISM_VULKANSWAPCHAIN_HPP
#define PRISM_VULKANSWAPCHAIN_HPP
#include <vulkan/vulkan.hpp>

namespace Prism {
class VulkanDevice;

class VulkanSwapchain {
public:
  VulkanSwapchain(const VulkanDevice& device, const vk::SurfaceKHR& surface, const vk::Extent2D& desiredExtent);
  ~VulkanSwapchain();

  [[nodiscard]] const vk::SwapchainKHR& swapchain() const {
    return *m_vkSwapchain;
  }

  [[nodiscard]] vk::SwapchainKHR& swapchain() {
    return *m_vkSwapchain;
  }

  [[nodiscard]] vk::SurfaceFormatKHR surfaceFormat() const {
    return m_surfaceFormat;
  }

  [[nodiscard]] vk::PresentModeKHR presentMode() const {
    return m_presentMode;
  }

  [[nodiscard]] const vk::Extent2D& extent() const {
    return m_extent;
  }

  [[nodiscard]] const std::vector<vk::Image>& images() const {
    return m_images;
  }

  [[nodiscard]] const std::vector<vk::UniqueImageView>& imageViews() const {
    return m_imageViews;
  }

private:
  vk::UniqueSwapchainKHR m_vkSwapchain;
  vk::SurfaceFormatKHR m_surfaceFormat;
  vk::PresentModeKHR m_presentMode = vk::PresentModeKHR::eFifo;
  vk::Extent2D m_extent;
  std::vector<vk::Image> m_images;
  std::vector<vk::UniqueImageView> m_imageViews;
};

}  // namespace Prism

#endif  // PRISM_VULKANSWAPCHAIN_HPP
