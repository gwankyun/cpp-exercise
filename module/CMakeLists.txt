set(CMAKE_FOLDER module)

set(CMAKE_CXX_SCAN_FOR_MODULES ON)

set(module_main module_main)
add_executable(${module_main})
target_sources(${module_main}
  PRIVATE
    src/main.cpp
    src/a.cpp
    src/a.h
  PRIVATE FILE_SET CXX_MODULES FILES
    src/main.ixx
    src/a.ixx
)
target_link_libraries(${module_main} PRIVATE spdlog::spdlog)
target_compile_features(${module_main} PRIVATE cxx_std_23)
