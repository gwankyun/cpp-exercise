#pragma once

#ifndef USE_STD_MODULE
#  define USE_STD_MODULE 1
#endif // !USE_STD_MODULE

#if !USE_STD_MODULE
#  include <print>
#endif

#ifndef EXPORT
#  if USE_STD_MODULE
#    define EXPORT export
#  else
#    define EXPORT
#  endif
#endif // !EXPORT
