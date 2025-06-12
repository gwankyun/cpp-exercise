add_library(dylib_module)
target_sources(dylib_module
  PRIVATE FILE_SET CXX_MODULES FILES
    dylib.ixx)
target_include_directories(dylib_module PRIVATE ${DYLIB_INCLUDE_DIRS})
target_compile_features(dylib_module PRIVATE cxx_std_23)
add_library(dylib::module ALIAS dylib_module)
