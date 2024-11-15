set(CMAKE_FOLDER module)

set(module_main module_main)
add_executable(${module_main})
target_sources(${module_main} PRIVATE
  src/main.cpp
)
target_link_libraries(${module_main} PRIVATE spdlog::spdlog)
target_compile_features(${module_main} PRIVATE cxx_std_23)
# target_compile_options(${module_main} PRIVATE
#   $<$<CXX_COMPILER_ID:MSVC>:/std:c++lastest;>)
