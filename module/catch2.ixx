module;
#include <catch2/catch_test_macros.hpp>

export module catch2;
export import catch2.session;
import std;

auto lineInfo(std::source_location _location)
{
    return ::Catch::SourceLineInfo(_location.file_name(), static_cast<std::size_t>(_location.line()));
}

namespace detail
{
    constexpr std::source_location current(std::source_location location = std::source_location::current())
    {
        return location;
    }
} // namespace detail

void test(
    bool _expression, std::string _message, Catch::StringRef _macroName,
    Catch::ResultDisposition::Flags _resultDisposition, const std::source_location _location = detail::current()
)
{
    do
    {
        Catch::AssertionHandler catchAssertionHandler(
            _macroName, lineInfo(_location), Catch::StringRef(_message), _resultDisposition
        );
        try
        {
            catchAssertionHandler.handleExpr(Catch::Decomposer() <= _expression);
        }
        catch (...)
        {
            (catchAssertionHandler).handleUnexpectedInflightException();
        }
        catchAssertionHandler.complete();
    } while ((void)0, (false) && static_cast<const bool&>(!!(_expression)));
}

export namespace Catch
{
    using detail::current;
    void require(bool _expression, std::string _message = "", const std::source_location _location = detail::current())
    {
        test(_expression, _message, "REQUIRE"_catch_sr, Catch::ResultDisposition::Normal, _location);
    }

    void check(bool _expression, std::string _message = "", const std::source_location _location = detail::current())
    {
        test(_expression, _message, "CHECK"_catch_sr, Catch::ResultDisposition::ContinueOnFailure, _location);
    }

    void test_case(std::string _name, std::string _tags, void (*_fn)(), std::source_location _location = detail::current())
    {
        CATCH_INTERNAL_START_WARNINGS_SUPPRESSION
        const Catch::AutoReg autoRegistrar(
            Catch::makeTestInvoker(_fn), lineInfo(_location), Catch::StringRef(), Catch::NameAndTags{_name, _tags}
        );
        CATCH_INTERNAL_STOP_WARNINGS_SUPPRESSION
    }

    void session(
        std::string _message, std::function<void()> _fn,
        std::source_location _location = std::source_location::current()
    )
    {
        if (Catch::Section const& catch_internal_Section = Catch::Section(lineInfo(_location), _message))
        {
            _fn();
        }
    }
} // namespace Catch
