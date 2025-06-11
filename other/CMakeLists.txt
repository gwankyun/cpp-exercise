set(CMAKE_FOLDER other)

add_executable(boost_example)
target_sources(boost_example PRIVATE
  src/boost_example.cpp
)
target_link_libraries(boost_example PRIVATE Boost::scope Catch2::Catch2)
target_compile_features(boost_example PRIVATE cxx_std_23)

add_executable(doctest_example)
target_sources(doctest_example PRIVATE
  src/doctest_example.cpp
)
target_link_libraries(doctest_example PRIVATE doctest::doctest)
target_compile_features(doctest_example PRIVATE cxx_std_23)

add_executable(intrusive_container)
target_sources(intrusive_container PRIVATE
  src/intrusive_container.cpp
)
target_link_libraries(intrusive_container PRIVATE spdlog::spdlog)

if((TARGET hello-imgui::hello_imgui) AND (TARGET nlohmann_json::nlohmann_json))
  add_executable(hello_imgui_example)
  target_sources(hello_imgui_example PRIVATE
    src/hello_imgui_example.cpp
  )
  target_link_libraries(hello_imgui_example PRIVATE
    hello-imgui::hello_imgui
    nlohmann_json::nlohmann_json
  )
  target_compile_features(hello_imgui_example PRIVATE cxx_std_23)
endif()

if(TARGET msft_proxy)
  add_executable(proxy_example)
  target_sources(proxy_example PRIVATE
    src/proxy_example.cpp
  )
  target_link_libraries(proxy_example PRIVATE spdlog::spdlog msft_proxy)
  target_compile_features(proxy_example PRIVATE cxx_std_23)
endif()

add_executable(catch2_example)
target_sources(catch2_example
  PRIVATE
    src/catch2_example.cpp
  PRIVATE FILE_SET CXX_MODULES FILES
    src/catch2_example.ixx
)
target_link_libraries(catch2_example PRIVATE Catch2::Catch2)
target_compile_features(catch2_example PRIVATE cxx_std_23)
