#include <string>

#include <spdlog/spdlog.h>

struct TreeNode
{
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int _x, TreeNode* _left = nullptr, TreeNode* _right = nullptr) : val(_x), left(_left), right(_right)
    {
    }
    ~TreeNode()
    {
        if (left != nullptr)
        {
            delete left;
            left = nullptr;
        }
        if (right != nullptr)
        {
            delete right;
            right = nullptr;
        }
    }
};

void init_tree()
{
    auto tree = new TreeNode(1, new TreeNode(2, new TreeNode(4), new TreeNode(5)), new TreeNode(3));

    // 插入新節點
    auto p = new TreeNode(0);
    auto n1 = tree;
    auto n2 = n1->left;
    n1->left = p;
    p->left = n2;

    // 刪除節點
    n1->left = n2;
    delete p;

    delete tree;
}

int main(int _argc, char* _argv[])
{
    std::string log_format{"[%C-%m-%d %T.%e] [%^%L%$] [%-20!!:%4#] %v"};
    spdlog::set_pattern(log_format);

    // SPDLOG_INFO("__cplusplus: {}", __cplusplus);

    // auto result = Catch::Session().run(_argc, _argv);
    // return result;
    return 0;
}