#include <cassert>
#include <iostream>
#include <string>

#include <boost/scope/defer.hpp>
#include <catch2/../catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("BOOST_SCOPE_DEFER", "[defer]")
{
    std::string result;
    {
        BOOST_SCOPE_DEFER[&result]
        {
            result += "2";
        };
        result += "1";
    }
    REQUIRE(result == "12");
}

int main(int _argc, char* _argv[])
{
    auto result = Catch::Session().run(_argc, _argv);
    return result;
}
