#include <hello_imgui/hello_imgui.h>

int main()
{
    HelloImGui::Run(
        []()
        {
            ImGui::Text("Hello, world!");
            ImGui::ShowDemoWindow();
        });
}
