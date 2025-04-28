#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include <algorithm>
#include <any>
#include <functional>
#include <memory>

#include <boost/scope/defer.hpp>
#include <boost/scope/scope_exit.hpp>
#include <catch2/../catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <spdlog/fmt/bin_to_hex.h>
#include <spdlog/spdlog.h>

#ifndef PRAGMA_SUPPORTED
#  define PRAGMA_SUPPORTED 1
#endif

#ifndef PRAGMA
#  if PRAGMA_SUPPORTED
#    define PRAGMA(x) _Pragma(#x)
#  else
#    define PRAGMA(x) __pragma(x)
#  endif
#endif

#ifndef PRAGMA_WARNING_PUSH
#  define PRAGMA_WARNING_PUSH() PRAGMA(warning(push))
#endif

#ifndef PRAGMA_WARNING_POP
#  define PRAGMA_WARNING_POP() PRAGMA(warning(pop))
#endif

#ifndef PRAGMA_WARNING_DISABLE
#  ifdef _MSC_VER
#    define PRAGMA_WARNING_DISABLE(x) PRAGMA(warning(disable : x))
#  else
#    define PRAGMA_WARNING_DISABLE(x)
#  endif
#endif

#ifndef ZERO_SIZE_ARRAY_SUPPORTED
#  define ZERO_SIZE_ARRAY_SUPPORTED 1
#endif

#ifndef ZERO_SIZE_ARRAY
#  if ZERO_SIZE_ARRAY_SUPPORTED
#    define ZERO_SIZE_ARRAY(type, name) type name[0]
#  else
#    define ZERO_SIZE_ARRAY(type, name)
#  endif
#endif

template <typename T>
inline void delete_ptr(T*& _ptr)
{
    if (_ptr != nullptr)
    {
        delete _ptr;
        _ptr = nullptr;
    }
}

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
        bool unique = false;
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

    {
        auto unique = [&](int* _ptr)
        {
            delete _ptr;
            SPDLOG_INFO("unique_ptr");
            destroy.unique = true;
        };
        std::unique_ptr<int, decltype(unique)> ptr(new int(0), unique);
    }

    REQUIRE(destroy.void_p);
    REQUIRE(destroy.any);
    REQUIRE(destroy.unique);
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

TEST_CASE("apply_callback", "[lambda]")
{
    int x = 0;

    // 將lambda轉成std::function<void()>包裝到類中
    Context context{[&] { x = 1; }};

    // 通過靜態類方法將void*轉成類再調用之前保存的lambda
    apply_callback(Context::call, &context);

    REQUIRE(x == 1);
}

TEST_CASE("memset", "[mem]")
{
    std::vector<std::uint8_t> v8(8, 1);
    SPDLOG_INFO("v: {}", spdlog::to_hex(v8));

    std::vector<std::uint32_t> v32(8, 1);
    auto v32a = v32;
    memset(v32.data(),                        // 目標
           0,                                 // 值
           v32.size() * sizeof(std::uint32_t) // 以字節為單位的長度
    );
    std::fill_n(v32a.begin(), v32a.size(), 0);
    SPDLOG_INFO("v32: {}", spdlog::to_hex(v32.begin(), v32.end()));
    REQUIRE(v32 == v32a);
}

TEST_CASE("memcpy", "[mem]")
{
    std::vector<std::uint8_t> v8(64, 0);
    auto v8a = v8;
    SPDLOG_INFO("v: {}", spdlog::to_hex(v8));
    std::vector<std::uint32_t> v32(4, 1);

    memcpy(v8.data(),                         // 目標
           v32.data(),                        // 源
           v32.size() * sizeof(std::uint32_t) // 以字節為單位的長度
    );
    std::copy_n(v32.data(), v32.size(), (std::uint32_t*)v8a.data());
    SPDLOG_INFO("v: {}", spdlog::to_hex(v8));
    REQUIRE(v8 == v8a);
}

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

// 柔性數組
TEST_CASE("zero size array", "[struct]")
{
    namespace z = zero_size_array;

    // 柔性數組本身不佔用空間，但是如果它是結構的最後一個成員，
    // 那麼結構的大小就會包含柔性數組的大小。
    // 因此，sizeof(A)和sizeof(B)的大小是相同的。
    REQUIRE(sizeof(z::A) == sizeof(z::B));

    // 柔性數組可以用於動態分配內存，
    // 因為它的大小是在運行時決定的。
    auto size = 8u;
    auto d = (z::B*)malloc(sizeof(z::B) + size);
    BOOST_SCOPE_DEFER[&]
    {
        free(d);
    };
    std::fill_n(z::B_a(d), size, std::byte{0x01});
}

namespace detail
{
    using fn = std::function<void()>;
    // fn move_construct = [] {
    //     SPDLOG_INFO("move Object: {}", _other.id);
    // };
    fn move_assign;
    fn copy_construct;
    fn copy_assign;
    fn destruct;
    fn construct;
}

struct Object
{
    Object()
    {
        SPDLOG_INFO("create Object");
    }
    ~Object() noexcept
    {
        SPDLOG_INFO("destroy Object: {}", id);
    }
    Object(const Object& _other)
    {
        SPDLOG_INFO("copy Object: {}", _other.id);
    }
    Object& operator=(const Object& _other)
    {
        SPDLOG_INFO("assign Object: {}", _other.id);
    }
    Object(Object&& _other) noexcept
    {
        SPDLOG_INFO("move Object: {}", _other.id);
    }
    Object& operator=(Object&&) noexcept
    {
        SPDLOG_INFO("move assign Object");
    }
    int id = 0;
};

TEST_CASE("Object", "[class]")
{
    Object obj;
    obj.id = 1;
    Object obj2 = obj;
    obj2.id = 2;
    Object obj3(std::move(obj2));
}

int main(int _argc, char* _argv[])
{
    std::string log_format{"[%C-%m-%d %T.%e] [%^%L%$] [%-20!!:%4#] %v"};
    spdlog::set_pattern(log_format);

    SPDLOG_INFO("__cplusplus: {}", __cplusplus);

    auto result = Catch::Session().run(_argc, _argv);
    return result;
}
