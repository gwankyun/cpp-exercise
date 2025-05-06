set(CMAKE_FOLDER use_library)

set(object_lib object_lib)
add_library(${object_lib} OBJECT)
target_sources(${object_lib} PRIVATE
  src/shared_lib.cpp
  include/shared_lib.h
)
target_compile_definitions(${object_lib} PRIVATE DYN_LIB_EXPORT)
target_compile_features(${object_lib} PRIVATE cxx_std_20)

# 生成共享庫
set(shared_lib shared_lib)
add_library(${shared_lib} SHARED)
target_compile_definitions(${shared_lib} PRIVATE DYN_LIB_EXPORT)
target_link_libraries(${shared_lib} PRIVATE ${object_lib})
target_compile_features(${shared_lib} PRIVATE cxx_std_20)

# 生成靜態庫
set(static_lib static_lib)
add_library(${static_lib} STATIC)
target_sources(${static_lib} PRIVATE
  src/static_lib.cpp
  include/static_lib.h
)
target_compile_features(${static_lib} PRIVATE cxx_std_20)

# 生成模塊庫
set(module_lib module_lib)
add_library(${module_lib} MODULE)
target_link_libraries(${module_lib} PRIVATE ${object_lib})
target_compile_definitions(${module_lib} PRIVATE DYN_LIB_EXPORT)
target_compile_features(${module_lib} PRIVATE cxx_std_20)

# 調用庫
set(shared_lib_test shared_lib_test)
add_executable(${shared_lib_test})
target_sources(${shared_lib_test} PRIVATE
  src/shared_lib_test.cpp
  include/shared_lib.h
  shared_lib_test.py
)
target_include_directories(${shared_lib_test} PRIVATE ${DYLIB_INCLUDE_DIRS})
target_link_libraries(${shared_lib_test} PRIVATE
  spdlog::spdlog Catch2::Catch2 ${shared_lib} ${static_lib})
target_compile_features(${shared_lib_test} PRIVATE cxx_std_20)
