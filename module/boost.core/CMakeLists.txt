add_library(boost_core_module)
target_sources(boost_core_module
  PRIVATE FILE_SET CXX_MODULES FILES
    boost.core.ixx
    boost.core.ignore_unused.ixx
)
target_include_directories(boost_core_module INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(boost_core_module
  PUBLIC Boost::core)
add_library(Boost::core::module ALIAS boost_core_module)
target_compile_features(boost_core_module PRIVATE cxx_std_23)

option(USE_BOOST_CORE_MODULE "使用boost.core模塊" ON)
add_library(boost_core_module_interface INTERFACE)
target_compile_definitions(boost_core_module_interface
  INTERFACE
    USE_BOOST_CORE_MODULE=$<BOOL:${USE_BOOST_CORE_MODULE}>
)
target_link_libraries(boost_core_module_interface
  INTERFACE
    $<IF:$<BOOL:${USE_BOOST_CORE_MODULE}>,Boost::core::module,Boost::core>
)
add_library(Boost::core::module::option ALIAS boost_core_module_interface)
