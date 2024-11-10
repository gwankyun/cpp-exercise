#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <functional>
#include <iostream>
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

void on_read(uv_stream_t* _stream, ssize_t _nread, const uv_buf_t* _buf)
{
    auto handle = (uv_handle_t*)_stream;
    BOOST_SCOPE_DEFER[&]
    {
        delete[] _buf->base;
    };
    if (_nread < 0)
    {
        // 如果讀取錯誤或連接已關閉，釋放內存並關閉客戶端
        uv_close(handle, on_close);
        return;
    }

    // 打印接收到的數據
    SPDLOG_INFO("nread: {}", _nread);

    auto buffer = (std::vector<char>*)_stream->data;
    std::copy_n(_buf->base, _nread, std::back_inserter(*buffer));
    SPDLOG_INFO("buf: {}", std::string(buffer->data()));

    if (buffer->back() == '\0')
    {
        SPDLOG_INFO("buf: {}", buffer->data());
        SPDLOG_INFO("read finished");
        uv_close(handle, on_close);
    }
}

void on_write(uv_write_t* _req, int _status)
{
    auto handle = _req->handle;
    SPDLOG_INFO("status: {}", _status);

    BOOST_SCOPE_DEFER
    [&]
    {
        if (_req != nullptr)
        {
            delete (std::string*)_req->data;
            delete[] _req;
        }
    };

    if (_status < 0)
    {
        SPDLOG_ERROR("uv_write error: {}", uv_strerror(_status));
        uv_close((uv_handle_t*)handle, on_close);
        return;
    }

    // 開始讀取數據
    uv_read_start(handle, on_alloc, on_read);
}

void on_connect(uv_connect_t* _req, int _status)
{
    auto handle = _req->handle;
    SPDLOG_INFO("status: {}", _status);
    if (_status < 0)
    {
        SPDLOG_ERROR("uv_tcp_connect error: {}", uv_strerror(_status));
        uv_close((uv_handle_t*)handle, on_close);
        return;
    }

    // 連接成功，發送數據
    auto message = new std::string("Hello from client!");
    *message += '\0';
    auto req = new uv_write_t();
    req->data = message;
    auto wrbuf = uv_buf_init((char*)message->c_str(), message->size());

    uv_write(req, handle, &wrbuf, 1, on_write);
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

    auto socket = new uv_tcp_t();
    uv_tcp_init(uv_default_loop(), socket);
    socket->data = new std::vector<char>();

    struct sockaddr_in dest;
    uv_ip4_addr("127.0.0.1", port, &dest);

    uv_connect_t connect_req;
    uv_tcp_connect(&connect_req, socket, (const struct sockaddr*)&dest, on_connect);

    SPDLOG_INFO("Connecting to server:{}...", port);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    return 0;
}
