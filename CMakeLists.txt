# cmake最低版本
cmake_minimum_required(VERSION 3.30.0)

# 項目名
set(project cpp-exercise)
project(${project})

include(CMakePrintHelpers) # cmake_print_variables

# 輸出目錄
set(source_dir ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${source_dir}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${source_dir}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${source_dir}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${source_dir}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${source_dir}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${source_dir}/lib)

find_package(spdlog CONFIG REQUIRED)

set(main main)
add_executable(${main})
target_sources(${main} PRIVATE
  src/main.cpp
)
target_link_libraries(${main} PRIVATE spdlog::spdlog)

# find_package(imgui CONFIG REQUIRED)
# set(imgui_test imgui_test)
# add_executable(${imgui_test})
# target_sources(${imgui_test} PRIVATE
#   src/imgui_test.cpp
#   src/imgui/imgui.cpp
#   src/imgui/imgui_demo.cpp
#   src/imgui/imgui_draw.cpp
#   src/imgui/imgui_tables.cpp
#   src/imgui/imgui_widgets.cpp
#   src/imgui/imgui_impl_dx10.cpp
#   # src/imgui/imgui_impl_win32.cpp
# )
# target_link_libraries(${imgui_test} PRIVATE
#   # imgui::imgui
#   d3d11 d3dcompiler dxgi)
