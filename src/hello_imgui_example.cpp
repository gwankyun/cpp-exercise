#include <string>

#include <hello_imgui/hello_imgui.h>

int main()
{
    std::string txt = "Hello, world!";
    HelloImGui::Run(
        [&]
        {
            ImGui::Text(txt.c_str());
            // ImGui::ShowDemoWindow();
            if (ImGui::Button("click"))
            {
                txt = "click";
            }
        });
}
