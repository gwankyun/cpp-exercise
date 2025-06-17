set(CMAKE_FOLDER module)

add_library(compat_static_lib_common INTERFACE)
target_sources(compat_static_lib_common
  INTERFACE
    src/static_lib_global.hpp
    src/static_lib.hpp
)
add_library(compat_static_lib::common ALIAS compat_static_lib_common)

add_library(compat_static_lib_cpp STATIC)
target_sources(compat_static_lib_cpp
  PRIVATE
    src/static_lib.cpp
    src/static_lib.h
)
target_link_libraries(compat_static_lib_cpp
  PRIVATE compat_static_lib::common)
target_compile_definitions(compat_static_lib_cpp
  PRIVATE
    USE_STD_MODULE=0
)
target_compile_features(compat_static_lib_cpp PRIVATE cxx_std_20)

add_library(compat_static_lib_ixx STATIC)
target_sources(compat_static_lib_ixx
  PRIVATE FILE_SET CXX_MODULES FILES
    src/static_lib.ixx
)
target_link_libraries(compat_static_lib_ixx
  PRIVATE compat_static_lib::common)
target_compile_definitions(compat_static_lib_ixx
  PRIVATE
    USE_STD_MODULE=1
)
target_compile_features(compat_static_lib_ixx PRIVATE cxx_std_23)
