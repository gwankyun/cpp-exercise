// src/shared_lib.cpp
#include <cassert>
#include <cstring>
#include <iostream>
#include <numbers>
#include <algorithm>
#include <list>
// #include <log.hpp>
// #include <shared_lib.h>
#include "../include/shared_lib.h"

/// @brief 相加
/// @param a 被加數
/// @param b 被加數
/// @return a和b的和
long add(long _a, long _b)
{
    return _a + _b;
}

void Point_init(Point* _point)
{
    auto& point = *_point;
    point.x = 0;
    point.y = 0;
}

double Point_distance(Point* _point, Point* _other)
{
    auto px = std::pow<double>(_point->x - _other->x, 2.0);
    auto py = std::pow<double>(_point->y - _other->y, 2.0);
    return std::sqrt(px + py);
}

void set_int(int* _v, int _n)
{
    *_v = _n;
}

void set_char_p(char** _str, const char* _value)
{
    *_str = const_cast<char*>(_value);
}

void set_struct(Point* _point, int _x, int _y)
{
    _point->x = _x;
    _point->y = _y;
}

void set_array(int _a[], size_t _index, int _value)
{
    _a[_index] = _value;
}

void List_init(List* _list)
{
    _list->data = new std::list<std::string>;
}

void List_deinit(List* _list)
{
    delete static_cast<List*>(_list->data);
}

void List_push_back(List* _list, const char* _value)
{
    static_cast<std::list<std::string>*>(_list->data)->push_back(_value);
}

void List_pop_back(List* _list)
{
    static_cast<std::list<std::string>*>(_list->data)->pop_back();
}

// void get_buffer(int a[])
// {
//     a[0] = 1;
//     a[1] = 2;
//     a[2] = 3;
// }

// void update_char_p(char *str)
// {
//     DBG(str);
//     strncpy(str, "abc", 3);
// }

// void update_int(int *i)
// {
//     DBG(*i);
//     *i = 21;
// }

// void update_array(int a[])
// {
//     DBG(a[4]);
//     a[4] = 5;
// }

// void update_struct(Point *point)
// {
//     assert(point->x == 1);
//     assert(point->y == 2);
//     point->x = 10;
//     point->y = 20;
// }
