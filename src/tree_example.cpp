#include <string>
#include <vector>
#include <cassert>

#include <spdlog/spdlog.h>


namespace tree
{
    struct Node
    {
        int val;
        Node* left;
        Node* right;
        Node(int _x, Node* _left = nullptr, Node* _right = nullptr) : val(_x), left(_left), right(_right)
        {
        }
        ~Node()
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

    bool is_leaf(const Node* _node)
    {
        assert(_node != nullptr);
        return _node->left == nullptr && _node->right == nullptr;
    }

    int degree(const Node* _node)
    {
        assert(_node!= nullptr);
        return int(_node->left != nullptr) + int(_node->right != nullptr);
    }

    enum struct Side
    {
        Left,
        Right
    };

    Node* left(Node* _node)
    {
        if (_node == nullptr)
        {
            return nullptr;
        }
        return _node->left;
    }

    Node* right(Node* _node)
    {
        if (_node == nullptr)
        {
            return nullptr;
        }
        return _node->right;
    }

    void insert(Node* _node, Side _side, Node* _new_node)
    {
        assert(_node != nullptr);
        assert(_new_node != nullptr);
        auto& node = *_node;
        auto& new_node = *_new_node;

        if (_side == Side::Left)
        {
            new_node.left = node.left;
            node.left = &new_node;
        }
        else if (_side == Side::Right)
        {
            new_node.right = node.right;
            node.right = &new_node;
        }
    }

    Node* remove(Node* _node, Side _side)
    {
        assert(_node != nullptr);
        auto& node = *_node;

        if (_side == Side::Left)
        {
            auto tmp = node.left;
            node.left = left(tmp);
            return tmp;
        }
        else if (_side == Side::Right)
        {
            auto tmp = node.right;
            node.right = right(tmp);
            return tmp;
        }
    }
} // namespace tree

void init_tree()
{
    std::vector<tree::Node*> node(10, nullptr);

    for (auto i = 1; i < 6; i++)
    {
        node[i] = new tree::Node(i);
    }

    node[1]->left = node[2];
    node[1]->right = node[3];
    node[2]->left = node[4];
    node[2]->right = node[5];

    auto tree = node[1];

    // 插入新節點
    tree::insert(node[1], tree::Side::Left, new tree::Node(0));

    // 刪除節點
    auto p = tree::remove(node[1], tree::Side::Left);
    if (p != nullptr)
    {
        delete p;
        p = nullptr;
    }

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