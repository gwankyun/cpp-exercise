module;
#ifdef __INTELLISENSE__
#  include "a.h"
#endif

export module a;
#ifndef __INTELLISENSE__
import std;
#endif

export {
    namespace a
    {
        int add(int _a, int _b);
        std::string get_info();
    } // namespace m
}
