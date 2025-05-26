module;
#include <catch2/../catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

module main;
import std;
import a;

TEST_CASE("main", "[main]")
{
    REQUIRE(a::add(1, 2) == 3);
    REQUIRE(a::get_info() == "module a");
}

int main(int _argc, char* _argv[])
{
    auto result = Catch::Session().run(_argc, _argv);
    return result;
}
