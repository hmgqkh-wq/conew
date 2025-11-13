#pragma once
#include <string>
#include <vulkan/vulkan.h>

void profile_manager_init(const std::string& profile_dir);
void apply_app_profile(const char* app_name);
void clamp_properties_with_profile(VkPhysicalDeviceProperties2* p);
void clamp_features_with_profile(VkPhysicalDeviceFeatures2* f);
