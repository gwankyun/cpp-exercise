set(CMAKE_FOLDER uv)

set(uv_common uv_common)
add_library(${uv_common} INTERFACE)
target_link_libraries(${uv_common} INTERFACE
  spdlog::spdlog libuv::uv Boost::scope
)
target_include_directories(${uv_common} INTERFACE include)
target_compile_features(${uv_common} INTERFACE cxx_std_20)

set(uv_server uv_server)
add_executable(${uv_server})
target_sources(${uv_server} PRIVATE
  src/uv_server.cpp
)
target_link_libraries(${uv_server} PRIVATE ${uv_common})

set(uv_client uv_client)
add_executable(${uv_client})
target_sources(${uv_client} PRIVATE
  src/uv_client.cpp
)
target_link_libraries(${uv_client} PRIVATE ${uv_common})
