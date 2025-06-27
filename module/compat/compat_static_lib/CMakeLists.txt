option(USE_COMPAT_LIB_MODULE "compat_lib使用模塊" ON)

add_library(compat_lib_common INTERFACE)
block()
  target_sources(compat_lib_common
    INTERFACE
      src/static_lib_decl.hpp
      src/static_lib_impl.hpp
  )
endblock()

add_library(compat_lib_header)
block()
  target_sources(compat_lib_header
    PRIVATE
      src/static_lib.cpp
      src/static_lib.h
  )
  target_include_directories(compat_lib_header
    INTERFACE
      ${CMAKE_CURRENT_SOURCE_DIR}/src
  )
  target_compile_definitions(compat_lib_header
    PUBLIC
      USE_COMPAT_LIB_MODULE=0
      USE_COMPAT_LIB_HEADER_ONLY=0
  )
  target_link_libraries(compat_lib_header
    PRIVATE
      compat_lib_common
  )
  target_compile_features(compat_lib_header
    PRIVATE
      cxx_std_17
  )
endblock()

add_library(compat_lib_module)
block()
  target_sources(compat_lib_module
    PRIVATE
      src/static_lib.h
    PRIVATE FILE_SET CXX_MODULES FILES
      src/static_lib.ixx
  )
  target_include_directories(compat_lib_module
    INTERFACE
      ${CMAKE_CURRENT_SOURCE_DIR}/src
  )
  target_compile_definitions(compat_lib_module
    PUBLIC
      USE_COMPAT_LIB_MODULE=1
      USE_COMPAT_LIB_HEADER_ONLY=0
  )
  target_link_libraries(compat_lib_module
    PRIVATE
      compat_lib_common
  )
  target_compile_features(compat_lib_module
    PRIVATE
      cxx_std_23
  )
endblock()

add_library(compat_lib_header_only_module)
block()
  set(lib compat_lib_header_only_module)
  target_sources(${lib}
    PRIVATE
      src/static_lib.hpp
    PRIVATE FILE_SET CXX_MODULES FILES
      src/static_lib.ixx
  )
  target_include_directories(${lib}
    INTERFACE
      ${CMAKE_CURRENT_SOURCE_DIR}/src
  )
  target_compile_definitions(${lib}
    PUBLIC
      USE_COMPAT_LIB_MODULE=1
      USE_COMPAT_LIB_HEADER_ONLY=1
  )
  target_link_libraries(${lib}
    PRIVATE
      compat_lib_common
  )
  target_compile_features(${lib}
    PRIVATE
      cxx_std_23
  )
endblock()

add_library(compat_lib INTERFACE)
block()
  target_include_directories(compat_lib
    INTERFACE
      ${CMAKE_CURRENT_SOURCE_DIR}/src
  )
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
endblock()
