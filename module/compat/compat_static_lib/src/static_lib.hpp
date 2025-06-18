#pragma once
#include "static_lib_global.hpp"

#if USE_COMPAT_LIB_MODULE
import std;
#endif

EXPORT namespace compat_lib
{
    std::string get_info()
    {
        return "static lib";
    }
} // namespace static_lib
