# cmake最低版本
cmake_minimum_required(VERSION 3.10...4.0)

# 項目名
project(cpp-exercise)

include(CMakePrintHelpers) # cmake_print_variables

cmake_print_variables(PROJECT_NAME)
cmake_print_variables(PROJECT_SOURCE_DIR)
cmake_print_variables(PROJECT_BINARY_DIR)
cmake_print_variables(PROJECT_IS_TOP_LEVEL)

cmake_print_variables(CMAKE_SOURCE_DIR)
cmake_print_variables(CMAKE_CURRENT_SOURCE_DIR)

# 輸出目錄
set(source_dir ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${source_dir}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${source_dir}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${source_dir}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${source_dir}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${source_dir}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${source_dir}/lib)

find_package(spdlog CONFIG REQUIRED)

find_package(boost_scope REQUIRED CONFIG)

find_package(proxy CONFIG)

find_package(Catch2 CONFIG REQUIRED)

find_package(libuv CONFIG REQUIRED)

set(msvc_options)
list(APPEND msvc_options "/W4" "/MP")

# 引入__cplusplus
list(APPEND msvc_options "/Zc:__cplusplus")

# 模塊支持
list(APPEND msvc_options "/experimental:module")

# 柔性數組
list(APPEND msvc_options "/Zc:zeroSizeArrayNew")

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:${msvc_options}>")

set(main main)
add_executable(${main})
target_sources(${main} PRIVATE
  src/main.cpp
)
target_link_libraries(${main} PRIVATE spdlog::spdlog Boost::scope Catch2::Catch2)
target_compile_features(${main} PRIVATE cxx_std_23)

set(boost_example boost_example)
add_executable(${boost_example})
target_sources(${boost_example} PRIVATE
  src/boost_example.cpp
)
target_link_libraries(${boost_example} PRIVATE Boost::scope Catch2::Catch2)
target_compile_features(${boost_example} PRIVATE cxx_std_23)

set(intrusive_container intrusive_container)
add_executable(${intrusive_container})
target_sources(${intrusive_container} PRIVATE
  src/intrusive_container.cpp
)
target_link_libraries(${intrusive_container} PRIVATE spdlog::spdlog)

find_package(hello-imgui CONFIG)

find_package(nlohmann_json CONFIG)

if((TARGET hello-imgui::hello_imgui) AND(TARGET nlohmann_json::nlohmann_json))
  set(hello_imgui_example hello_imgui_example)
  add_executable(${hello_imgui_example})
  target_sources(${hello_imgui_example} PRIVATE
    src/hello_imgui_example.cpp
  )
  target_link_libraries(${hello_imgui_example} PRIVATE
    hello-imgui::hello_imgui
    nlohmann_json::nlohmann_json
  )
  target_compile_features(${hello_imgui_example} PRIVATE cxx_std_20)
endif()

if(TARGET msft_proxy)
  set(proxy_example proxy_example)
  add_executable(${proxy_example})
  target_sources(${proxy_example} PRIVATE
    src/proxy_example.cpp
  )
  target_link_libraries(${proxy_example} PRIVATE spdlog::spdlog msft_proxy)
  target_compile_features(${proxy_example} PRIVATE cxx_std_20)
endif()

add_subdirectory(uv)

find_path(DYLIB_INCLUDE_DIRS "dylib.hpp")

add_subdirectory(use_library)

add_subdirectory(module)

add_subdirectory(data_structure)
