# cmake最低版本
cmake_minimum_required(VERSION 3.31.0)

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

find_package(proxy CONFIG)

set(main main)
add_executable(${main})
target_sources(${main} PRIVATE
  src/main.cpp
)
target_link_libraries(${main} PRIVATE spdlog::spdlog)
target_compile_features(${main} PRIVATE cxx_std_20)

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

find_package(libuv CONFIG REQUIRED)
find_package(boost_scope REQUIRED CONFIG)

set(uv_common uv_common)
add_library(${uv_common} INTERFACE)
target_link_libraries(${uv_common} INTERFACE
  spdlog::spdlog libuv::uv Boost::scope
)
target_include_directories(${uv_common} INTERFACE include)

set(uv uv)

set(uv_server uv_server)
add_executable(${uv_server})
target_sources(${uv_server} PRIVATE
  src/uv_server.cpp
)
target_link_libraries(${uv_server} PRIVATE ${uv_common})
target_compile_features(${uv_server} PRIVATE cxx_std_20)
set_target_properties(${uv_server} PROPERTIES FOLDER ${uv})
# set_target_properties(${uv_common} PROPERTIES FOLDER uv)

set(uv_client uv_client)
add_executable(${uv_client})
target_sources(${uv_client} PRIVATE
  src/uv_client.cpp
)
target_link_libraries(${uv_client} PRIVATE ${uv_common})
target_compile_features(${uv_client} PRIVATE cxx_std_20)
set_target_properties(${uv_client} PROPERTIES FOLDER ${uv})
