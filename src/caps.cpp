#include "caps.hpp"
#include <cstring>

void override_physical_device_properties2(VkPhysicalDeviceProperties2* p) {
  p->properties.apiVersion = VK_API_VERSION_1_3;
  p->properties.vendorID = 0x1002;
  p->properties.deviceID = 0x0940;
  std::strncpy(p->properties.deviceName, "Xclipse 940 (Wrapper ICD)", VK_MAX_PHYSICAL_DEVICE_NAME_SIZE);
}

void override_physical_device_features2(VkPhysicalDeviceFeatures2* f) {
  f->features.robustBufferAccess = VK_TRUE;
  f->features.samplerAnisotropy = VK_TRUE;
  f->features.shaderInt16 = VK_TRUE;
  f->features.shaderFloat16 = VK_TRUE;
  f->features.shaderInt64 = VK_FALSE;
  f->features.shaderFloat64 = VK_FALSE;
}
