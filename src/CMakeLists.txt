cmake_minimum_required(VERSION 3.20)
project(xclipse_src CXX)

include(FetchContent)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

add_library(vulkan_xclipse940 SHARED
  wrapper.cpp
  forward.cpp
  caps.cpp
  profile_manager.cpp
  pipeline_cache.cpp
  logging.cpp
  bc_decode_comp.comp
  version.hpp
)

target_include_directories(vulkan_xclipse940 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(vulkan_xclipse940 PRIVATE nlohmann_json::nlohmann_json)
set_target_properties(vulkan_xclipse940 PROPERTIES OUTPUT_NAME "libvulkan")
target_compile_definitions(vulkan_xclipse940 PRIVATE _GNU_SOURCE)
target_link_options(vulkan_xclipse940 PRIVATE "-Wl,--version-script=${CMAKE_CURRENT_SOURCE_DIR}/exports.map")
install(TARGETS vulkan_xclipse940 DESTINATION usr/lib)
