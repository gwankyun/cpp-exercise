module;
#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/spdlog.h>

export module spdlog;
import std;

std::string format(std::source_location _location)
{
    return std::format("[{}:{}]", _location.function_name(), _location.line());
}

struct Location
{
    std::source_location location;

    template <typename... Args>
    void trace(const std::format_string<Args...> fmt, Args&&... args)
    {
        auto message = std::vformat(fmt.get(), std::make_format_args(args...));
        spdlog::trace("{} {}", format(this->location), message);
    }

    template <typename... Args>
    void debug(const std::format_string<Args...> fmt, Args&&... args)
    {
        auto message = std::vformat(fmt.get(), std::make_format_args(args...));
        spdlog::debug("{} {}", format(this->location), message);
    }

    template <typename... Args>
    void info(const std::format_string<Args...> fmt, Args&&... args)
    {
        auto message = std::vformat(fmt.get(), std::make_format_args(args...));
        spdlog::info("{} {}", format(this->location), message);
    }

    template <typename... Args>
    void warn(const std::format_string<Args...> fmt, Args&&... args)
    {
        auto message = std::vformat(fmt.get(), std::make_format_args(args...));
        spdlog::warn("{} {}", format(this->location), message);
    }

    template <typename... Args>
    void error(const std::format_string<Args...> fmt, Args&&... args)
    {
        auto message = std::vformat(fmt.get(), std::make_format_args(args...));
        spdlog::error("{} {}", format(this->location), message);
    }

    template <typename... Args>
    void critical(const std::format_string<Args...> fmt, Args&&... args)
    {
        auto message = std::vformat(fmt.get(), std::make_format_args(args...));
        spdlog::critical("{} {}", format(this->location), message);
    }
};

export namespace spdlog
{
    Location get(const std::source_location location = std::source_location::current())
    {
        Location current;
        current.location = location;
        return current;
    }

    using spdlog::critical;
    using spdlog::debug;
    using spdlog::error;
    using spdlog::info;
    using spdlog::trace;
    using spdlog::warn;

    using spdlog::to_hex;
    using spdlog::set_pattern;
} // namespace spdlog_module
