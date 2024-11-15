#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#include <boost/scope/defer.hpp>
#include <spdlog/spdlog.h>
#include <uv.h>

void on_alloc(uv_handle_t* _handle, size_t _suggested_size, uv_buf_t* _buf)
{
    SPDLOG_INFO("suggested_size: {}", _suggested_size);
    _buf->base = new char[_suggested_size];
    _buf->len = _suggested_size;
}

void on_close(uv_handle_t* _handle)
{
    if (_handle != nullptr)
    {
        delete (std::vector<char>*)_handle->data;
        delete _handle;
    }
}

std::queue<std::function<void(uv_write_t* _req, int _status)>> g_write_task;
std::queue<std::function<void(uv_stream_t* _stream, ssize_t _nread, const uv_buf_t* _buf)>> g_read_task;
std::queue<std::function<void(uv_stream_t* _server, int _status)>> g_connection_task;

void on_connection(uv_stream_t* _server, int _status);

bool g_listen = false;

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

        front(_server, _status);
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

        front(_req, _status);
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

        front(_stream, _nread, _buf);
    }
} // namespace detail

void async_listen(uv_stream_t* stream, int backlog, std::function<void(uv_stream_t* _server, int _status)> _fn)
{
    if (!g_listen)
    {
        int r = uv_listen(stream, backlog, on_connection);
        if (r)
        {
            SPDLOG_ERROR("listen error: {}", uv_strerror(r));
            // _fn(stream, r);
            return;
        }
        g_listen = true;
    }

    // async_listen(stream, backlog, );

    g_connection_task.push(_fn);
}

void async_read(std::function<void(uv_stream_t* _stream, ssize_t _nread, const uv_buf_t* _buf)> _fn)
{
    g_read_task.push(_fn);
}

void async_write(std::function<void(uv_write_t* _req, int _status)> _fn)
{
    g_write_task.push(_fn);
}

void on_write_task(uv_write_t* _req, int _status)
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
    }
    uv_close((uv_handle_t*)_req->handle, on_close);
}

void on_read_task(uv_stream_t* _stream, ssize_t _nread, const uv_buf_t* _buf)
{
    BOOST_SCOPE_DEFER[&]
    {
        delete[] _buf->base;
    };
    if (_nread < 0)
    {
        // 如果讀取錯誤或連接已關閉，釋放內存並關閉客戶端
        uv_close((uv_handle_t*)_stream, on_close);
        SPDLOG_INFO("uv_close");
        SPDLOG_INFO("================");
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
    uv_write(req, _stream, &wrbuf, 1, detail::on_write);
    // g_write_task.push([](uv_write_t* _req, int _status) { on_write_task(_req, _status); });
    async_write([](uv_write_t* _req, int _status) { on_write_task(_req, _status); });
}

void on_connection_task(uv_stream_t* _server, int _status)
{
    SPDLOG_INFO("status: {}", _status);
    if (_status < 0)
    {
        SPDLOG_ERROR("connection error: {}", uv_strerror(_status));
        uv_close((uv_handle_t*)_server, on_close);
        return;
    }

    // async_listen([](uv_stream_t* _server, int _status) { on_connection_task(_server, _status); });
    async_listen((uv_stream_t*)&_server, 128,
                 [](uv_stream_t* _server, int _status) { on_connection_task(_server, _status); });

    auto client = new uv_tcp_t();
    uv_tcp_init(uv_default_loop(), client);
    client->data = new std::vector<char>();
    if (uv_accept(_server, (uv_stream_t*)client) == 0)
    {
        uv_read_start((uv_stream_t*)client, on_alloc, detail::on_read);
        // g_read_task.push([](uv_stream_t* _stream, ssize_t _nread, const uv_buf_t* _buf)
        //                  { on_read_task(_stream, _nread, _buf); });
        async_read([](uv_stream_t* _stream, ssize_t _nread, const uv_buf_t* _buf)
                   { on_read_task(_stream, _nread, _buf); });
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
    async_listen((uv_stream_t*)&server, 128,
                 [](uv_stream_t* _server, int _status) { on_connection_task(_server, _status); });

    SPDLOG_INFO("Listening on port {}...", port);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    return 0;
}
