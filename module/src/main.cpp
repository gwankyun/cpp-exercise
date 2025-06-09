module;
#include <module/boost/scope/macro.h>
#include <module/catch2/macro.h>

module main;
import std;
import a;
import catch2;
import spdlog;
import boost.scope;

#ifndef ON
#  define ON(_exp) (CATCH_EXPRESSION(_exp))
#endif

namespace c = Catch;

unsigned int Factorial(unsigned int number)
{
    return number <= 1 ? number : Factorial(number - 1) * number;
}

void factorials_are_computed()
{
    c::require ON(Factorial(1) == 1);
    c::require ON(Factorial(2) == 2);
    c::require ON(Factorial(3) == 6);
    c::require ON(Factorial(10) == 3628800);
}

void test_add()
{
    c::require ON(a::add(1, 2) == 3);
    c::check ON(false);
    c::require ON(a::add(1, 2) == 4);
}

void test_vector()
{
    std::vector<int> vec{1, 2, 3};
    c::require ON(vec.size() == 3);
    vec.push_back(4);
    c::require ON(vec.size() == 4);
}

void vectors_can_be_sized_and_resized()
{
    using Catch::session;
    // This setup will be done 4 times in total, once for each section
    std::vector<int> v(5);

    c::require ON(v.size() == 5);
    c::require ON(v.capacity() >= 5);

    session(
        "resizing bigger changes size and capacity",
        [&]
        {
            v.resize(10);

            c::require ON(v.size() == 10);
            c::require ON(v.capacity() >= 10);
        }
    );
    session(
        "resizing smaller changes size but not capacity",
        [&]
        {
            v.resize(0);

            c::require ON(v.size() == 0);
            c::require ON(v.capacity() >= 5);
        }
    );
    session(
        "reserving bigger changes capacity but not size",
        [&]
        {
            v.reserve(10);

            c::require ON(v.size() == 5);
            c::require ON(v.capacity() >= 10);
        }
    );
    session(
        "reserving smaller does not change size or capacity",
        [&]
        {
            v.reserve(0);

            c::require ON(v.size() == 5);
            c::require ON(v.capacity() >= 5);
        }
    );
}

void test_defer()
{
    int n = 1;
    {
        BOOST_SCOPE_DEFER[&]
        {
            n++;
            spdlog::get().info("");
        };
        c::require ON(n == 1);
    }
    c::require ON(n == 2);
}

int main(int _argc, char* _argv[])
{
    spdlog::get().info("module test");
    using Catch::test_case;
    test_case("a", "[a]", &test_add);
    test_case("vec", "[vector]", &test_vector);
    test_case(
        "base", "[base]",
        []
        {
            using namespace std::literals::string_literals;
            c::require ON(1 + 2 == 3);
            c::require ON("123"s + "456"s == "123456"s);
        }
    );
    test_case("Factorials are computed", "[factorial]", &factorials_are_computed);
    test_case("vectors can be sized and resized", "[vector]", &vectors_can_be_sized_and_resized);

    {
        test_case("lambda", "[lambda]", [] { c::require ON(1 + 2 == 3); });
    }

    test_case("scope", "[boost]", test_defer);

    test_case(
        "without macro", "[lambda]",
        []
        {
            c::require ON(a::add(1, 2) == 3);
            c::check(false, "false", Catch::current());
            c::require ON(a::add(1, 2) == 4);
        }
    );

    auto result = Catch::Session().run(_argc, _argv);
    return result;
}
