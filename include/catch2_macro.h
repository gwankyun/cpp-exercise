#pragma once

#define STRING(_str) (#_str)

#define REQUIRE(_expression) catch2::require(_expression, STRING(_expression))

#define CHECK(_expression) catch2::check(_expression, STRING(_expression))

