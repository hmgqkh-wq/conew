#include "logging.hpp"
#include <mutex>
#include <cstdarg>
#include <cstdio>
#include <filesystem>
#include <string>

static std::mutex g_log_mu;
static FILE* g_log_file = nullptr;
static std::string g_log_path = "/sdcard/Winlator/xclipse940/logs/wrapper.log";

void log_to(const std::string& path) {
  std::lock_guard<std::mutex> lk(g_log_mu);
  g_log_path = path;
  if (g_log_file) { fclose(g_log_file); g_log_file = nullptr; }
}

void log_init() {
  std::lock_guard<std::mutex> lk(g_log_mu);
  if (g_log_file) return;
  std::filesystem::path p(g_log_path);
  std::error_code ec;
  std::filesystem::create_directories(p.parent_path(), ec);
  g_log_file = std::fopen(g_log_path.c_str(), "a");
  if (!g_log_file) g_log_file = stderr;
  std::fprintf(g_log_file, "xclipse940 wrapper init\n");
  std::fflush(g_log_file);
}

void logf(const char* fmt, ...) {
  std::lock_guard<std::mutex> lk(g_log_mu);
  if (!g_log_file) log_init();
  va_list ap; va_start(ap, fmt);
  std::vfprintf(g_log_file, fmt, ap);
  std::fprintf(g_log_file, "\n");
  std::fflush(g_log_file);
  va_end(ap);
}
