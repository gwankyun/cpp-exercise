#pragma once

#ifndef DYN_LIB_DECLSPEC
#  ifdef _MSC_VER // __declspec是MSVC關鍵字
#    define DYN_LIB_DECLSPEC(x) __declspec(x)
#  else
#    define DYN_LIB_DECLSPEC(x)
#  endif
#endif

#ifndef DYN_LIB_API
#  ifdef DYN_LIB_EXPORT // 動態庫需要定義DYN_LIB_EXPORT這個宏
#    define DYN_LIB_API DYN_LIB_DECLSPEC(dllexport)
#  else
#    define DYN_LIB_API DYN_LIB_DECLSPEC(dllimport)
#  endif
#endif

struct Point
{
    int x;
    int y;
};

struct Persion
{
    char* name;
    int age;
};

struct List
{
    void* data;
};

#ifdef __cplusplus // 衹在C++下有效
extern "C"         // 表示像C語言那樣導出的名字不作修飾。
{
#endif
    DYN_LIB_API long add(long _a, long _b);
    DYN_LIB_API void Point_init(struct Point* _point);
    DYN_LIB_API double Point_distance(struct Point* _point, struct Point* _other);
    // DYN_LIB_API void get_buffer(int a[]);
    DYN_LIB_API void set_char_p(char** _str, const char* _value);
    DYN_LIB_API void set_int(int* _v, int _n);
    DYN_LIB_API void set_array(int _a[], size_t _index, int _value);
    DYN_LIB_API void set_struct(struct Point* _point, int _x, int _y);
#ifdef __cplusplus
}
#endif
