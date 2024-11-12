# 生成動態庫
set(shared_lib shared_lib)
add_library(${shared_lib} SHARED)
target_sources(${shared_lib} PRIVATE
  src/shared_lib.cpp
  include/shared_lib.h
)
target_compile_definitions(${shared_lib} PRIVATE DLL_EXPORT)
target_compile_features(${shared_lib} PRIVATE cxx_std_20)

# 調用動態庫
set(shared_lib_test shared_lib_test)
add_executable(${shared_lib_test})
target_sources(${shared_lib_test} PRIVATE
  src/shared_lib_test.cpp
  include/shared_lib.h
)
target_include_directories(${shared_lib_test} PRIVATE ${DYLIB_INCLUDE_DIRS})
target_link_libraries(${shared_lib_test} PRIVATE spdlog::spdlog Catch2::Catch2)
target_compile_features(${shared_lib_test} PRIVATE cxx_std_20)
