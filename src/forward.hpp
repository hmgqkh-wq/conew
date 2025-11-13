#pragma once
#include <cstddef>
bool load_backends();
void* resolve_sym(const char* name);
