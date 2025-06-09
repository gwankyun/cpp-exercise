module;
#include "main_macro.h"
#include <module/boost/scope/macro.h>
#include <module/catch2/macro.h>

module main;
import std;
import spdlog;
import catch2;
import boost.scope;

#ifndef ON
#  define ON(_exp) (CATCH_EXPRESSION(_exp))
#endif

namespace c = Catch;

template <typename T>
inline void delete_ptr(T*& _ptr)
{
    if (_ptr != nullptr)
    {
        delete _ptr;
        _ptr = nullptr;
    }
}

typedef void (*callback)(void*);

void apply_callback(callback _func, void* _context)
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

namespace zero_size_array
{
    struct A
    {
        std::uint8_t data[8];
    };

    PRAGMA_WARNING_PUSH();
    PRAGMA_WARNING_DISABLE(4200);
    struct B
    {
        std::uint8_t data[8];
        ZERO_SIZE_ARRAY(std::byte, a);
    };
    PRAGMA_WARNING_POP();

    std::byte* B_a(B* _self)
    {
#if ZERO_SIZE_ARRAY_SUPPORTED
        return _self->a;
#else
        return (std::byte*)_self + sizeof(B);
#endif
    }
} // namespace zero_size_array

namespace test
{
    void void_and_any()
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
            bool unique = false;
        } destroy;

        void* obj_void_p = new Object(
            [&]
            {
                spdlog::get().info("void*");
                destroy.void_p = true;
            }
        );

        delete static_cast<Object*>(obj_void_p);

        {
            auto obj_any = std::make_any<Object>(
                [&]
                {
                    spdlog::get().info("any");
                    destroy.any = true;
                }
            );

            auto p = std::any_cast<Object>(&obj_any);
            if (p != nullptr)
            {
                static_assert(std::is_same_v<Object*, decltype(p)>);
            }
        }

        {
            auto unique = [&](int* _ptr)
            {
                delete _ptr;
                spdlog::get().info("unique_ptr");
                destroy.unique = true;
            };
            std::unique_ptr<int, decltype(unique)> ptr(new int(0), unique);
        }

        c::require ON(destroy.void_p);
        c::require ON(destroy.any);
        c::require ON(destroy.unique);
    }

    void apply_callback()
    {
        int x = 0;

        // 將lambda轉成std::function<void()>包裝到類中
        Context context{[&] { x = 1; }};

        // 通過靜態類方法將void*轉成類再調用之前保存的lambda
        apply_callback(Context::call, &context);

        c::require ON(x == 1);
    }

    void defer_check()
    {
        int n = 1;
        {
            BOOST_SCOPE_DEFER[&]
            {
                n++;
                spdlog::get().info("");
            };
            c::require ON(n == 1);
        }
        c::require ON(n == 2);
    }

    void memset_check()
    {
        std::vector<std::uint8_t> v8(8, 1);
        // spdlog_module::get().info("v: {}", spdlog::to_hex(v8));

        std::vector<std::uint32_t> v32(8, 1);
        auto v32a = v32;
        std::memset(
            v32.data(),                        // 目標
            0,                                 // 值
            v32.size() * sizeof(std::uint32_t) // 以字節為單位的長度
        );
        std::fill_n(v32a.begin(), v32a.size(), 0);
        // spdlog_module::get().info("v32: {}", spdlog::to_hex(v32.begin(), v32.end()));
        c::require ON(v32 == v32a);
    }

    void memcpy_check()
    {
        std::vector<std::uint8_t> v8(64, 0);
        auto v8a = v8;
        // SPDLOG_INFO("v: {}", spdlog::to_hex(v8));
        // spdlog_module::get().info("v: {}", spdlog_module::to_hex(v8));
        std::vector<std::uint32_t> v32(4, 1);

        std::memcpy(
            v8.data(),                         // 目標
            v32.data(),                        // 源
            v32.size() * sizeof(std::uint32_t) // 以字節為單位的長度
        );
        std::copy_n(v32.data(), v32.size(), (std::uint32_t*)v8a.data());
        // SPDLOG_INFO("v: {}", spdlog::to_hex(v8));
        c::require ON(v8 == v8a);
    }

    // 柔性數組
    void zero_size_array()
    {
        namespace z = zero_size_array;

        // 柔性數組本身不佔用空間，但是如果它是結構的最後一個成員，
        // 那麼結構的大小就會包含柔性數組的大小。
        // 因此，sizeof(A)和sizeof(B)的大小是相同的。
        c::require ON(sizeof(z::A) == sizeof(z::B));

        // 柔性數組可以用於動態分配內存，
        // 因為它的大小是在運行時決定的。
        auto size = 8u;
        auto d = (z::B*)std::malloc(sizeof(z::B) + size);
        BOOST_SCOPE_DEFER[&]
        {
            std::free(d);
        };
        std::fill_n(z::B_a(d), size, std::byte{0x01});
    }

    void set()
    {
        namespace ranges = std::ranges;
        using SetType = std::set<int>;
        SetType v1{1, 2, 3, 4};
        SetType v2{3, 7, 2, 1};

        SetType v1_v2;
        // v1 - v2
        ranges::set_difference(v1, v2, std::inserter(v1_v2, v1_v2.begin()));
        c::require ON(v1_v2 == SetType{4});

        SetType v2_v1;
        // v2 - v1
        ranges::set_difference(v2, v1, std::inserter(v2_v1, v2_v1.begin()));
        c::require ON(v2_v1 == SetType{7});
    }
} // namespace test

namespace detail
{
    using fn = std::function<void()>;
    // fn move_construct = [] {
    //     SPDLOG_INFO("move Object: {}", _other.id);
    // };
    fn move_construct;
    fn move_assign;
    fn copy_construct;
    fn copy_assign;
    fn destruct;
    fn construct;
    int count = 0;
} // namespace detail

struct Object
{
    /// @brief 構造函數
    Object() = default;
    /// @brief 析構函數
    ~Object() noexcept = default;
    /// @brief 複製構造函數
    Object(const Object& _other) : value(_other.value)
    {
        spdlog::get().info("copy Object");
        detail::copy_construct();
    }
    /// @brief 複製賦值
    Object& operator=(const Object& _other)
    {
        spdlog::get().info("assign Object");
        value = _other.value;
        detail::copy_assign();
        return *this;
    }
    /// @brief 移動構造函數
    Object(Object&& _other) noexcept : value(_other.value)
    {
        spdlog::get().info("move Object");
        detail::move_construct();
    }
    /// @brief 移動賦值
    Object& operator=(Object&& _other) noexcept
    {
        spdlog::get().info("move assign Object");
        value = _other.value;
        detail::move_assign();
        return *this;
    }
    int value = 0;
};

Object fn()
{
    Object obj;
    obj.value = 1;
    return obj;
}

void log(const std::string_view _message, const std::source_location location = std::source_location::current())
{
    std::print("[{}:{}] {}\n", location.function_name(), location.line(), _message);
}

int main(int _argc, char* _argv[])
{
    std::string log_format{"[%C-%m-%d %T.%e] [%^%L%$] [%-20!!:%4#] %v"};
    spdlog::set_pattern(log_format);

    spdlog::get().info("__cplusplus: {}", __cplusplus);

    log(std::format(""));

    spdlog::get().warn("{}", "hello");
    spdlog::get().info("{} + {} = {}", 1, 2, 3);

    spdlog::info("info");

    using Catch::test_case;
    test_case("void* and any", "[ptr]", test::void_and_any);
    test_case("apply_callback", "[lambda]", test::apply_callback);
    test_case("memset", "[mem]", test::memset_check);
    test_case("memcpy", "[mem]", test::memcpy_check);
    test_case("zero size array", "[struct]", test::zero_size_array);
    test_case("set", "[algorithm]", test::set);
    test_case("defer", "[boost.scope]", test::defer_check);

    auto result = Catch::Session().run(_argc, _argv);
    return result;
}
