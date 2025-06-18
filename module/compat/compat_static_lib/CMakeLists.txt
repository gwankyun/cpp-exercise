#[[
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
]]

option(USE_COMPAT_LIB_MODULE "compat_lib使用模塊" ON)

add_library(compat_lib_common INTERFACE)
target_sources(compat_lib_common
  INTERFACE
    src/static_lib_global.hpp
    src/static_lib.hpp
)

add_library(compat_lib_header STATIC)
target_sources(compat_lib_header
  PRIVATE
    src/static_lib.cpp
    src/static_lib.h
)
target_include_directories(compat_lib_header INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(compat_lib_header
  PUBLIC
    USE_COMPAT_LIB_MODULE=0
)
target_link_libraries(compat_lib_header
  PRIVATE
    compat_lib_common
)
target_compile_features(compat_lib_header
  PRIVATE
    cxx_std_17
)

add_library(compat_lib_module STATIC)
target_sources(compat_lib_module
  PRIVATE
    src/static_lib.h
  PRIVATE FILE_SET CXX_MODULES FILES
    src/static_lib.ixx
)
target_include_directories(compat_lib_module INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(compat_lib_module
  PUBLIC
    USE_COMPAT_LIB_MODULE=1
)
target_link_libraries(compat_lib_module
  PRIVATE
    compat_lib_common
)
target_compile_features(compat_lib_module
  PRIVATE
    cxx_std_23
)

#[[
add_library(compat_lib STATIC)
target_sources(compat_lib
  PRIVATE
    $<$<NOT:$<BOOL:${USE_COMPAT_LIB_MODULE}>>:src/static_lib.cpp>
    src/static_lib_global.hpp
    src/static_lib.h
    src/static_lib.hpp
)
if(${USE_COMPAT_LIB_MODULE})
target_sources(compat_lib
  PRIVATE FILE_SET CXX_MODULES FILES
    $<$<BOOL:${USE_COMPAT_LIB_MODULE}>:src/static_lib.ixx>
)
endif()
target_include_directories(compat_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(compat_lib
  PUBLIC
    USE_COMPAT_LIB_MODULE=$<BOOL:${USE_COMPAT_LIB_MODULE}>
)
target_compile_features(compat_lib
  PRIVATE
    $<IF:$<BOOL:${USE_COMPAT_LIB_MODULE}>,cxx_std_23,cxx_std_17>
)
]]

add_library(compat_lib INTERFACE)
target_include_directories(compat_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(compat_lib
  INTERFACE
    $<IF:$<BOOL:${USE_COMPAT_LIB_MODULE}>,compat_lib_module,compat_lib_header>
)
target_compile_definitions(compat_lib
  INTERFACE
    USE_COMPAT_LIB_MODULE=$<BOOL:${USE_COMPAT_LIB_MODULE}>
)
target_compile_features(compat_lib
  INTERFACE
    $<IF:$<BOOL:${USE_COMPAT_LIB_MODULE}>,cxx_std_23,cxx_std_17>
)
