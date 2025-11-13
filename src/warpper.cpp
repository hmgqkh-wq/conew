#include <vulkan/vulkan.h>
#include "forward.hpp"
#include "logging.hpp"
#include "caps.hpp"
#include "profile_manager.hpp"
#include "pipeline_cache.hpp"
#include <cstring>

extern "C" {

VKAPI_ATTR VkResult VKAPI_CALL vkCreateInstance(
  const VkInstanceCreateInfo* pCreateInfo,
  const VkAllocationCallbacks* pAllocator,
  VkInstance* pInstance)
{
  log_init();
  profile_manager_init("profiles/winlator");
  pipeline_cache_init("/sdcard/Winlator/xclipse940/cache");
  using PFN = PFN_vkCreateInstance;
  PFN fn = (PFN)resolve_sym("vkCreateInstance");
  if (!fn) return VK_ERROR_INITIALIZATION_FAILED;
  VkResult r = fn(pCreateInfo, pAllocator, pInstance);
  logf("vkCreateInstance => %d", r);
  return r;
}

VKAPI_ATTR VkResult VKAPI_CALL vkEnumeratePhysicalDevices(
  VkInstance instance, uint32_t* pCount, VkPhysicalDevice* pDevices)
{
  using PFN = PFN_vkEnumeratePhysicalDevices;
  PFN fn = (PFN)resolve_sym("vkEnumeratePhysicalDevices");
  if (!fn) return VK_ERROR_INITIALIZATION_FAILED;
  VkResult r = fn(instance, pCount, pDevices);
  logf("vkEnumeratePhysicalDevices => %d, count=%u", r, pCount ? *pCount : 0);
  return r;
}

VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceProperties2(
  VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties2* pProperties)
{
  using PFN = PFN_vkGetPhysicalDeviceProperties2;
  PFN fn = (PFN)resolve_sym("vkGetPhysicalDeviceProperties2");
  if (fn) fn(physicalDevice, pProperties);
  override_physical_device_properties2(pProperties);
  clamp_properties_with_profile(pProperties);
  logf("vkGetPhysicalDeviceProperties2: deviceName=%s", pProperties->properties.deviceName);
}

VKAPI_ATTR void VKAPI_CALL vkGetPhysicalDeviceFeatures2(
  VkPhysicalDevice physicalDevice, VkPhysicalDeviceFeatures2* pFeatures)
{
  using PFN = PFN_vkGetPhysicalDeviceFeatures2;
  PFN fn = (PFN)resolve_sym("vkGetPhysicalDeviceFeatures2");
  if (fn) fn(physicalDevice, pFeatures);
  override_physical_device_features2(pFeatures);
  clamp_features_with_profile(pFeatures);
  logf("vkGetPhysicalDeviceFeatures2 called");
}

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance instance, const char* pName) {
  if (!pName) return nullptr;
  if (strcmp(pName, "vkGetInstanceProcAddr") == 0) return (PFN_vkVoidFunction)vkGetInstanceProcAddr;
  if (strcmp(pName, "vkCreateInstance") == 0) return (PFN_vkVoidFunction)vkCreateInstance;
  auto fn = (PFN_vkGetInstanceProcAddr)resolve_sym("vkGetInstanceProcAddr");
  if (!fn) return nullptr;
  return fn(instance, pName);
}

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(VkDevice device, const char* pName) {
  if (!pName) return nullptr;
  auto fn = (PFN_vkGetDeviceProcAddr)resolve_sym("vkGetDeviceProcAddr");
  if (!fn) return nullptr;
  return fn(device, pName);
}

} // extern "C"
