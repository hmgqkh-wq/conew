cmake_minimum_required(VERSION 3.20)
project(xclipse940_wrapper LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

add_subdirectory(src)

install(FILES icd_manifest.json DESTINATION .)
install(FILES usr/share/meta.json DESTINATION usr/share)
install(FILES profiles/winlator/dxvk.env DESTINATION profiles/winlator)
install(FILES profiles/winlator/mesa.env DESTINATION profiles/winlator)
install(FILES etc/exynostools/performance_mode.conf DESTINATION etc/exynostools)
