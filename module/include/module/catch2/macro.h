#pragma once

#ifndef CATCH_STRING
#  define CATCH_STRING(_str) (#_str)
#endif // !CATCH_STRING

#if defined(CATCH_CONFIG_PREFIX_ALL)
#  define CATCH_REQUIRE(_expression) catch2::require(_expression, CATCH_STRING(_expression))
#  define CATCH_CHECK(_expression) catch2::check(_expression, CATCH_STRING(_expression))
#else
#  define REQUIRE(_expression) catch2::require(_expression, CATCH_STRING(_expression))
#  define CHECK(_expression) catch2::check(_expression, CATCH_STRING(_expression))
#endif
