module;
#include <catch2/../catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>

export module catch2;
import std;

export namespace catch2_module
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
                __pragma(warning(push)) catchAssertionHandler.handleExpr(Catch::Decomposer() <= _expression);
                __pragma(warning(pop))
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
                __pragma(warning(push)) catchAssertionHandler.handleExpr(Catch::Decomposer() <= _expression);
                __pragma(warning(pop))
            }
            catch (...)
            {
                (catchAssertionHandler).handleUnexpectedInflightException();
            }
            catchAssertionHandler.complete();
        } while ((void)0, (false) && static_cast<const bool&>(!!(_expression)));
    }

    std::unique_ptr<Catch::AutoReg> regist(void (*_fn)(), std::string _name, std::string _tags,
                                           std::source_location _location = std::source_location::current())
    {
        auto autoRegistrar = std::make_unique<Catch::AutoReg>(
            Catch::makeTestInvoker(_fn),
            ::Catch::SourceLineInfo(_location.file_name(), static_cast<std::size_t>(_location.line())),
            Catch::StringRef(), Catch::NameAndTags{_name, _tags});
        return autoRegistrar;
    }
} // namespace catch2_module
