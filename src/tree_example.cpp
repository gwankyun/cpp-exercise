#include <cassert>
#include <queue>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

#include <catch2/../catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <spdlog/spdlog.h>

namespace global
{
    std::unordered_set<ptrdiff_t> ptr_set;
}

template <typename T>
T* safe_new(T* _ptr)
{
    global::ptr_set.insert(reinterpret_cast<ptrdiff_t>(_ptr));
    return _ptr;
}

template <typename T>
void safe_delete(T*& _ptr)
{
    auto& g_ptr_set = global::ptr_set;
    if (_ptr != nullptr)
    {
        assert(g_ptr_set.find(reinterpret_cast<ptrdiff_t>(_ptr)) != g_ptr_set.end());
        g_ptr_set.erase(reinterpret_cast<ptrdiff_t>(_ptr));
        delete _ptr;
        _ptr = nullptr;
    }
}

namespace tree
{
    template <typename T>
    struct Node
    {
        T value;
        Node<T>* left = nullptr;
        Node<T>* right = nullptr;
        Node(T _x, Node<T>* _left = nullptr, Node<T>* _right = nullptr) : value(_x), left(_left), right(_right)
        {
        }
        ~Node()
        {
            safe_delete(left);
            safe_delete(right);
        }
    };

    template <typename T>
    bool is_leaf(const Node<T>* _node)
    {
        assert(_node != nullptr);
        return _node->left == nullptr && _node->right == nullptr;
    }

    template <typename T>
    int degree(const Node<T>* _node)
    {
        assert(_node != nullptr);
        return int(_node->left != nullptr) + int(_node->right != nullptr);
    }

    template <typename T>
    int size(const Node<T>* _node)
    {
        std::queue<const Node<T>*> q;
        q.push(_node);
        auto count = 0u;
        while (!q.empty())
        {
            auto node = q.front();
            q.pop();
            count++;
            if (node->left != nullptr)
            {
                q.push(node->left);
            }
            if (node->right != nullptr)
            {
                q.push(node->right);
            }
        }
        return count;
    }

    enum struct Side
    {
        Left,
        Right
    };

    enum struct TraversalMethod
    {
        Iterative,
        Recursive
    };

    template <typename T>
    Node<T>* left(const Node<T>* _node)
    {
        if (_node == nullptr)
        {
            return nullptr;
        }
        return _node->left;
    }

    template <typename T>
    Node<T>* right(const Node<T>* _node)
    {
        if (_node == nullptr)
        {
            return nullptr;
        }
        return _node->right;
    }

    template <typename T>
    void insert(Node<T>* _node, Side _side, Node<T>* _new_node)
    {
        assert(_node != nullptr);
        assert(_new_node != nullptr);

        if (_side == Side::Left)
        {
            _new_node->left = _node->left;
            _node->left = _new_node;
        }
        else if (_side == Side::Right)
        {
            _new_node->right = _node->right;
            _node->right = _new_node;
        }
    }

    template <typename T>
    Node<T>* remove(Node<T>* _node, Side _side)
    {
        assert(_node != nullptr);

        Node<T>* temp = nullptr;
        if (_side == Side::Left)
        {
            temp = _node->left;
            _node->left = left(temp);
        }
        else if (_side == Side::Right)
        {
            temp = _node->right;
            _node->right = right(temp);
        }

        if (temp != nullptr)
        {
            temp->left = nullptr;
            temp->right = nullptr;
        }

        return temp;
    }

    template <typename T>
    void levelOrder(Node<T>* _node, std::vector<T>& _result)
    {
        std::queue<Node<T>*> q;
        q.push(_node);
        while (!q.empty())
        {
            auto node = q.front();
            q.pop();
            _result.push_back(node->value);
            if (node->left != nullptr)
            {
                q.push(node->left);
            }
            if (node->right != nullptr)
            {
                q.push(node->right);
            }
        }
    }

    template <typename T>
    void preOrder(Node<T>* _node, TraversalMethod _method, std::vector<T>& _result)
    {
        if (_method == TraversalMethod::Recursive)
        {
            if (_node == nullptr)
            {
                return;
            }
            _result.push_back(_node->value);
            preOrder(_node->left, TraversalMethod::Recursive, _result);
            preOrder(_node->right, TraversalMethod::Recursive, _result);
        }
        else if (_method == TraversalMethod::Iterative)
        {
            std::stack<Node<T>*> s;
            s.push(_node);
            while (!s.empty())
            {
                auto node = s.top();
                s.pop();
                _result.push_back(node->value);
                if (node->right != nullptr)
                {
                    s.push(node->right);
                }
                if (node->left != nullptr)
                {
                    s.push(node->left);
                }
            }
        }
    }

    template <typename T>
    void inOrder(Node<T>* _node, TraversalMethod _method, std::vector<T>& _result)
    {
        if (_method == TraversalMethod::Recursive)
        {
            if (_node == nullptr)
            {
                return;
            }
            inOrder(_node->left, TraversalMethod::Recursive, _result);
            _result.push_back(_node->value);
            inOrder(_node->right, TraversalMethod::Recursive, _result);
        }
        else if (_method == TraversalMethod::Iterative)
        {
            std::stack<Node<T>*> s;
            auto node = _node;
            while (node != nullptr || !s.empty())
            {
                while (node != nullptr)
                {
                    s.push(node);
                    node = node->left;
                }
                node = s.top();
                s.pop();
                _result.push_back(node->value);
                node = node->right;
            }
        }
    }

    template <typename T>
    void postOrder(Node<T>* _node, TraversalMethod _method, std::vector<T>& _result)
    {
        if (_method == TraversalMethod::Recursive)
        {
            if (_node == nullptr)
            {
                return;
            }
            postOrder(_node->left, TraversalMethod::Recursive, _result);
            postOrder(_node->right, TraversalMethod::Recursive, _result);
            _result.push_back(_node->value);
        }
        else if (_method == TraversalMethod::Iterative)
        {
            std::stack<Node<T>*> s;
            std::stack<Node<T>*> output;
            s.push(_node);

            auto push = [&s](Node<T>* _node)
            {
                if (_node != nullptr)
                {
                    s.push(_node);
                }
            };

            while (!s.empty())
            {
                auto node = s.top();
                s.pop();
                output.push(node);
                push(node->left);
                push(node->right);
            }
            while (!output.empty())
            {
                auto node = output.top();
                output.pop();
                _result.push_back(node->value);
            }
        }
    }
} // namespace tree

TEST_CASE("insert remove", "[tree]")
{
    std::vector<tree::Node<int>*> node(10, nullptr);

    for (auto i = 1; i < 6; i++)
    {
        node[i] = safe_new(new tree::Node(i));
    }

    node[1]->left = node[2];
    node[1]->right = node[3];
    node[2]->left = node[4];
    node[2]->right = node[5];

    auto tree = node[1];
    REQUIRE(tree::size(tree) == 5);

    // 插入新節點
    tree::insert(node[1], tree::Side::Left, safe_new(new tree::Node(0)));
    REQUIRE(tree::size(tree) == 6);

    // 刪除節點
    auto p = tree::remove(node[1], tree::Side::Left);
    safe_delete(p);
    REQUIRE(tree::size(tree) == 5);

    safe_delete(tree);
    REQUIRE(global::ptr_set.empty());
}

TEST_CASE("order", "[tree]")
{
    std::vector<tree::Node<int>*> node(10, nullptr);

    for (auto i = 1; i < 8; i++)
    {
        node[i] = safe_new(new tree::Node(i));
    }

    node[1]->left = node[2];
    node[1]->right = node[3];

    node[2]->left = node[4];
    node[2]->right = node[5];

    node[3]->left = node[6];
    node[3]->right = node[7];

    auto tree = node[1];

    std::vector<int> result;
    tree::levelOrder(tree, result);
    REQUIRE(result == std::vector<int>{1, 2, 3, 4, 5, 6, 7});

    result.clear();
    tree::preOrder(tree, tree::TraversalMethod::Recursive, result);
    REQUIRE(result == std::vector<int>{1, 2, 4, 5, 3, 6, 7});

    result.clear();
    tree::preOrder(tree, tree::TraversalMethod::Iterative, result);
    REQUIRE(result == std::vector<int>{1, 2, 4, 5, 3, 6, 7});

    result.clear();
    tree::inOrder(tree, tree::TraversalMethod::Recursive, result);
    REQUIRE(result == std::vector<int>{4, 2, 5, 1, 6, 3, 7});

    result.clear();
    tree::inOrder(tree, tree::TraversalMethod::Iterative, result);
    REQUIRE(result == std::vector<int>{4, 2, 5, 1, 6, 3, 7});

    result.clear();
    tree::postOrder(tree, tree::TraversalMethod::Recursive, result);
    REQUIRE(result == std::vector<int>{4, 5, 2, 6, 7, 3, 1});

    result.clear();
    tree::postOrder(tree, tree::TraversalMethod::Iterative, result);
    REQUIRE(result == std::vector<int>{4, 5, 2, 6, 7, 3, 1});

    safe_delete(tree);
    REQUIRE(global::ptr_set.empty());
}

int main(int _argc, char* _argv[])
{
    std::string log_format{"[%C-%m-%d %T.%e] [%^%L%$] [%-20!!:%4#] %v"};
    spdlog::set_pattern(log_format);

    SPDLOG_INFO("__cplusplus: {}", __cplusplus);

    auto result = Catch::Session().run(_argc, _argv);
    return result;
}