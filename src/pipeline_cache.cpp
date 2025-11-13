#include "pipeline_cache.hpp"
#include "logging.hpp"
#include <filesystem>
#include <fstream>

static std::string g_cache_dir = "/sdcard/Winlator/xclipse940/cache";

void pipeline_cache_init(const std::string& cache_dir) {
  if (!cache_dir.empty()) g_cache_dir = cache_dir;
  std::error_code ec;
  std::filesystem::create_directories(g_cache_dir, ec);
  logf("Pipeline cache dir: %s", g_cache_dir.c_str());
}

void pipeline_cache_store(const char* key, const void* data, size_t size) {
  std::string path = g_cache_dir + "/" + key + ".bin";
  std::ofstream out(path, std::ios::binary);
  out.write(reinterpret_cast<const char*>(data), size);
  logf("Stored pipeline cache %s (%zu bytes)", path.c_str(), size);
}

bool pipeline_cache_load(const char* key, void** out_data, size_t* out_size) {
  std::string path = g_cache_dir + "/" + key + ".bin";
  if (!std::filesystem::exists(path)) return false;
  std::ifstream in(path, std::ios::binary | std::ios::ate);
  auto sz = in.tellg();
  in.seekg(0);
  char* buf = new char[sz];
  in.read(buf, sz);
  *out_data = buf; *out_size = (size_t)sz;
  logf("Loaded pipeline cache %s (%zu bytes)", path.c_str(), (size_t)sz);
  return true;
}
