set(CMAKE_FOLDER data_structure)

add_library(data_structure_common INTERFACE)
target_link_libraries(data_structure_common INTERFACE
  spdlog::spdlog Boost::scope Catch2::Catch2)
target_compile_features(data_structure_common INTERFACE cxx_std_23)

add_executable(tree_example)
target_sources(tree_example PRIVATE
  src/tree_example.cpp
)
target_link_libraries(tree_example PRIVATE data_structure_common)

add_executable(list_example)
target_sources(list_example PRIVATE
  src/list_example.cpp
)
target_link_libraries(list_example PRIVATE data_structure_common)
