#pragma once

#ifndef CATCH_STRING
#  define CATCH_STRING(_str) #_str
#endif // !CATCH_STRING

#ifndef CATCH_EXPRESSION
#  define CATCH_EXPRESSION(_expression) _expression, CATCH_STRING(_expression), Catch::current()
#endif // !CATCH_EXPRESSION

#if defined(CATCH_CONFIG_PREFIX_ALL)
#  define CATCH_REQUIRE(_expression) Catch::require(CATCH_EXPRESSION(_expression))
#  define CATCH_CHECK(_expression) Catch::check(CATCH_EXPRESSION(_expression))
#else
#  define REQUIRE(_expression) Catch::require(CATCH_EXPRESSION(_expression))
#  define CHECK(_expression) Catch::check(CATCH_EXPRESSION(_expression))
#endif

#ifndef CATCH_ON
#  define CATCH_ON(_exp) (CATCH_EXPRESSION(_exp))
#endif
