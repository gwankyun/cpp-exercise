module;
#ifdef __INTELLISENSE__
#  include "a.h"
#  include <print>
#endif

module main;
#ifndef __INTELLISENSE__
import std;
import a;
#endif

int main()
{
    std::println("{}", a::add(1, 2));
    std::println("{}", a::get_info());
    return 0;
}
