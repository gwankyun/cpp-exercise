module;
#include "global_module.hpp"

export module compat_module;
#if USE_STD_MODULE
import std;
#endif

#include "main.hpp"
