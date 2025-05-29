set(CMAKE_FOLDER module)

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
target_link_libraries(${module_main} PRIVATE spdlog_module catch2_module)
target_compile_features(${module_main} PRIVATE cxx_std_23)
