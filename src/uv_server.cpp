#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "uv.h"
#include <boost/scope/defer.hpp>
#include <spdlog/spdlog.h>

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
    uv_write(req, _stream, &wrbuf, 1, on_write);
}

void on_connection(uv_stream_t* _server, int _status)
{
    SPDLOG_INFO("status: {}", _status);
    if (_status < 0)
    {
        SPDLOG_ERROR("connection error: {}", uv_strerror(_status));
        return;
    }

    auto client = new uv_tcp_t();
    uv_tcp_init(uv_default_loop(), client);
    client->data = new std::vector<char>();
    if (uv_accept(_server, (uv_stream_t*)client) == 0)
    {
        uv_read_start((uv_stream_t*)client, on_alloc, on_read);
    }
    else
    {
        uv_close((uv_handle_t*)client, on_close);
    }
}

int main()
{
    std::string log_format{"[%C-%m-%d %T.%e] [%^%L%$] [%-10!!:%4#] %v"};
    spdlog::set_pattern(log_format);

    uv_tcp_t server;
    uv_tcp_init(uv_default_loop(), &server);

    struct sockaddr_in addr;
    uv_ip4_addr("0.0.0.0", 12345, &addr);

    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    int r = uv_listen((uv_stream_t*)&server, 128, on_connection);
    if (r)
    {
        SPDLOG_ERROR("listen error: {}", uv_strerror(r));
        return 1;
    }

    SPDLOG_INFO("Listening on port 12345...");
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    return 0;
}
