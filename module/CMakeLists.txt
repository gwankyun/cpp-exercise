set(CMAKE_FOLDER module)

add_library(spdlog_module)
target_sources(spdlog_module
  PRIVATE FILE_SET CXX_MODULES FILES
    spdlog.ixx)
target_link_libraries(spdlog_module PUBLIC spdlog::spdlog)
target_compile_features(spdlog_module PRIVATE cxx_std_23)
add_library(spdlog::module ALIAS spdlog_module)

add_library(catch2_module)
target_sources(catch2_module
  PRIVATE
    catch2/catch2/macro.h
    catch2/catch2/compat.h
  PRIVATE FILE_SET CXX_MODULES FILES
    catch2/catch2.ixx
    catch2/catch2.base.ixx
    catch2/catch2.session.ixx
    catch2/catch2.compat.ixx
)
target_include_directories(catch2_module PUBLIC catch2)
target_link_libraries(catch2_module PUBLIC Catch2::Catch2)
target_compile_features(catch2_module PRIVATE cxx_std_23)
add_library(Catch2::module ALIAS catch2_module)

add_library(boost_scope_module)
target_sources(boost_scope_module
  PRIVATE
    boost.scope/boost.scope/macro.h
  PRIVATE FILE_SET CXX_MODULES FILES
    boost.scope/boost.scope.ixx)
target_include_directories(boost_scope_module PUBLIC boost.scope)
target_link_libraries(boost_scope_module PUBLIC Boost::scope)
target_compile_features(boost_scope_module PRIVATE cxx_std_23)
add_library(Boost::scope::module ALIAS boost_scope_module)

add_library(dylib_module)
target_sources(dylib_module
  PRIVATE FILE_SET CXX_MODULES FILES
    dylib.ixx)
target_include_directories(dylib_module PRIVATE ${DYLIB_INCLUDE_DIRS})
target_compile_features(dylib_module PRIVATE cxx_std_23)
add_library(dylib::module ALIAS dylib_module)

add_library(a)
target_sources(a
  PRIVATE
    src/a.cpp
    src/a.h
  PRIVATE FILE_SET CXX_MODULES FILES
    src/a.ixx
)
target_compile_features(a PRIVATE cxx_std_23)

add_executable(catch2_module_example)
target_sources(catch2_module_example
  PRIVATE
    src/catch2_module_example.cpp
  PRIVATE FILE_SET CXX_MODULES FILES
    src/catch2_module_example.ixx
)
target_link_libraries(catch2_module_example
  PRIVATE
    Catch2::module)
target_compile_features(catch2_module_example PRIVATE cxx_std_23)

add_executable(catch2_module_compat_example)
target_sources(catch2_module_compat_example
  PRIVATE
    src/catch2_compat_example.cpp
  PRIVATE FILE_SET CXX_MODULES FILES
    src/catch2_compat_example.ixx
)
target_link_libraries(catch2_module_compat_example
  PRIVATE
    Catch2::module)
target_compile_features(catch2_module_compat_example PRIVATE cxx_std_23)

add_executable(module_main)
target_sources(module_main
  PRIVATE
    src/main.cpp
  PRIVATE FILE_SET CXX_MODULES FILES
    src/main.ixx
)
target_link_libraries(module_main
  PRIVATE
    spdlog::module
    Catch2::module
    Boost::scope::module
    a)
target_compile_features(module_main PRIVATE cxx_std_23)
