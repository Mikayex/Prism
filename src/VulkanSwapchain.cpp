#include "VulkanSwapchain.hpp"

#include <glog/logging.h>

#include "VulkanDevice.hpp"
#include "VulkanUtils.hpp"

namespace Prism {
namespace {
constexpr std::array<vk::SurfaceFormatKHR, 3> surfaceFormatPriorityList = {
    vk::SurfaceFormatKHR{vk::Format::eR8G8B8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear},
    vk::SurfaceFormatKHR{vk::Format::eB8G8R8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear},
    vk::SurfaceFormatKHR{vk::Format::eA8B8G8R8SrgbPack32, vk::ColorSpaceKHR::eSrgbNonlinear},
};

vk::SurfaceFormatKHR selectFormat(const VulkanDevice& device, const vk::SurfaceKHR& surface) {
  const auto surfaceFormats = device.physicalDevice().getSurfaceFormatsKHR(surface);
  const auto found = std::find_first_of(surfaceFormatPriorityList.cbegin(), surfaceFormatPriorityList.cend(),
                                        surfaceFormats.cbegin(), surfaceFormats.cend());

  if (found == surfaceFormatPriorityList.cend())
    LOG(FATAL) << "Unable to find a suitable surface format";
  return *found;
}

vk::Extent2D computeExtent(const VulkanDevice& device, const vk::SurfaceCapabilitiesKHR& capabilities,
                           const vk::Extent2D& desiredExtent) {
  if (capabilities.currentExtent.width != std::numeric_limits<std::uint32_t>::max()) {
    return capabilities.currentExtent;
  } else {
    return {
        std::clamp(desiredExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
        std::clamp(desiredExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height),
    };
  }
}
}  // namespace
VulkanSwapchain::VulkanSwapchain(const VulkanDevice& device, const vk::SurfaceKHR& surface,
                                 const vk::Extent2D& desiredExtent) {
  const auto surfaceCapabilities = device.physicalDevice().getSurfaceCapabilitiesKHR(surface);

  m_surfaceFormat = selectFormat(device, surface);
  m_extent = computeExtent(device, surfaceCapabilities, desiredExtent);

  const auto imageCount = std::clamp(std::uint32_t{2}, surfaceCapabilities.minImageCount,
                                     surfaceCapabilities.maxImageCount > 0 ? surfaceCapabilities.maxImageCount
                                                                           : std::numeric_limits<std::uint32_t>::max());

  const bool sameQueue = device.graphicsQueueFamily() == device.presentQueueFamily();
  const std::vector<std::uint32_t> queueFamilyIndices = {device.graphicsQueueFamily(), device.presentQueueFamily()};
  const vk::SwapchainCreateInfoKHR createInfo{
      {},                                                                      // flags
      surface,                                                                 // surface
      imageCount,                                                              // minImageCount
      m_surfaceFormat.format,                                                  // format
      m_surfaceFormat.colorSpace,                                              // colorSpace
      m_extent,                                                                // extent
      1,                                                                       // imageLayers
      vk::ImageUsageFlagBits::eColorAttachment,                                // imageUsage
      sameQueue ? vk::SharingMode::eExclusive : vk::SharingMode::eConcurrent,  // sharingMode
      queueFamilyIndices,                                                      // queueFamilyIndicies
      vk::SurfaceTransformFlagBitsKHR::eIdentity,                              // preTransform
      vk::CompositeAlphaFlagBitsKHR::eOpaque,                                  // compositeAlpha
      m_presentMode,                                                           // presentMode
      VK_TRUE,                                                                 // clipped
  };
  m_vkSwapchain = CHECK_NOTNULL(device.device().createSwapchainKHRUnique(createInfo));
  m_images = device.device().getSwapchainImagesKHR(*m_vkSwapchain);

  // Create image views
  m_imageViews.clear();
  m_imageViews.reserve(m_images.size());
  std::transform(m_images.cbegin(), m_images.cend(), std::back_inserter(m_imageViews),
                 [this, &device](const vk::Image& image) {
                   constexpr vk::ImageSubresourceRange subresourceRange{
                       vk::ImageAspectFlagBits::eColor,  // aspectMask
                       0,                                // baseMipLevel
                       1,                                // levelCount
                       0,                                // baseLayer
                       1,                                // layerCount
                   };

                   const vk::ImageViewCreateInfo viewCreateInfo{
                       {},                      // flags
                       image,                   // image
                       vk::ImageViewType::e2D,  // viewType
                       m_surfaceFormat.format,  // format
                       {},                      // mapping (default identity)
                       subresourceRange,        // subresourceRange
                   };
                   return CHECK_NOTNULL(device.device().createImageViewUnique(viewCreateInfo));
                 });
}

VulkanSwapchain::~VulkanSwapchain() = default;
}  // namespace Prism