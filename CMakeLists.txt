# cmake最低版本
cmake_minimum_required(VERSION 3.28...4.0)

# 項目名
project(cpp_exercise LANGUAGES CXX)

include(CMakePrintHelpers) # cmake_print_variables

cmake_print_variables(PROJECT_NAME)
cmake_print_variables(PROJECT_SOURCE_DIR)
cmake_print_variables(PROJECT_BINARY_DIR)
cmake_print_variables(PROJECT_IS_TOP_LEVEL)

cmake_print_variables(CMAKE_SOURCE_DIR)
cmake_print_variables(CMAKE_CURRENT_SOURCE_DIR)

# 掃描模塊
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# 自定義輸出目錄
set(output OFF CACHE BOOL "output path")
if(output)
  set(source_dir ${CMAKE_CURRENT_SOURCE_DIR})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${source_dir}/bin)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${source_dir}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${source_dir}/lib)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${source_dir}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${source_dir}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${source_dir}/lib)
endif()

find_package(spdlog CONFIG REQUIRED)

find_package(boost_scope REQUIRED CONFIG)

find_package(proxy CONFIG)

find_package(Catch2 CONFIG REQUIRED)

find_package(libuv CONFIG REQUIRED)

find_package(doctest CONFIG REQUIRED)

find_package(hello-imgui CONFIG)

find_package(nlohmann_json CONFIG)

find_path(DYLIB_INCLUDE_DIRS "dylib.hpp")

set(msvc_options)

# 提高警告級別
list(APPEND msvc_options "/W4" "/MP")

# 引入__cplusplus
list(APPEND msvc_options "/Zc:__cplusplus")

# 模塊支持
list(APPEND msvc_options "/experimental:module")

# 柔性數組
list(APPEND msvc_options "/Zc:zeroSizeArrayNew")

# 支持源碼UTF-8
list(APPEND msvc_options "/utf-8")

# 全局添加
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:${msvc_options}>")

# 下拉選項
set(color "" CACHE STRING "color")
set_property(CACHE color PROPERTY STRINGS "red;green;blue")

add_subdirectory(module)

add_executable(main)
target_sources(main
  PRIVATE
    src/main_macro.h
  PRIVATE FILE_SET CXX_MODULES FILES
    src/main.ixx
)
target_link_libraries(main
  PRIVATE Boost::scope::module Catch2::module spdlog::module)
target_compile_features(main PRIVATE cxx_std_23)

add_subdirectory(other)

add_subdirectory(uv)

add_subdirectory(use_library)

add_subdirectory(data_structure)

add_subdirectory(ut)

add_subdirectory(target)
