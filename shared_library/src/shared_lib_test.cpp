﻿#include <filesystem>
#include <string>

#include <catch2/../catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <dylib.hpp>
#include <spdlog/spdlog.h>

namespace shared_lib
{
#include "../include/shared_lib.h"
}

namespace fs = std::filesystem;

TEST_CASE("void* and any", "[ptr]")
{
    auto dll_path = fs::current_path() / "shared_lib.dll";
    REQUIRE(fs::exists(dll_path));
    if (!fs::exists(dll_path))
    {
        return;
    }

    dylib lib(dll_path);

    namespace sl = shared_lib;

    auto add = lib.get_function<decltype(sl::add)>("add");

    REQUIRE(add(1, 2) == 3);

    sl::Point pt;
    auto Point_init = lib.get_function<decltype(sl::Point_init)>("Point_init");
    Point_init(&pt);
    REQUIRE((pt.x == 0 && pt.y == 0));

    sl::Point pt1;
    Point_init(&pt1);

    sl::Point pt2;
    Point_init(&pt2);
    pt2.x = 3;
    pt2.y = 4;

    auto Point_distance = lib.get_function<decltype(sl::Point_distance)>("Point_distance");

    auto dist = Point_distance(&pt1, &pt2);
    REQUIRE(static_cast<int>(dist) == 5);
}

int main(int _argc, char* _argv[])
{
    std::string log_format{"[%C-%m-%d %T.%e] [%^%L%$] [%-20!!:%4#] %v"};
    spdlog::set_pattern(log_format);

    auto result = Catch::Session().run(_argc, _argv);
    return result;
}