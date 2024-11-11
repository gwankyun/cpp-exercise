#include <iostream>
#include <numbers> // std::numbers::pi
#include <string>
#include <vector>

#include <proxy/proxy.h> // PRO_DEF_MEM_DISPATCH pro::
#include <spdlog/spdlog.h>

// Windows下有全局Rectangle函數，所以加個命名空間
namespace type
{
    struct Circle
    {
        Circle(double _radius) : radius(_radius)
        {
        }
        void draw()
        {
            SPDLOG_INFO("Circle");
        }
        double area()
        {
            return std::pow(radius, 2.0) * std::numbers::pi;
        }
        double radius = 0.0;
    };

    struct Rectangle
    {
        Rectangle(double _width, double _height) : width(_width), height(_height)
        {
        }
        void draw()
        {
            SPDLOG_INFO("Rectangle");
        }
        double area()
        {
            return width * height;
        }
        double width = 0.0;
        double height = 0.0;
    };
} // namespace type

// 定義兩個分派成員函數draw和area
PRO_DEF_MEM_DISPATCH(MemDraw, draw); // draw成員函數
PRO_DEF_MEM_DISPATCH(MemArea, area); // area成員函數

// 定義支持以上分派成員函數的偽裝類
struct Shape                                            //
    : pro::facade_builder                               // 偽裝類構造器
      ::add_convention<MemDraw, void()>                 // draw的簽名是void draw()
      ::add_convention<MemArea, double()>               // area的簽名是double area()
      ::support_copy<pro::constraint_level::nontrivial> // 支持複製
      ::build
{
};

int main(int _argc, char* _argv[])
{
    std::string log_format{"[%C-%m-%d %T.%e] [%^%L%$] [%-10!!:%4#] %v"};
    spdlog::set_pattern(log_format);

    std::vector<pro::proxy<Shape>> shape_container = {pro::make_proxy<Shape, type::Circle>(1.0),
                                                      pro::make_proxy<Shape, type::Rectangle>(2.0, 3.0)};

    for (auto& i : shape_container)
    {
        i->draw();
        SPDLOG_INFO("area: {}", i->area());
    }

    return 0;
}
