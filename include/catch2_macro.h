#pragma once

#define STRING(_str) (#_str)

#define REQUIRE(_expression) catch2_module::require(_expression, STRING(_expression))

#define CHECK(_expression) catch2_module::check(_expression, STRING(_expression))

