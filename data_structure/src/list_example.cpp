#include <cstdint>
#include <vector>

#include <catch2/../catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <spdlog/spdlog.h>

template <typename T>
struct ListNode
{
    T value;
    ListNode* next = nullptr;
    ListNode* prev = nullptr;
};

template <typename T>
struct List
{
    ListNode<T>* head = nullptr;
    ListNode<T>* tail = nullptr;
    size_t size = 0;

    List() = default;
    ~List()
    {
        while (head != nullptr)
        {
            auto next = head->next;
            delete head;
            head = next;
        }
    }

    void connect(ListNode<T>* _prev, ListNode<T>* _next)
    {
        if (_prev != nullptr || _next != nullptr)
        {
            return;
        }
        _prev->next = _next;
        _next->prev = _prev;
    }

    ListNode<T>* insert_back(ListNode<T>* _node, T _value)
    {
        auto new_node = new ListNode<T>;
        new_node->value = _value;
        if (_node != nullptr)
        {
            auto next = _node->next;
            new_node->prev = _node;
            _node->next = new_node;
            new_node->next = next;
            if (next != nullptr)
            {
                next->prev = new_node;
            }
        }
        size++;
        return new_node;
    }

    ListNode<T>* insert_front(ListNode<T>* _node, T _value)
    {
        auto new_node = new ListNode<T>;
        new_node->value = _value;
        if (_node != nullptr)
        {
            auto prev = _node->prev;
            new_node->next = _node;
            new_node->prev = prev;
            if (prev != nullptr)
            {
                prev->next = new_node;
            }
            _node->prev = new_node;
        }
        size++;
        return new_node;
    }

    ListNode<T>* push_back(T _value)
    {
        ListNode<T>* new_node = nullptr;
        if (head == nullptr)
        {
            new_node = insert_back(nullptr, _value);
            head = new_node;
        }
        else
        {
            new_node = insert_back(tail, _value);
        }
        tail = new_node;

        return new_node;
    }

    ListNode<T>* push_front(T _value)
    {
        auto new_node = insert_front(head, _value);
        head = new_node;
        if (size == 1)
        {
            tail = new_node;
        }

        return new_node;
    }

    void erase(ListNode<T>* _node)
    {
        if (_node == nullptr)
        {
            return;
        }
        auto prev = _node->prev;
        auto next = _node->next;
        if (prev != nullptr)
        {
            prev->next = next;
        }
        if (next != nullptr)
        {
            next->prev = prev;
        }
        delete _node;
        size--;
    }

    void pop_back()
    {
        if (tail == nullptr)
        {
            return;
        }
        auto next = tail->next;
        erase(tail);
        if (size == 1)
        {
            tail = head;
        }
        else
        {
            tail = next;
        }
    }

    std::vector<T> to_vector() const
    {
        std::vector<T> result;
        auto current = head;
        while (current != nullptr)
        {
            result.push_back(current->value);
            current = current->next;
        }
        return result;
    }
};

TEST_CASE("List", "[insert_front]")
{
    List<int> list;
    list.push_back(1);
    auto note2 = list.push_back(2);
    list.push_back(3);
    REQUIRE(list.to_vector() == std::vector<int>{1, 2, 3});

    list.insert_front(note2, 4);
    REQUIRE(list.to_vector() == std::vector<int>{1, 4, 2, 3});
}

TEST_CASE("List", "[insert_back]")
{
    List<int> list;
    list.push_back(1);
    auto note2 = list.push_back(2);
    list.push_back(3);

    list.insert_back(note2, 4);
    REQUIRE(list.to_vector() == std::vector<int>{1, 2, 4, 3});
}

TEST_CASE("List", "[push_back]")
{
    List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    REQUIRE(list.to_vector() == std::vector<int>{1, 2, 3});
}

TEST_CASE("List", "[push_front]")
{
    List<int> list;
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);
    REQUIRE(list.to_vector() == std::vector<int>{3, 2, 1});
}

TEST_CASE("List", "[pop_back]")
{
    List<int> list;
    list.push_back(1);
    list.push_back(2);
    REQUIRE(list.to_vector() == std::vector<int>{1, 2});
    list.pop_back();
    REQUIRE(list.to_vector() == std::vector<int>{1});
    list.push_back(3);
    REQUIRE(list.to_vector() == std::vector<int>{1, 3});
}

int main(int _argc, char* _argv[])
{
    std::string log_format{"[%C-%m-%d %T.%e] [%^%L%$] [%-20!!:%4#] %v"};
    spdlog::set_pattern(log_format);
    auto result = Catch::Session().run(_argc, _argv);
    return result;
}
