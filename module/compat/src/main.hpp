#pragma once
#include "global_module.hpp"

#if USE_COMPAT_LIB_MODULE
import compat_lib;
#endif

EXPORT int main(int _argc, char* _argv[])
{
    std::cout << "_args: " << _argc << std::endl;
    std::cout << "_argv[0]: " << _argv[0] << std::endl;
    std::cout << compat_lib::get_info() << std::endl;
    return 0;
}
