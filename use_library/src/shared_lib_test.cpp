module;
#include <module/catch2/macro.h>
#include "../include/shared_lib.h"
#include "../include/static_lib.h"

module shared_lib_test;
import std;
import spdlog;
import catch2;
import dylib;

namespace fs = std::filesystem;

void test_shared_lib()
{
    REQUIRE(add(1, 2) == 3);

    {
        Point pt;
        Point_init(&pt);
        REQUIRE((pt.x == 0 && pt.y == 0));
    }

    {
        Point pt1;
        Point_init(&pt1);

        Point pt2;
        Point_init(&pt2);
        pt2.x = 3;
        pt2.y = 4;

        auto dist = Point_distance(&pt1, &pt2);
        REQUIRE(static_cast<int>(dist) == 5);
    }

    {
        int v = 0;
        set_int(&v, 1);
        REQUIRE(v == 1);
    }

    {
        char* str = nullptr;
        set_char_p(&str, "123");
        REQUIRE(memcmp(str, "123", 3) == 0);
    }

    {
        Point pt;
        set_struct(&pt, 3, 4);
        REQUIRE((pt.x == 3 && pt.y == 4));
    }

    {
        int a[10];
        set_array(a, 3, 4);
        REQUIRE(a[3] == 4);
    }
}

void test_static_lib()
{
    REQUIRE(static_lib::get_info() == "static lib");
}

void test_module_lib()
{
    auto dll_path = fs::current_path() / "module_lib.dll";
    spdlog::get().info("dll_path: {}", dll_path.string());
    CHECK(fs::exists(dll_path));

    dylib lib(dll_path);

    auto f_add = lib.get_function<decltype(add)>("add");
    CHECK(f_add != nullptr);
    REQUIRE(f_add(1, 2) == 3);

    auto f_Point_init = lib.get_function<decltype(Point_init)>("Point_init");
    CHECK(f_Point_init != nullptr);
    {
        Point pt;
        f_Point_init(&pt);
        REQUIRE((pt.x == 0 && pt.y == 0));
    }

    auto f_Point_distance = lib.get_function<decltype(Point_distance)>("Point_distance");
    CHECK(f_Point_distance != nullptr);
    {
        Point pt1;
        Point_init(&pt1);

        Point pt2;
        Point_init(&pt2);
        pt2.x = 3;
        pt2.y = 4;

        auto dist = f_Point_distance(&pt1, &pt2);
        REQUIRE(static_cast<int>(dist) == 5);
    }

    auto f_set_int = lib.get_function<decltype(set_int)>("set_int");
    CHECK(f_set_int != nullptr);
    {
        int v = 0;
        f_set_int(&v, 1);
        REQUIRE(v == 1);
    }

    auto f_set_char_p = lib.get_function<decltype(set_char_p)>("set_char_p");
    CHECK(f_set_char_p != nullptr);
    {
        char* str = nullptr;
        f_set_char_p(&str, "123");
        REQUIRE(memcmp(str, "123", 3) == 0);
    }

    auto f_set_struct = lib.get_function<decltype(set_struct)>("set_struct");
    CHECK(f_set_struct != nullptr);
    {
        Point pt;
        f_set_struct(&pt, 3, 4);
        REQUIRE((pt.x == 3 && pt.y == 4));
    }

    auto f_set_array = lib.get_function<decltype(set_array)>("set_array");
    CHECK(f_set_array != nullptr);
    {
        int a[10];
        f_set_array(a, 3, 4);
        REQUIRE(a[3] == 4);
    }
}

int main(int _argc, char* _argv[])
{
    std::string log_format{"[%C-%m-%d %T.%e] [%^%L%$] [%-20!!:%4#] %v"};
    spdlog::set_pattern(log_format);

    using Catch::test_case;
    test_case("static_lib", "[static_lib]", test_static_lib);
    test_case("shared_lib", "[shared_lib]", test_shared_lib);
    test_case("module_lib", "[dylib]", test_module_lib);

    auto result = Catch::Session().run(_argc, _argv);
    return result;
}
