set(CMAKE_FOLDER data_structure)

set(common data_structure_common)
add_library(${common} INTERFACE)
target_link_libraries(${common} INTERFACE
  spdlog::spdlog Boost::scope Catch2::Catch2)
target_compile_features(${common} INTERFACE cxx_std_23)

set(tree_example tree_example)
add_executable(${tree_example})
target_sources(${tree_example} PRIVATE
  src/tree_example.cpp
)
target_link_libraries(${tree_example} PRIVATE ${common})

set(list_example list_example)
add_executable(${list_example})
target_sources(${list_example} PRIVATE
  src/list_example.cpp
)
target_link_libraries(${list_example} PRIVATE ${common})
