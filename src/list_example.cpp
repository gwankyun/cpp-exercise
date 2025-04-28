#include <cstdint>
#include <vector>

#include <catch2/../catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include <spdlog/spdlog.h>

template<typename T>
struct ListNode
{
    T value;
    ListNode* next = nullptr;
    ListNode* prev = nullptr;
};

template<typename T>
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

    ListNode<T>* insert_back(ListNode<T>* _node, T _value)
    {
        auto new_node = new ListNode<T>;
        new_node->value = _value;
        new_node->next = _node->next;
        new_node->prev = _node;
        _node->next = new_node;
        size++;

        return new_node;
    }

    ListNode<T>* push_back(T _value)
    {
        auto new_node = new ListNode<T>;
        new_node->value = _value;
        if (head == nullptr)
        {
            head = new_node;
            tail = new_node;
        }
        else
        {
            tail->next = new_node;
            new_node->prev = tail;
            tail = new_node;
        }
        size++;

        return new_node;
    }

    void pop_back()
    {
        if (tail == nullptr)
        {
            return; 
        }
        tail = tail->prev;
        delete tail->next;
        tail->next = nullptr;
        size--;
    }

    std::vector<T> to_vector() const
    {
        std::vector<T> result;
        auto current = head;
        while (current!= nullptr)
        {
            result.push_back(current->value);
            current = current->next;
        }
        return result;
    }
};

TEST_CASE("List", "[push_back]")
{
    List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    REQUIRE(list.to_vector() == std::vector<int>{1, 2, 3});
}

TEST_CASE("List", "[pop_back]")
{
    List<int> list;
    list.push_back(1);
    list.push_back(2);
    list.pop_back();
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
