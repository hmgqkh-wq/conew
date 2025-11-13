#include "profile_manager.hpp"
#include "logging.hpp"
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

static std::string g_profile_dir = "profiles/winlator";
static std::unordered_map<std::string, json> g_profiles;
static json g_default_profile;

void profile_manager_init(const std::string& profile_dir) {
  g_profile_dir = profile_dir;
  std::error_code ec;
  if (!std::filesystem::exists(g_profile_dir, ec)) {
    logf("Profile dir missing: %s", g_profile_dir.c_str());
    return;
  }
  for (auto &p : std::filesystem::directory_iterator(g_profile_dir, ec)) {
    if (p.path().extension() == ".json") {
      std::ifstream in(p.path());
      json j; in >> j;
      std::string name = p.path().stem().string();
      g_profiles.emplace(name, j);
      logf("Loaded profile %s", name.c_str());
      if (name == "default") g_default_profile = j;
    }
  }
  if (g_default_profile.is_null()) {
    g_default_profile["features"] = json::object();
  }
}

static const json* find_profile_for_app(const char* app_name) {
  if (!app_name) return &g_default_profile;
  auto it = g_profiles.find(app_name);
  if (it != g_profiles.end()) return &it->second;
  return &g_default_profile;
}

void apply_app_profile(const char* app_name) {
  auto p = find_profile_for_app(app_name);
  logf("Applying profile for %s", app_name ? app_name : "unknown");
}

void clamp_properties_with_profile(VkPhysicalDeviceProperties2* p) {
  const json& prof = g_default_profile;
  if (prof.contains("clamp_limits")) {
    auto &cl = prof["clamp_limits"];
    if (cl.contains("maxImageDimension2D")) {
      uint32_t val = cl["maxImageDimension2D"].get<uint32_t>();
      if (p->properties.limits.maxImageDimension2D > val)
        p->properties.limits.maxImageDimension2D = val;
    }
    if (cl.contains("maxBoundDescriptorSets")) {
      uint32_t val = cl["maxBoundDescriptorSets"].get<uint32_t>();
      if (p->properties.limits.maxBoundDescriptorSets > val)
        p->properties.limits.maxBoundDescriptorSets = val;
    }
  }
}

void clamp_features_with_profile(VkPhysicalDeviceFeatures2* f) {
  const json& prof = g_default_profile;
  if (prof.contains("features")) {
    auto &fe = prof["features"];
    if (fe.contains("descriptorIndexing") && !fe["descriptorIndexing"].get<bool>()) {
      logf("Profile disables descriptorIndexing");
    }
  }
}
