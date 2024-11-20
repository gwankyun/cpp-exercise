#include <cassert>

#include <any>
#include <functional>

#include <boost/scope/scope_exit.hpp>
#include <catch2/../catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <spdlog/spdlog.h>

TEST_CASE("void* and any", "[ptr]")
{
    struct Object
    {
        using Fn = std::function<void()>;
        Object(Fn _fn) : fn(_fn)
        {
        }
        ~Object()
        {
            fn();
        }

      private:
        Fn fn;
    };

    struct Destroy
    {
        bool void_p = false;
        bool any = false;
    } destroy;

    void* obj_void_p = new Object(
        [&]
        {
            SPDLOG_INFO("void*");
            destroy.void_p = true;
        });

    delete static_cast<Object*>(obj_void_p);

    {
        auto obj_any = std::make_any<Object>(
            [&]
            {
                SPDLOG_INFO("any");
                destroy.any = true;
            });

        auto p = std::any_cast<Object>(&obj_any);
        if (p != nullptr)
        {
            static_assert(std::is_same_v<Object*, decltype(p)>);
        }
    }

    REQUIRE(destroy.void_p);
    REQUIRE(destroy.any);
}

typedef void (*callback)(void*);

void register_callback(void* _context, callback _func)
{
    _func(_context);
}

struct Context
{
    std::function<void()> bind;
    static void call(void* _self)
    {
        static_cast<Context*>(_self)->bind();
    }
};

TEST_CASE("register_callback", "[lambda]")
{
    int x = 0;

    // 將lambda轉成std::function<void()>包裝到類中
    Context context{[&] { x = 1; }};

    // 通過靜態類方法將void*轉成類再調用之前保存的lambda
    register_callback(&context, Context::call);

    REQUIRE(x == 1);
}

int main(int _argc, char* _argv[])
{
    std::string log_format{"[%C-%m-%d %T.%e] [%^%L%$] [%-20!!:%4#] %v"};
    spdlog::set_pattern(log_format);

    auto result = Catch::Session().run(_argc, _argv);
    return result;
}
