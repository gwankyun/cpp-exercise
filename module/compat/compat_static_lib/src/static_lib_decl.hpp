#pragma once

#ifndef USE_COMPAT_LIB_MODULE
#  define USE_COMPAT_LIB_MODULE 1
#endif // !USE_COMPAT_LIB_MODULE

#if !USE_COMPAT_LIB_MODULE
#  include <string>
#endif

#ifndef MODULE_EXPORT
#  if USE_COMPAT_LIB_MODULE
#    define MODULE_EXPORT export
#  else
#    define MODULE_EXPORT
#  endif
#endif // !MODULE_EXPORT

#ifndef MODULE_INLINE
#  if USE_COMPAT_LIB_MODULE
#    define MODULE_INLINE
#  else
#    define MODULE_INLINE inline
#  endif
#endif // !MODULE_INLINE

