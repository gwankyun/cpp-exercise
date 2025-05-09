module;
#ifdef __INTELLISENSE__
#  include "a.h"
#endif

module a;
#ifndef __INTELLISENSE__
import std;
#endif

namespace a
{
    int add(int _a, int _b)
    {
        return _a + _b;
    }

    std::string get_info()
    {
        return "module a";
    }
} // namespace m
