set(uv_common uv_common)
add_library(${uv_common} INTERFACE)
target_link_libraries(${uv_common} INTERFACE
  spdlog::spdlog libuv::uv Boost::scope
)
target_include_directories(${uv_common} INTERFACE include)

cmake_print_variables(CMAKE_SOURCE_DIR)
cmake_print_variables(CMAKE_CURRENT_SOURCE_DIR)

set(uv uv)

set(CMAKE_FOLDER ${uv})

set(uv_server uv_server)
add_executable(${uv_server})
target_sources(${uv_server} PRIVATE
  src/uv_server.cpp
)
target_link_libraries(${uv_server} PRIVATE ${uv_common})
target_compile_features(${uv_server} PRIVATE cxx_std_20)
set_target_properties(${uv_server} PROPERTIES FOLDER ${uv})

set(uv_client uv_client)
add_executable(${uv_client})
target_sources(${uv_client} PRIVATE
  src/uv_client.cpp
)
target_link_libraries(${uv_client} PRIVATE ${uv_common})
target_compile_features(${uv_client} PRIVATE cxx_std_20)
