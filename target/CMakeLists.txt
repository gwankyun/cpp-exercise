set(CMAKE_FOLDER target)

add_library(target_interface INTERFACE)
target_include_directories(target_interface INTERFACE include)
target_compile_features(target_interface INTERFACE cxx_std_23)

add_library(target_a)
target_sources(target_a
  PRIVATE
    src/target_a.cpp
    include/macro.h
  PRIVATE FILE_SET CXX_MODULES FILES
    src/target_a.ixx
)
# 設為公開可以傳遞
target_include_directories(target_a PUBLIC include)
# 設為公開可以傳遞
target_compile_features(target_a PUBLIC cxx_std_23)

# 源文件
get_target_property(a_src target_a SOURCES)
cmake_print_variables(a_src)

# 包含目錄
get_target_property(a_include target_a INCLUDE_DIRECTORIES)
cmake_print_variables(a_include)

# 特性
get_target_property(a_feature target_a COMPILE_FEATURES)
cmake_print_variables(a_feature)

add_library(target_b)
target_sources(target_b
  PRIVATE
    src/target_b.cpp
  PRIVATE FILE_SET CXX_MODULES FILES
    src/target_b.ixx
)
target_link_libraries(target_b PRIVATE target_a)

# 源文件
get_target_property(b_src target_b SOURCES)
cmake_print_variables(b_src)

# 包含目錄
get_target_property(b_include target_b INCLUDE_DIRECTORIES)
cmake_print_variables(b_include)

# 庫
get_target_property(b_lib target_b LINK_LIBRARIES)
cmake_print_variables(b_lib)

# 特性
get_target_property(b_feature target_b COMPILE_FEATURES)
cmake_print_variables(b_feature)

add_library(target_c)
target_sources(target_c
  PRIVATE
    src/target_c.cpp
  PRIVATE FILE_SET CXX_MODULES FILES
    src/target_c.ixx
)
target_link_libraries(target_c PRIVATE target_interface)
# 不能傳遞target_a的標準
# target_compile_features(target_c PRIVATE cxx_std_23)
