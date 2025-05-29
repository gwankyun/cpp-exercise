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

# 掃描模塊
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

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

list(APPEND msvc_options "/utf-8")

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:${msvc_options}>")

add_subdirectory(module)

set(spdlog_module spdlog_module)
add_library(${spdlog_module})
target_sources(${spdlog_module}
  PRIVATE FILE_SET CXX_MODULES FILES
    include/spdlog.ixx)
target_link_libraries(${spdlog_module} PUBLIC spdlog::spdlog)
target_compile_features(${spdlog_module} PRIVATE cxx_std_23)
set_target_properties(${spdlog_module} PROPERTIES FOLDER module)

set(catch2_module catch2_module)
add_library(${catch2_module})
target_sources(${catch2_module}
  PRIVATE FILE_SET CXX_MODULES FILES
    include/catch2.ixx)
target_link_libraries(${catch2_module} PUBLIC Catch2::Catch2)
target_compile_features(${catch2_module} PRIVATE cxx_std_23)
set_target_properties(${catch2_module} PROPERTIES FOLDER module)

set(main main)
add_executable(${main})
target_sources(${main}
  PRIVATE
    src/main.cpp
  PRIVATE FILE_SET CXX_MODULES FILES
    src/main.ixx
)
target_link_libraries(${main} PRIVATE Boost::scope Catch2::Catch2 ${spdlog_module})
target_compile_features(${main} PRIVATE cxx_std_23)

set(other other)

set(boost_example boost_example)
add_executable(${boost_example})
target_sources(${boost_example} PRIVATE
  src/boost_example.cpp
)
target_link_libraries(${boost_example} PRIVATE Boost::scope Catch2::Catch2)
target_compile_features(${boost_example} PRIVATE cxx_std_23)
set_target_properties(${boost_example} PROPERTIES FOLDER ${other})

find_package(doctest CONFIG REQUIRED)
set(doctest_example doctest_example)
add_executable(${doctest_example})
target_sources(${doctest_example} PRIVATE
  src/doctest_example.cpp
)
target_link_libraries(${doctest_example} PRIVATE doctest::doctest)
target_compile_features(${doctest_example} PRIVATE cxx_std_23)
set_target_properties(${doctest_example} PROPERTIES FOLDER ${other})

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
  set_target_properties(${hello_imgui_example} PROPERTIES FOLDER ${other})
endif()

if(TARGET msft_proxy)
  set(proxy_example proxy_example)
  add_executable(${proxy_example})
  target_sources(${proxy_example} PRIVATE
    src/proxy_example.cpp
  )
  target_link_libraries(${proxy_example} PRIVATE spdlog::spdlog msft_proxy)
  target_compile_features(${proxy_example} PRIVATE cxx_std_20)
  set_target_properties(${proxy_example} PROPERTIES FOLDER ${other})
endif()

add_subdirectory(uv)

find_path(DYLIB_INCLUDE_DIRS "dylib.hpp")

add_subdirectory(use_library)

add_subdirectory(data_structure)

add_subdirectory(ut)
