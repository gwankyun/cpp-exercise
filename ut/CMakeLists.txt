add_executable(ut)
target_sources(ut
  PRIVATE
    src/main.cpp
    include/boost/ut.hpp
  PRIVATE FILE_SET CXX_MODULES FILES
    src/main.ixx
    include/boost/ut.ixx
)
target_link_libraries(ut PRIVATE spdlog::spdlog)
target_compile_features(ut PRIVATE cxx_std_23)
