#include "forward.hpp"
#include "logging.hpp"
#include <dlfcn.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <mutex>

static std::vector<std::string> candidates = {
  "libvulkan.so.1",
  "libvulkan.so",
  "/system/vendor/lib64/libvulkan.so",
  "/system/lib64/libvulkan.so"
};

static void* backend = nullptr;
static std::unordered_map<std::string, void*> symcache;
static std::mutex g_mu;

bool load_backends() {
  std::lock_guard<std::mutex> lk(g_mu);
  if (backend) return true;
  for (const auto &c : candidates) {
    backend = dlopen(c.c_str(), RTLD_NOW | RTLD_LOCAL);
    if (backend) { logf("Loaded backend: %s", c.c_str()); break; }
  }
  if (!backend) logf("No system Vulkan backend found");
  return backend != nullptr;
}

void* resolve_sym(const char* name) {
  std::lock_guard<std::mutex> lk(g_mu);
  if (!load_backends()) return nullptr;
  auto it = symcache.find(name);
  if (it != symcache.end()) return it->second;
  void* s = dlsym(backend, name);
  if (!s) logf("resolve_sym: missing %s", name);
  symcache.emplace(name, s);
  return s;
}
