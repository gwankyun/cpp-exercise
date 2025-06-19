set(CMAKE_FOLDER module)

block()
  set(CMAKE_FOLDER module/binding)

  add_subdirectory(spdlog)

  add_subdirectory(catch2)

  add_subdirectory(boost.scope)

  add_subdirectory(boost.core)

  add_subdirectory(boost.intrusive)

  add_subdirectory(dylib)
endblock()

add_library(a)
target_sources(a
  PRIVATE
    src/a.cpp
    src/a.h
  PRIVATE FILE_SET CXX_MODULES FILES
    src/a.ixx
)
target_compile_features(a PRIVATE cxx_std_23)

set(USE_CATCH2_MODERN OFF CACHE BOOL "use catch2.modern")
set(USE_CATCH2_COMPAT OFF CACHE BOOL "use catch2.compat")

add_executable(catch2_module_example)
target_sources(catch2_module_example
  PRIVATE FILE_SET CXX_MODULES FILES
    src/catch2_module_example.ixx
)
target_link_libraries(catch2_module_example
  PRIVATE
    Catch2::module)
target_compile_features(catch2_module_example PRIVATE cxx_std_23)

add_executable(catch2_module_compat_example)
target_sources(catch2_module_compat_example
  PRIVATE FILE_SET CXX_MODULES FILES
    src/catch2_compat_example.ixx
)
target_link_libraries(catch2_module_compat_example
  PRIVATE
    Catch2::module)
target_compile_features(catch2_module_compat_example PRIVATE cxx_std_23)

add_executable(module_main)
target_sources(module_main
  PRIVATE FILE_SET CXX_MODULES FILES
    src/main.ixx
)
target_link_libraries(module_main
  PRIVATE
    spdlog::module::option
    Catch2::module::option
    Boost::scope::module::option
    Boost::core::module::option
    Boost::intrusive::module::option
    a
)
target_compile_features(module_main PRIVATE cxx_std_23)

add_executable(single_main)
target_sources(single_main
  PRIVATE FILE_SET CXX_MODULES FILES
    src/single_main.ixx
)
target_compile_features(single_main PRIVATE cxx_std_23)

#option(USE_STD_MODULE "使用標準庫模塊" ON)

add_library(std_module_interface INTERFACE)
target_compile_definitions(std_module_interface
  INTERFACE
    USE_STD_MODULE=$<BOOL:${USE_STD_MODULE}>
)

add_subdirectory(compat)
