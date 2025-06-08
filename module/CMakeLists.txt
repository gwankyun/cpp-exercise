set(CMAKE_FOLDER module)

set(spdlog_module spdlog_module)
set(spdlog_module ${spdlog_module} PARENT_SCOPE)
add_library(${spdlog_module})
target_sources(${spdlog_module}
  PRIVATE FILE_SET CXX_MODULES FILES
    spdlog.ixx)
target_link_libraries(${spdlog_module} PUBLIC spdlog::spdlog)
target_compile_features(${spdlog_module} PRIVATE cxx_std_23)

set(catch2_module catch2_module)
set(catch2_module ${catch2_module} PARENT_SCOPE)
add_library(${catch2_module})
target_sources(${catch2_module}
  PRIVATE
    include/module/catch2/macro.h
  PRIVATE FILE_SET CXX_MODULES FILES
    catch2.ixx
    catch2.session.ixx)
target_include_directories(${catch2_module} PUBLIC include)
target_link_libraries(${catch2_module} PUBLIC Catch2::Catch2)
target_compile_features(${catch2_module} PRIVATE cxx_std_23)

set(boost_scope_module boost_scope_module)
set(boost_scope_module ${boost_scope_module} PARENT_SCOPE)
add_library(${boost_scope_module})
target_sources(${boost_scope_module}
  PRIVATE
    include/module/boost/scope/macro.h
  PRIVATE FILE_SET CXX_MODULES FILES
    boost.scope.ixx)
target_include_directories(${boost_scope_module} PUBLIC include)
target_link_libraries(${boost_scope_module} PUBLIC Boost::scope)
target_compile_features(${boost_scope_module} PRIVATE cxx_std_23)

set(dylib_module dylib_module)
set(dylib_module ${dylib_module} PARENT_SCOPE)
add_library(${dylib_module})
target_sources(${dylib_module}
  PRIVATE FILE_SET CXX_MODULES FILES
    dylib.ixx)
target_include_directories(${dylib_module} PRIVATE ${DYLIB_INCLUDE_DIRS})
target_compile_features(${dylib_module} PRIVATE cxx_std_23)

set(module_main module_main)
add_executable(${module_main})
target_sources(${module_main}
  PRIVATE
    src/main.cpp
    src/a.cpp
    src/a.h
  PRIVATE FILE_SET CXX_MODULES FILES
    src/main.ixx
    src/a.ixx
)
target_link_libraries(${module_main}
  PRIVATE spdlog_module catch2_module boost_scope_module)
target_compile_features(${module_main} PRIVATE cxx_std_23)
