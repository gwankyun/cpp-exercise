module;
#include <catch2_macro.h>

module main;
import std;
import a;
import catch2;
import spdlog;

unsigned int Factorial(unsigned int number)
{
    return number <= 1 ? number : Factorial(number - 1) * number;
}

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

void factorials_are_computed()
{
    REQUIRE(Factorial(1) == 1);
    REQUIRE(Factorial(2) == 2);
    REQUIRE(Factorial(3) == 6);
    REQUIRE(Factorial(10) == 3628800);
}

void vectors_can_be_sized_and_resized()
{
    using catch2::session;
    // This setup will be done 4 times in total, once for each section
    std::vector<int> v(5);

    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);

    session(
        "resizing bigger changes size and capacity",
        [&]
        {
            v.resize(10);

            REQUIRE(v.size() == 10);
            REQUIRE(v.capacity() >= 10);
        }
    );
    session(
        "resizing smaller changes size but not capacity",
        [&]
        {
            v.resize(0);

            REQUIRE(v.size() == 0);
            REQUIRE(v.capacity() >= 5);
        }
    );
    session(
        "reserving bigger changes capacity but not size",
        [&]
        {
            v.reserve(10);

            REQUIRE(v.size() == 5);
            REQUIRE(v.capacity() >= 10);
        }
    );
    session(
        "reserving smaller does not change size or capacity",
        [&]
        {
            v.reserve(0);

            REQUIRE(v.size() == 5);
            REQUIRE(v.capacity() >= 5);
        }
    );
}

int main(int _argc, char* _argv[])
{
    spdlog::get().info("module test");
    using catch2::test_case;
    std::vector<catch2::TestCase> testCase{
        test_case("a", "[a]", &test_add), test_case("vec", "[vector]", &test_vector),
        test_case(
            "base", "[base]",
            []
            {
                using namespace std::literals::string_literals;
                REQUIRE(1 + 2 == 3);
                REQUIRE("123"s + "456"s == "123456"s);
            }
        ),
        test_case("Factorials are computed", "[factorial]", &factorials_are_computed),
        test_case("vectors can be sized and resized", "[vector]", &vectors_can_be_sized_and_resized)
    };

    auto result = catch2::run(_argc, _argv);
    return result;
}
