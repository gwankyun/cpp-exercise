module;
#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/spdlog.h>

export module spdlog;
import std;

class Logger
{
  public:
    Logger(
        spdlog::logger* _logger = spdlog::default_logger_raw(),
        const std::source_location _location = std::source_location::current()
    )
        : logger(_logger), location(_location)
    {
    }
    ~Logger() = default;

    template <typename... Args>
    void log(spdlog::level::level_enum _level, const std::format_string<Args...> fmt, Args&&... args)
    {
        auto message = std::vformat(fmt.get(), std::make_format_args(args...));
        auto file_name = location.file_name();
        auto line = location.line();
        auto function_name = location.function_name();
        spdlog::source_loc loc(file_name, line, function_name);
        logger->log(loc, _level, message);
    }

    template <typename... Args>
    void trace(const std::format_string<Args...> fmt, Args&&... args)
    {
        log(spdlog::level::trace, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void debug(const std::format_string<Args...> fmt, Args&&... args)
    {
        log(spdlog::level::trace, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void info(const std::format_string<Args...> fmt, Args&&... args)
    {
        log(spdlog::level::info, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void warn(const std::format_string<Args...> fmt, Args&&... args)
    {
        log(spdlog::level::warn, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void error(const std::format_string<Args...> fmt, Args&&... args)
    {
        log(spdlog::level::err, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void critical(const std::format_string<Args...> fmt, Args&&... args)
    {
        log(spdlog::level::critical, fmt, std::forward<Args>(args)...);
    }

  private:
    std::source_location location;
    spdlog::logger* logger = spdlog::default_logger_raw();
};

export namespace spdlog
{
    Logger get(
        spdlog::logger* logger = spdlog::default_logger_raw(),
        const std::source_location location = std::source_location::current()
    )
    {
        Logger current(logger, location);
        return current;
    }

    using spdlog::critical;
    using spdlog::debug;
    using spdlog::error;
    using spdlog::info;
    using spdlog::trace;
    using spdlog::warn;

    using spdlog::set_pattern;
    using spdlog::to_hex;
} // namespace spdlog
