module;
#include <catch2/catch_test_macros.hpp>

export module catch2;
export import catch2.session;
import std;

::Catch::SourceLineInfo lineInfo(std::source_location _location)
{
    return ::Catch::SourceLineInfo(_location.file_name(), static_cast<std::size_t>(_location.line()));
}

void test(
    bool _expression, std::string _message, Catch::StringRef _macroName,
    Catch::ResultDisposition::Flags _resultDisposition, std::source_location _location = std::source_location::current()
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
    void require(
        bool _expression, std::string _message = "", std::source_location _location = std::source_location::current()
    )
    {
        test(_expression, _message, "REQUIRE"_catch_sr, Catch::ResultDisposition::Normal, _location);
    }

    void check(
        bool _expression, std::string _message = "", std::source_location _location = std::source_location::current()
    )
    {
        test(_expression, _message, "CHECK"_catch_sr, Catch::ResultDisposition::ContinueOnFailure, _location);
    }

    void test_case(
        std::string _name, std::string _tags, void (*_fn)(),
        std::source_location _location = std::source_location::current()
    )
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
