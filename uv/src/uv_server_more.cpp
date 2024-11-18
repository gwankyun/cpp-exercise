#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>

#include <boost/scope/defer.hpp>
#include <spdlog/spdlog.h>
#include <uv.h>

void on_alloc(uv_handle_t* _handle, size_t _suggested_size, uv_buf_t* _buf)
{
    SPDLOG_INFO("suggested_size: {}", _suggested_size);
    _buf->base = new char[_suggested_size];
    _buf->len = _suggested_size;
}

std::unordered_map<uv_stream_t*, bool> g_read_table;
std::unordered_map<uv_write_t*, bool> g_write_table;

void on_close(uv_handle_t* _handle)
{
    if (_handle != nullptr)
    {
        auto ri = g_read_table.find((uv_stream_t*)_handle);
        if (ri != g_read_table.end())
        {
            g_read_table.erase(ri);
        }
        auto wi = g_write_table.find((uv_write_t*)_handle);
        if (wi != g_write_table.end())
        {
            g_write_table.erase(wi);
        }
        delete (std::vector<char>*)_handle->data;
        delete _handle;
    }
}

std::queue<std::function<void(int _status)>> g_write_task;
std::queue<std::function<void(ssize_t _nread, const uv_buf_t* _buf)>> g_read_task;
std::queue<std::function<void(int _status)>> g_connection_task;

void on_connection(uv_stream_t* _server, int _status);

namespace detail
{
    void on_connection(uv_stream_t* _server, int _status)
    {
        SPDLOG_INFO("");
        auto& task = g_connection_task;
        if (task.empty())
        {
            return;
        }

        auto front = task.front();
        g_connection_task.pop();

        front(_status);
    }

    void on_write(uv_write_t* _req, int _status)
    {
        SPDLOG_INFO("");
        auto& task = g_write_task;
        if (task.empty())
        {
            return;
        }

        auto front = task.front();
        task.pop();

        if (g_write_table.find(_req) != g_write_table.end())
        {
            front(_status);
        }
    }

    void on_read(uv_stream_t* _stream, ssize_t _nread, const uv_buf_t* _buf)
    {
        SPDLOG_INFO("");
        auto& task = g_read_task;
        if (task.empty())
        {
            return;
        }

        auto front = task.front();
        task.pop();

        if (g_read_table.find(_stream) != g_read_table.end())
        {
            front(_nread, _buf);
        }
    }
} // namespace detail

std::map<uv_stream_t*, bool> g_listem_table;

void async_listen(uv_stream_t* stream, int backlog, std::function<void(int _status)> _fn)
{
    auto iter = g_listem_table.find(stream);
    if (iter == g_listem_table.end())
    {
        int r = uv_listen(stream, backlog, on_connection);
        if (r)
        {
            SPDLOG_ERROR("listen error: {}", uv_strerror(r));
            return;
        }
        // g_listen = true;
        g_listem_table[stream] = true;
    }

    g_connection_task.push(_fn);
}


void async_read(uv_stream_t* stream, uv_alloc_cb alloc_cb,
                std::function<void(ssize_t _nread, const uv_buf_t* _buf)> _fn)
{
    auto iter = g_read_table.find(stream);
    if (iter == g_read_table.end())
    {
       uv_read_start(stream, on_alloc, detail::on_read);
       g_read_table[stream] = true;
    }

    g_read_task.push(_fn);
}

void async_write(uv_write_t* req, uv_stream_t* handle, const uv_buf_t bufs[], unsigned int nbufs,
                 std::function<void(int _status)> _fn)
{
    auto iter = g_write_table.find(req);
    if (iter == g_write_table.end())
    {
        //    uv_read_start(req, on_alloc, detail::on_read);
        uv_write(req, handle, bufs, nbufs, detail::on_write);
        g_write_table[req] = true;
    }

    g_write_task.push(_fn);
}

void on_read(uv_stream_t* _stream, ssize_t _nread, const uv_buf_t* _buf);

void on_write(uv_write_t* _req, int _status)
{
    SPDLOG_INFO("status: {}", _status);
    BOOST_SCOPE_DEFER[&]
    {
        delete (std::string*)_req->data;
        delete[] _req;
    };
    if (_status < 0)
    {
        SPDLOG_ERROR("write error: {}", uv_strerror(_status));
        uv_close((uv_handle_t*)_req->handle, on_close);
    }
    else
    {
        auto handle = (uv_stream_t*)_req->handle;
        async_read(handle, on_alloc, [handle](ssize_t _nread, const uv_buf_t* _buf) { on_read(handle, _nread, _buf); });
    }
}


void on_read(uv_stream_t* _stream, ssize_t _nread, const uv_buf_t* _buf)
{
    BOOST_SCOPE_DEFER[&]
    {
        delete[] _buf->base;
    };
    if (_nread < 0)
    {
        // 如果讀取錯誤或連接已關閉，釋放內存並關閉客戶端
        if (_stream != nullptr)
        {
            uv_close((uv_handle_t*)_stream, on_close);
            SPDLOG_INFO("uv_close");
            SPDLOG_INFO("================");
        }
        return;
    }

    SPDLOG_INFO("nread: {}", _nread);

    auto buffer = (std::vector<char>*)_stream->data;
    std::copy_n(_buf->base, _nread, std::back_inserter(*buffer));
    SPDLOG_INFO("buf: {}", std::string(buffer->data()));

    if (buffer->back() == '\0')
    {
        SPDLOG_INFO("read finished");
    }

    auto message = new std::string("Hello from server!");
    *message += '\0';
    auto req = new uv_write_t();
    req->data = message;
    auto wrbuf = uv_buf_init((char*)message->c_str(), message->size());
    async_write(req, _stream, &wrbuf, 1, [req](int _status) { on_write(req, _status); });
}

void on_connection(uv_stream_t* _server, int _status)
{
    SPDLOG_INFO("status: {}", _status);
    if (_status < 0)
    {
        SPDLOG_ERROR("connection error: {}", uv_strerror(_status));
        uv_close((uv_handle_t*)_server, on_close);
        return;
    }

    async_listen((uv_stream_t*)_server, 128,
                 [_server](int _status) { on_connection((uv_stream_t*)_server, _status); });

    auto client = new uv_tcp_t();
    uv_tcp_init(uv_default_loop(), client);
    client->data = new std::vector<char>();
    if (uv_accept(_server, (uv_stream_t*)client) == 0)
    {
        async_read((uv_stream_t*)client, on_alloc,
                   [client](ssize_t _nread, const uv_buf_t* _buf) { on_read((uv_stream_t*)client, _nread, _buf); });
    }
    else
    {
        uv_close((uv_handle_t*)client, on_close);
    }
}

int main(int _argc, char* _argv[])
{
    std::string log_format{"[%C-%m-%d %T.%e] [%^%L%$] [%-10!!:%4#] %v"};
    spdlog::set_pattern(log_format);

    auto port = 12345;

    if (_argc >= 2)
    {
        SPDLOG_INFO("_argv[0]: {}", _argv[1]);
        port = std::stoi(_argv[1]);
    }

    uv_tcp_t server;
    uv_tcp_init(uv_default_loop(), &server);

    struct sockaddr_in addr;
    uv_ip4_addr("0.0.0.0", port, &addr);

    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    async_listen((uv_stream_t*)&server, 128, [server](int _status) { on_connection((uv_stream_t*)&server, _status); });

    SPDLOG_INFO("Listening on port {}...", port);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    return 0;
}
