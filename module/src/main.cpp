module;
#include "../../include/catch2_macro.h"

module main;
import std;
import a;
import catch2;
import spdlog;

void test_add()
{
    REQUIRE(a::add(1, 2) == 3);
    CHECK(false);
    REQUIRE(a::add(1, 2) == 4);
}

void test_vector()
{
    std::vector<int> vec{1, 2, 3};
    REQUIRE(vec.size() == 3);
    vec.push_back(4);
    REQUIRE(vec.size() == 4);
}

int main(int _argc, char* _argv[])
{
    spdlog::info("module test");
    auto a = catch2::regist(&test_add, "a", "[a]");
    auto vec = catch2::regist(&test_vector, "vec", "[vec]");
    auto result = catch2::run(_argc, _argv);
    return result;
}
