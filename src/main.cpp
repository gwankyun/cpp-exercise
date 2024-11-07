#include <iostream>
#include <numbers> // std::numbers::pi
#include <string>
#include <vector>

#include <proxy/proxy.h> // PRO_DEF_MEM_DISPATCH pro::
#include <spdlog/spdlog.h>

// 定義兩個分派類型MemDraw和MemArea
PRO_DEF_MEM_DISPATCH(MemDraw, draw); // 有draw成員函數
PRO_DEF_MEM_DISPATCH(MemArea, area); // 有area成員函數

// 定義支持以上分派類型的偽裝類
struct Drawable : pro::facade_builder                 // 偽裝類構造器
                  ::add_convention<MemDraw, void()>   // draw的簽名是void draw()
                  ::add_convention<MemArea, double()> // area的簽名是double area()
                  ::build
{
};

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
            return radius * radius * std::numbers::pi;
        }
        double radius = 0.0;
    };

    struct Rectangle
    {
        Rectangle(double _width, double _length) : width(_width), length(_length)
        {
        }
        void draw()
        {
            SPDLOG_INFO("Rectangle");
        }
        double area()
        {
            return width * length;
        }
        double width = 0.0;
        double length = 0.0;
    };
} // namespace type

int main(int _argc, char* _argv[])
{
    std::string log_format{"[%C-%m-%d %T.%e] [%^%L%$] [%-10!!:%4#] %v"};
    spdlog::set_pattern(log_format);

    std::vector<pro::proxy<Drawable>> shape_container;

    shape_container.emplace_back(pro::make_proxy<Drawable, type::Circle>(1.0));
    shape_container.emplace_back(pro::make_proxy<Drawable, type::Rectangle>(2.0, 3.0));

    for (auto& i : shape_container)
    {
        i->draw();
        SPDLOG_INFO("area: {}", i->area());
    }

    return 0;
}
