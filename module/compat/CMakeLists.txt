set(CMAKE_FOLDER module)

add_subdirectory(compat_static_lib)

add_library(compat_common INTERFACE)
target_sources(compat_common
  INTERFACE
    src/global_module.hpp
    src/main.hpp
)

add_executable(compat_module_ixx)
target_sources(compat_module_ixx
  PRIVATE FILE_SET CXX_MODULES FILES
    src/main.ixx
)
target_compile_definitions(compat_module_ixx
  PRIVATE
    USE_STD_MODULE=1
)
target_link_libraries(compat_module_ixx PRIVATE compat_common)
target_compile_features(compat_module_ixx PRIVATE cxx_std_23)

add_executable(compat_module_cpp)
target_sources(compat_module_cpp
  PRIVATE
    src/main.cpp
)
target_link_libraries(compat_module_cpp PRIVATE compat_common)
target_compile_definitions(compat_module_cpp
  PRIVATE
    USE_STD_MODULE=0
)
target_compile_features(compat_module_cpp PRIVATE cxx_std_23)
