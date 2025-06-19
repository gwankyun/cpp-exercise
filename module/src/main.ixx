module;

#if !USE_CATCH2_MODULE
#  include <catch2/../catch2/catch_session.hpp>
#else
#  include <catch2/macro.h>
#endif
// clang-format off
#include <catch2/catch_test_macros.hpp>
// clang-format on

#include <boost/scope/defer.hpp>
#include <spdlog/spdlog.h>

#if !USE_BOOST_INTRUSIVE_MODULE
#  include <boost/intrusive/list.hpp>
#endif

#if !USE_BOOST_CORE_MODULE
#  include <boost/core/ignore_unused.hpp>
#endif

export module main;
import std;
import a;

#if USE_CATCH2_MODULE
import catch2.modern;
import catch2.compat;
#endif

#if USE_SPDLOG_MODULE
import spdlog;
#endif

#if USE_BOOST_SCOPE_MODULE
import boost.scope;
#endif

#if USE_BOOST_CORE_MODULE
import boost.core;
#endif

#if USE_BOOST_INTRUSIVE_MODULE
import boost.intrusive;
#endif

#if USE_CATCH2_MODULE
#  if !defined(ON) && defined(CATCH_ON)
#    define ON CATCH_ON
#  else
#    error "ON can not define."
#  endif
#endif

namespace c = Catch;
namespace intrusive = boost::intrusive;

class MyClass : public intrusive::list_base_hook<intrusive::link_mode<intrusive::safe_link>>
{
  public:
    MyClass(int value) : value_(value)
    {
    }
    int getValue() const
    {
        return value_;
    }

  private:
    int value_;
};

unsigned int Factorial(unsigned int number)
{
    return number <= 1 ? number : Factorial(number - 1) * number;
}

#if USE_CATCH2_MODULE
void factorials_are_computed()
{
    c::require ON(Factorial(1) == 1);
    c::require ON(Factorial(2) == 2);
    c::require ON(Factorial(3) == 6);
    c::require ON(Factorial(10) == 3628800);
}
#endif

TEST_CASE("Factorials are computed compatibility", "[factorial]")
{
    REQUIRE(Factorial(1) == 1);
    REQUIRE(Factorial(2) == 2);
    REQUIRE(Factorial(3) == 6);
    REQUIRE(Factorial(10) == 3628800);
}

TEST_CASE("vectors can be sized and resized compatibility", "[vector]")
{
    // This setup will be done 4 times in total, once for each section
    std::vector<int> v(5);

    REQUIRE(v.size() == 5);
    REQUIRE(v.capacity() >= 5);

    SECTION("resizing bigger changes size and capacity")
    {
        v.resize(10);

        REQUIRE(v.size() == 10);
        REQUIRE(v.capacity() >= 10);
    }
    SECTION("resizing smaller changes size but not capacity")
    {
        v.resize(0);

        REQUIRE(v.size() == 0);
        REQUIRE(v.capacity() >= 5);
    }
    SECTION("reserving bigger changes capacity but not size")
    {
        v.reserve(10);

        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() >= 10);
    }
    SECTION("reserving smaller does not change size or capacity")
    {
        v.reserve(0);

        REQUIRE(v.size() == 5);
        REQUIRE(v.capacity() >= 5);
    }
}

TEST_CASE("intrusive", "[boost]")
{
    boost::intrusive::list<MyClass> myList;
    MyClass obj1(1);
    MyClass obj2(2);
    MyClass obj3(3);

    myList.push_back(obj1);
    myList.push_back(obj2);
    myList.push_back(obj3);
    REQUIRE(myList.size() == 3);
}

#if USE_CATCH2_MODULE
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
#endif

export int main(int _argc, char* _argv[])
{
    int ununsed = 0;
    boost::ignore_unused(ununsed);
    SPDLOG_INFO("module test");
#if USE_CATCH2_MODULE
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
#endif

    auto result = Catch::Session().run(_argc, _argv);
    return result;
}
