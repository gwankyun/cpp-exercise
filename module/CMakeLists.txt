set(CMAKE_FOLDER module)

set(module_main module_main)
add_executable(${module_main})
target_sources(${module_main}
  PRIVATE
    src/main.cpp
    src/a.cpp
    src/a.h
  PRIVATE FILE_SET CXX_MODULES FILES
    src/main.cppm
    src/a.cppm
)
target_link_libraries(${module_main} PRIVATE spdlog::spdlog)
target_compile_features(${module_main} PRIVATE cxx_std_23)
