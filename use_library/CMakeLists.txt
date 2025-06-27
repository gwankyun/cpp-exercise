set(CMAKE_FOLDER use_library)

add_library(object_lib OBJECT)
target_sources(object_lib PRIVATE
  src/shared_lib.cpp
  include/shared_lib.h
)
target_compile_definitions(object_lib PRIVATE DYN_LIB_EXPORT)
target_compile_features(object_lib PRIVATE cxx_std_20)

# 生成共享庫
add_library(shared_lib SHARED)
target_sources(shared_lib PRIVATE
  src/shared_lib.cpp
  include/shared_lib.h
)
target_compile_definitions(shared_lib PRIVATE DYN_LIB_EXPORT)
target_compile_features(shared_lib PRIVATE cxx_std_20)

# 生成靜態庫
add_library(static_lib STATIC)
block()
  set(lib static_lib)
  target_sources(${lib} PRIVATE
    src/static_lib.cpp
    include/static_lib.h
  )
  target_compile_features(${lib} PRIVATE cxx_std_20)
endblock()

# 生成模塊庫
add_library(module_lib MODULE)
target_sources(module_lib PRIVATE
  src/shared_lib.cpp
  include/shared_lib.h
)
target_compile_definitions(module_lib PRIVATE DYN_LIB_EXPORT)
target_compile_features(module_lib PRIVATE cxx_std_20)

# 調用庫
add_executable(shared_lib_test)
block()
  set(lib shared_lib_test)
  target_sources(${lib}
    PRIVATE
      include/shared_lib.h
      shared_lib_test.py
    PRIVATE FILE_SET CXX_MODULES FILES
      src/shared_lib_test.ixx
  )
  target_link_libraries(${lib}
    PRIVATE
      spdlog::module
      Catch2::module
      dylib::module
      shared_lib static_lib
  )
  target_compile_features(${lib} PRIVATE cxx_std_23)
endblock()
