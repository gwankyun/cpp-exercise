#include <string>

#include <spdlog/spdlog.h>

struct list_node_t;

typedef void (*destroy_cb)(void*);

struct list_node_t
{
    list_node_t* prev;
    list_node_t* next;
};

struct list_t
{
    list_node_t* front;
    list_node_t* back;
    size_t size;
};

void list_init(list_t* _list)
{
    _list->front = nullptr;
    _list->back = nullptr;
    _list->size = 0u;
}


void list_node_init(list_node_t* _node)
{
    _node->prev = nullptr;
    _node->next = nullptr;
}

void list_push_back(list_t* _list, list_node_t* _node)
{
    if (_list->size == 0)
    {
        _list->front = _node;
        _list->back = _node;
    }
    if (_list->size == 1)
    {
        _node->prev = _list->front;
        _list->front->next = _node;
        _list->back = _node;
    }
    else
    {
        _node->prev = _list->back;
        _list->back->next = _node;
        _list->back = _node;
    }
    _list->size++;
}

// void list_push_front(list_t* _list, list_node_t* _node)
// {
//     if (_list->size == 0)
//     {
//         _list->begin = _node;
//     }
//     else
//     {
//         _node->next = _list->begin;
//         _list->begin->prev = _node;
//         _list->begin = _node;
//     }
//     _list->size++;
// }

void list_pop(list_t* _list, list_node_t* _node, destroy_cb _destroy)
{
    if (_list->size == 1)
    {
        _list->front = nullptr;
        _list->back = nullptr;
    }
    else if (_node == _list->front)
    {
        _list->front = _list->front->next;
        _list->front->prev = nullptr;
    }
    else if (_node == _list->back)
    {
        _list->back = _list->back->prev;
        _list->back->next = nullptr;
    }

    if (_destroy != nullptr)
    {
        _destroy(_node);
    }

    _list->size--;
}

void list_destroy(list_t* _list, destroy_cb _destroy)
{
    // for (list_node_t* node = _list->front; node != nullptr; node = node->prev)
    // {
    //     if (_destroy != nullptr)
    //     {
    //         _destroy(node);
    //     }
    // }
    while (_list->size > 0)
    {
        list_pop(_list, _list->back, _destroy);
    }
}

// list_node_t* list_pop_front(list_node_t* _node, destroy_cb _destroy)
// {
//     list_node_t* next = _node->next;
//     next->prev = nullptr;
//     if (_destroy != nullptr)
//     {
//         _destroy(_node);
//     }
//     return prev;
// }

// std::size_t list_size(list_node_t* _node)
// {
//     std::size_t size = 0u;
//     auto node = _node;
//     while (node != nullptr)
//     {
//         size++;
//         node = node->next;
//     }
//     return size;
// }


// void destroy(void* _data)
// {
//     delete (int_list_t*)_data;
// }

int main(int _argc, char* _argv[])
{
    std::string log_format{"[%C-%m-%d %T.%e] [%^%L%$] [%-10!!:%4#] %v"};
    spdlog::set_pattern(log_format);

    SPDLOG_INFO("spdlog");

    list_t lst;
    list_init(&lst);

    enum class data_type
    {
        Default = 0,
        Stack,
        Heap
    };

    struct int_list_t
    {
        list_node_t base;
        int value;
        data_type type;
    };

    int_list_t h;
    h.value = 0;
    list_node_init((list_node_t*)&h);

    int_list_t h1;
    h1.value = 1;
    list_node_init((list_node_t*)&h1);
    list_push_back(&lst, (list_node_t*)&h1);

    int_list_t h2;
    h2.value = 2;
    list_node_init((list_node_t*)&h2);
    list_push_back(&lst, (list_node_t*)&h2);

    SPDLOG_INFO("size: {}", lst.size);

    auto h3 = new int_list_t();
    list_node_init((list_node_t*)h3);
    h3->value = 3;
    h3->type = data_type::Heap;
    list_push_back(&lst, (list_node_t*)h3);

    SPDLOG_INFO("size: {}", lst.size);
    SPDLOG_INFO("value: {}", ((int_list_t*)lst.back)->value);

    list_pop(&lst, lst.back, [](void* _data) { delete (int_list_t*)_data; });

    SPDLOG_INFO("size: {}", lst.size);
    SPDLOG_INFO("value: {}", ((int_list_t*)lst.back)->value);

    list_destroy(&lst,
                 [](void* _data)
                 {
                     int_list_t* data = (int_list_t*)_data;
                     if (data->type == data_type::Heap)
                     {
                         delete data;
                     }
                     SPDLOG_INFO("");
                 });

    return 0;
}
