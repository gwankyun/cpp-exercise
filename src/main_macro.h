#pragma once

#ifndef PRAGMA_SUPPORTED
#  define PRAGMA_SUPPORTED 1
#endif

#ifndef PRAGMA
#  if PRAGMA_SUPPORTED
#    define PRAGMA(x) _Pragma(#x)
#  else
#    define PRAGMA(x) __pragma(x)
#  endif
#endif

#ifndef PRAGMA_WARNING_PUSH
#  define PRAGMA_WARNING_PUSH() PRAGMA(warning(push))
#endif

#ifndef PRAGMA_WARNING_POP
#  define PRAGMA_WARNING_POP() PRAGMA(warning(pop))
#endif

#ifndef PRAGMA_WARNING_DISABLE
#  ifdef _MSC_VER
#    define PRAGMA_WARNING_DISABLE(x) PRAGMA(warning(disable : x))
#  else
#    define PRAGMA_WARNING_DISABLE(x)
#  endif
#endif

#ifndef ZERO_SIZE_ARRAY_SUPPORTED
#  define ZERO_SIZE_ARRAY_SUPPORTED 1
#endif

#ifndef ZERO_SIZE_ARRAY
#  if ZERO_SIZE_ARRAY_SUPPORTED
#    define ZERO_SIZE_ARRAY(type, name) type name[0]
#  else
#    define ZERO_SIZE_ARRAY(type, name)
#  endif
#endif
