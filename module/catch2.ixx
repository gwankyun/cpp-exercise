module;
#include <catch2/catch_test_macros.hpp>
#include <catch2/../catch2/catch_session.hpp>
//#include <cat>

export module catch2;
import std;

void test(
    bool _expression, std::string _message, Catch::StringRef macroName,
    Catch::ResultDisposition::Flags resultDisposition, std::source_location _location = std::source_location::current()
)
{
    do
    {
        Catch::AssertionHandler catchAssertionHandler(
            macroName, ::Catch::SourceLineInfo(_location.file_name(), static_cast<std::size_t>(_location.line())),
            Catch::StringRef(_message), resultDisposition
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

export namespace catch2
{
    template <typename CharT>
    int run(int argc, CharT const* const argv[])
    {
        return Catch::Session().run(argc, argv);
    }

    using Catch::Section;

    void require(
        bool _expression, std::string _message, std::source_location _location = std::source_location::current()
    )
    {
        test(_expression, _message, "REQUIRE"_catch_sr, Catch::ResultDisposition::Normal, _location);
    }

    void check(bool _expression, std::string _message, std::source_location _location = std::source_location::current())
    {
        test(_expression, _message, "CHECK"_catch_sr, Catch::ResultDisposition::ContinueOnFailure, _location);
    }

    using TestCase = std::shared_ptr<Catch::AutoReg>;

    void test_case(
        std::string _name, std::string _tags, void (*_fn)(),
        std::source_location _location = std::source_location::current()
    )
    {
        CATCH_INTERNAL_START_WARNINGS_SUPPRESSION
        Catch::AutoReg autoRegistrar(
            Catch::makeTestInvoker(_fn),
            ::Catch::SourceLineInfo(_location.file_name(), static_cast<std::size_t>(_location.line())),
            Catch::StringRef(), Catch::NameAndTags{_name, _tags}
        );
        CATCH_INTERNAL_STOP_WARNINGS_SUPPRESSION
    }

    void session(
        std::string _message, std::function<void()> _fn,
        std::source_location _location = std::source_location::current()
    )
    {
        if (Catch::Section const& catch_internal_Section = Catch::Section(
                ::Catch::SourceLineInfo(_location.file_name(), static_cast<std::size_t>(_location.line())), _message
            ))
        {
            _fn();
        }
    }
} // namespace catch2
