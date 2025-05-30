module;
#include <catch2/../catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

export module catch2;
import std;

export namespace catch2
{
    template <typename CharT>
    int run(int argc, CharT const* const argv[])
    {
        return Catch::Session().run(argc, argv);
    }

    void require(bool _expression, std::string _message,
                 std::source_location _location = std::source_location::current())
    {
        do
        {
            ;
            Catch::AssertionHandler catchAssertionHandler(
                "REQUIRE"_catch_sr,
                ::Catch::SourceLineInfo(_location.file_name(), static_cast<std::size_t>(_location.line())),
                Catch::StringRef(_message), Catch::ResultDisposition::Normal);
            try
            {
                CATCH_INTERNAL_START_WARNINGS_SUPPRESSION
                CATCH_INTERNAL_SUPPRESS_GLOBALS_WARNINGS
                CATCH_INTERNAL_SUPPRESS_UNUSED_VARIABLE_WARNINGS
                catchAssertionHandler.handleExpr(Catch::Decomposer() <= _expression);
                CATCH_INTERNAL_STOP_WARNINGS_SUPPRESSION
            }
            catch (...)
            {
                (catchAssertionHandler).handleUnexpectedInflightException();
            }
            catchAssertionHandler.complete();
        } while ((void)0, (false) && static_cast<const bool&>(!!(_expression)));
    }

    void check(bool _expression, std::string _message, std::source_location _location = std::source_location::current())
    {
        do
        {
            ;
            Catch::AssertionHandler catchAssertionHandler(
                "CHECK"_catch_sr,
                ::Catch::SourceLineInfo(_location.file_name(), static_cast<std::size_t>(_location.line())),
                Catch::StringRef(_message), Catch::ResultDisposition::ContinueOnFailure);
            try
            {
                CATCH_INTERNAL_START_WARNINGS_SUPPRESSION
                CATCH_INTERNAL_SUPPRESS_PARENTHESES_WARNINGS
                catchAssertionHandler.handleExpr(Catch::Decomposer() <= _expression);
                CATCH_INTERNAL_STOP_WARNINGS_SUPPRESSION
            }
            catch (...)
            {
                (catchAssertionHandler).handleUnexpectedInflightException();
            }
            catchAssertionHandler.complete();
        } while ((void)0, (false) && static_cast<const bool&>(!!(_expression)));
    }

    using TestCase = std::shared_ptr<Catch::AutoReg>;

    TestCase test_case(std::string _name, std::string _tags, void (*_fn)(),
                                              std::source_location _location = std::source_location::current())
    {
        auto autoRegistrar = std::make_shared<Catch::AutoReg>(
            Catch::makeTestInvoker(_fn),
            ::Catch::SourceLineInfo(_location.file_name(), static_cast<std::size_t>(_location.line())),
            Catch::StringRef(), Catch::NameAndTags{_name, _tags});
        return autoRegistrar;
    }

    void session(std::string _message, std::function<void()> _fn, std::source_location _location = std::source_location::current())
    {
        CATCH_INTERNAL_START_WARNINGS_SUPPRESSION
        CATCH_INTERNAL_SUPPRESS_UNUSED_VARIABLE_WARNINGS
        if (Catch::Section const& catch_internal_Section = Catch::Section(
                ::Catch::SourceLineInfo(_location.file_name(), static_cast<std::size_t>(_location.line())), _message))
        {
            _fn();
        }
        CATCH_INTERNAL_STOP_WARNINGS_SUPPRESSION
    }
} // namespace catch2
