#pragma once
#include <string>
void log_init();
void logf(const char* fmt, ...);
void log_to(const std::string& path);
