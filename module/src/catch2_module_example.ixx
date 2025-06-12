module;
#include <catch2/macro.h>

export module catch2_module_example;
import std;
import catch2.modern;

#if !defined(ON) && defined(CATCH_ON)
#  define ON CATCH_ON
#else
#  error "ON can not define."
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

int test_add(int _i, int _j)
{
    return _i + _j;
}

export int main(int _argc, char* _argv[])
{
    using Catch::test_case;
    test_case(
        "without capture", "[lambda]",
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

    int value = 9;
    test_case(
        "with capture", "[lambda]",
        [&value]
        {
            c::check(false, "false", Catch::current());
            c::require(test_add(value, 1) == 10);
        }
    );

    auto result = Catch::Session().run(_argc, _argv);
    return result;
}
