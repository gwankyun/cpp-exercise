#pragma once

#ifndef USE_COMPAT_LIB_MODULE
#  define USE_COMPAT_LIB_MODULE 1
#endif // !USE_COMPAT_LIB_MODULE

#if !USE_COMPAT_LIB_MODULE
#  include <string>
#endif

#ifndef EXPORT
#  if USE_COMPAT_LIB_MODULE
#    define EXPORT export
#  else
#    define EXPORT
#  endif
#endif // !EXPORT
