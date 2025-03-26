#include <filesystem>
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
    auto dll_path = fs::current_path() / "bin/shared_lib.dll";
    CHECK(fs::exists(dll_path));

    dylib lib(dll_path);

    namespace sl = shared_lib;

    auto add = lib.get_function<decltype(sl::add)>("add");
    CHECK(add != nullptr);
    REQUIRE(add(1, 2) == 3);

    auto Point_init = lib.get_function<decltype(sl::Point_init)>("Point_init");
    CHECK(Point_init != nullptr);
    {
        sl::Point pt;
        Point_init(&pt);
        REQUIRE((pt.x == 0 && pt.y == 0));
    }

    auto Point_distance = lib.get_function<decltype(sl::Point_distance)>("Point_distance");
    CHECK(Point_distance != nullptr);
    {
        sl::Point pt1;
        Point_init(&pt1);

        sl::Point pt2;
        Point_init(&pt2);
        pt2.x = 3;
        pt2.y = 4;

        auto dist = Point_distance(&pt1, &pt2);
        REQUIRE(static_cast<int>(dist) == 5);
    }

    auto set_int = lib.get_function<decltype(sl::set_int)>("set_int");
    CHECK(set_int != nullptr);
    {
        int v = 0;
        set_int(&v, 1);
        REQUIRE(v == 1);
    }

    auto set_char_p = lib.get_function<decltype(sl::set_char_p)>("set_char_p");
    CHECK(set_char_p != nullptr);
    {
        char* str = nullptr;
        set_char_p(&str, "123");
        REQUIRE(memcmp(str, "123", 3) == 0);
    }

    auto set_struct = lib.get_function<decltype(sl::set_struct)>("set_struct");
    CHECK(set_struct != nullptr);
    {
        sl::Point pt;
        set_struct(&pt, 3, 4);
        REQUIRE((pt.x == 3 && pt.y == 4));
    }

    auto set_array = lib.get_function<decltype(sl::set_array)>("set_array");
    CHECK(set_array != nullptr);
    {
        int a[10];
        set_array(a, 3, 4);
        REQUIRE(a[3] == 4);
    }
}

int main(int _argc, char* _argv[])
{
    std::string log_format{"[%C-%m-%d %T.%e] [%^%L%$] [%-20!!:%4#] %v"};
    spdlog::set_pattern(log_format);

    auto result = Catch::Session().run(_argc, _argv);
    return result;
}
