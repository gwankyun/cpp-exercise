#pragma once
#include "static_lib_decl.hpp"

#if USE_COMPAT_LIB_MODULE
import std;
#endif

MODULE_EXPORT namespace compat_lib
{
    //struct Version
    //{
    //    std::uint64_t major;
    //    std::uint64_t minor;
    //    std::uint64_t patch;
    //};

    //Version version = {0, 0, 0};

    inline int add(int a, int b)
    {
        return a + b;
    }

    MODULE_INLINE std::string get_info()
    {
        return "compat_lib";
    }
} // namespace compat_lib
