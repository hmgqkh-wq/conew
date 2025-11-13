#pragma once
#include <string>
void pipeline_cache_init(const std::string& cache_dir);
void pipeline_cache_store(const char* key, const void* data, size_t size);
bool pipeline_cache_load(const char* key, void** out_data, size_t* out_size);
