#pragma once

#ifndef USE_COMPAT_MAIN_MODULE
#  define USE_COMPAT_MAIN_MODULE 1
#endif // !USE_COMPAT_MAIN_MODULE

#if !USE_COMPAT_MAIN_MODULE
#  include <iostream>
#endif

#if !USE_COMPAT_LIB_MODULE
#include <static_lib.h>
#endif

#ifndef EXPORT
#  if USE_COMPAT_MAIN_MODULE
#    define EXPORT export
#  else
#    define EXPORT
#  endif
#endif // !EXPORT
