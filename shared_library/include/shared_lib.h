﻿#pragma once

#ifndef DECLSPEC
#    ifdef _MSC_VER // __declspec是MSVC關鍵字
#        define DECLSPEC(x) __declspec(x)
#    else
#        define DECLSPEC(x)
#    endif
#endif

#ifndef DYN_LIB_API
#    ifdef DLL_EXPORT // 動態庫需要定義DLL_EXPORT這個宏
#        define DYN_LIB_API DECLSPEC(dllexport)
#    else
#        define DYN_LIB_API DECLSPEC(dllimport)
#    endif
#endif

struct Point
{
    int x;
    int y;
};

#ifdef __cplusplus // 衹在C++下有效
extern "C"         // 表示像C語言那樣導出的名字不作修飾。
{
#endif
    DYN_LIB_API long add(long _a, long _b);
    DYN_LIB_API void Point_init(Point* _point);
    DYN_LIB_API double Point_distance(Point* _point, Point* _other);
    // DYN_LIB_API void get_buffer(int a[]);
    // DYN_LIB_API void update_char_p(char* str);
    // DYN_LIB_API void update_int(int* i);
    // DYN_LIB_API void update_array(int a[]);
    // DYN_LIB_API void update_struct(Point* point);
#ifdef __cplusplus
}
#endif